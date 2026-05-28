#pragma once 

#include <filesystem/io/File.h>
#include <src/filesystem/io/MutableBuffer.h>
#include <src/filesystem/io/ConstBuffer.h>
#include <src/filesystem/system/IoError.h>
#include <src/filesystem/io/Overlapped.h>

__FILESYSTEM_IO_NAMESPACE_BEGIN

namespace detail {

template <class _Buffer_>
concept buffer_sequence = requires {
	typename _Buffer_::buffer_type;
	typename _Buffer_::size_type;
} && requires(typename _Buffer_::buffer_type ptr, typename _Buffer_::size_type sz) {
	_Buffer_{ ptr, sz};
} && requires(const _Buffer_ buf, typename _Buffer_::size_type sz) {
	{ buf.data() } -> std::same_as<typename _Buffer_::buffer_type>;
	{ buf.size() } -> std::same_as<typename _Buffer_::size_type>;
	{ buf + sz } -> std::same_as<_Buffer_>;
};

template <class _Buffer_>
concept mutable_buffer_sequence = buffer_sequence<_Buffer_> &&
    std::is_pointer_v<typename _Buffer_::buffer_type> &&
    !std::is_const_v<std::remove_pointer_t<typename _Buffer_::buffer_type>>;

template <class _Buffer_>
concept const_buffer_sequence = buffer_sequence<_Buffer_> &&
    std::is_pointer_v<typename _Buffer_::buffer_type> &&
    std::is_const_v<std::remove_pointer_t<typename _Buffer_::buffer_type>>;

enum class file_disposition : dword_t {
    delete_				= FILE_DISPOSITION_FLAG_DELETE,
    posix_semantics		= FILE_DISPOSITION_FLAG_POSIX_SEMANTICS,
    ignore_readonly_attr	= FILE_DISPOSITION_FLAG_IGNORE_READONLY_ATTRIBUTE
};

inline system::io_error set_delete_flag(system::handle handle) noexcept {
	FILE_DISPOSITION_INFO_EX info = { 0 };
	info.Flags = FILE_DISPOSITION_FLAG_DELETE | FILE_DISPOSITION_FLAG_POSIX_SEMANTICS;

	if (SetFileInformationByHandle(handle.native(), FileDispositionInfoEx, &info, sizeof(info)))
		return system::io_error::success;

	const auto last_error = system::io_error::last();

	switch (last_error.value()) {
		case system::io_error::invalid_parameter:
		case system::io_error::invalid_function:
		case system::io_error::not_supported: break;
		case system::io_error::access_denied:
		default: return last_error;
	}

	FILE_DISPOSITION_INFO disposition_info = { TRUE };

	if (SetFileInformationByHandle(handle.native(), FileDispositionInfo, &disposition_info, sizeof(disposition_info)))
		return system::io_error::success;

	return system::io_error::last();
}

inline system::io_error try_clear_readonly_for_delete(system::handle handle) noexcept {
	FILE_BASIC_INFO info;

	if (!GetFileInformationByHandleEx(handle.native(), FileBasicInfo, &info, sizeof(info)))
		return system::io_error::last();

	const auto is_readonly = (info.FileAttributes & FILE_ATTRIBUTE_READONLY) != 0;

	if (is_readonly) {
		info.FileAttributes ^= FILE_ATTRIBUTE_READONLY;

		if (!SetFileInformationByHandle(handle.native(), FileBasicInfo, &info, sizeof(info)))
			return system::io_error::last();

		const auto error = set_delete_flag(handle.native());

		if (error) return system::io_error::success;
		else if (error == system::io_error::access_denied) {
			info.FileAttributes |= FILE_ATTRIBUTE_READONLY;

			if (!SetFileInformationByHandle(handle.native(), FileBasicInfo, &info, sizeof(info)))
				return system::io_error::last();
		}

		return error;
	}
	
	return system::io_error::success;
}

filesystem_nodiscard inline std::pair<file, system::io_error> create_file(const path& path, win_file_access_flags access_flags,
	share_mode_flags share_mode_flags, win_file_creation_disposition creation_disposition,
	win_file_attributes attributes, win_file_flags flags)
{
	system::handle handle = CreateFileW(path.native().c_str(), access_flags.value()
		| flags.value(), share_mode_flags.value(), NULL, 
		static_cast<dword_t>(creation_disposition), attributes.value(), NULL);

	if (!handle.available()) return { file(), system::io_error::last() };
	else return { file(path, handle), system::io_error::success };
}

filesystem_nodiscard inline bool is_running_asynchronously(system::io_error error) noexcept {
	return error == system::io_error::io_pending || error == system::io_error::internal::more_data;
}

filesystem_nodiscard inline bool is_eof(system::io_error error) noexcept {
	return error == system::io_error::handle_eof;
}

sizetype wait_for_async_io(overlapped& overlapped, system::handle handle) {
	dword_t transferred_bytes = 0;
	const auto ok = GetOverlappedResult(handle.native(), &overlapped, &transferred_bytes, TRUE);

	if (!ok) return (system::io_error::last() == system::io_error::internal::more_data) ? transferred_bytes : 0;
	return transferred_bytes;
}

template <mutable_buffer_sequence _BufferSequence_>
inline std::pair<sizetype, system::io_error> read_file(file& file, _BufferSequence_ buffer, sizetype byte_offset) {
	sizetype readed_bytes = 0;
	overlapped overlapped;

	system::io_error last_error = system::io_error::success;
	
	while (readed_bytes < buffer.size()) {
		overlapped.set_offset(byte_offset + readed_bytes);

		auto ok = ReadFile(file.handle().native(), buffer +
			readed_bytes, buffer.size() - readed_bytes, NULL, &overlapped);

		last_error = system::io_error::last();

		if (!ok && !is_running_asynchronously(last_error) && !is_eof(last_error))
			return { readed_bytes, last_error };

		readed_bytes += wait_for_async_io(overlapped, file.handle());
		if (is_eof(system::io_error::last())) break;
	}

	return { readed_bytes, last_error };
}

template <buffer_sequence _BufferSequence_>
inline std::pair<sizetype, system::io_error> write_file(file& file, _BufferSequence_ buffer, sizetype byte_offset) {
	sizetype written_bytes = 0;
	overlapped overlapped;

	system::io_error last_error = system::io_error::success;

	while (written_bytes < buffer.size()) {
		overlapped.set_offset(byte_offset + written_bytes);

		auto ok = WriteFile(file.handle().native(), buffer +
			written_bytes, buffer.size() - written_bytes, NULL, &overlapped);

		last_error = system::io_error::last();

		if (!ok && !is_running_asynchronously(last_error))
			return { written_bytes, last_error };

		written_bytes += wait_for_async_io(overlapped, file.handle());
	}

	return { written_bytes, last_error };
}

template <buffer_sequence _BufferSequence_>
inline std::pair<sizetype, system::io_error> append_file(file& file, _BufferSequence_ buffer) {
	return write_file(file, buffer, static_cast<sizetype>(-1));
}

inline bool file_exists(const path& path) {
	return (GetFileAttributesW(path.native().c_str()) != INVALID_FILE_ATTRIBUTES);
}

inline system::io_error remove_file(const path& path) {
	constexpr auto flags = win_file_flags_data::none;// win_file_flags_data::backup_semantics | win_file_flags_data::open_reparse_point;

	auto [file, err] = create_file(path, win_file_access_mode::all_attributes, win_share_mode::all,
		win_file_creation_disposition::open_existing, win_file_attributes_data::normal, flags);

	auto has_permissions_to_change_file_attrs = false;

	if (err) has_permissions_to_change_file_attrs = true;
	else if (err == system::io_error::access_denied) {
		auto [for_delete, err2] = create_file(path, win_file_access_mode::delete_,
			win_share_mode::all, win_file_creation_disposition::open_existing,
			win_file_attributes_data::normal, flags);
		
		if (!err2) return err2;
		file = std::move(for_delete);
	}
	else return err;

	FILE_DISPOSITION_INFO_EX info = { 0 };
	info.Flags = FILE_DISPOSITION_FLAG_DELETE | FILE_DISPOSITION_FLAG_POSIX_SEMANTICS
		| FILE_DISPOSITION_FLAG_IGNORE_READONLY_ATTRIBUTE;

	if (SetFileInformationByHandle(file.handle().native(), FileDispositionInfoEx, &info, sizeof(info)))
		return system::io_error::success;

	const auto last_error = system::io_error::last();

	switch (last_error.value()) {
		case system::io_error::invalid_parameter:
		case system::io_error::invalid_function:
		case system::io_error::not_supported: break;
		default: return last_error;
	}

	const auto err4 = set_delete_flag(file.handle());

	if (err4 == system::io_error::access_denied && has_permissions_to_change_file_attrs)
		return try_clear_readonly_for_delete(file.handle());

	return err4;
}

inline bool change_file_attributes(const path& path, win_file_attributes attrs) {
	return SetFileAttributesW(path.native().c_str(), attrs.value());
}

inline win_file_info file_info(file& file) {
	BY_HANDLE_FILE_INFORMATION info;
	GetFileInformationByHandle(file.handle().native(), &info);
	return info;
}

inline win_file_info file_info(const path& path) {
	win_file_info info;
	GetFileAttributesExW(path.native().c_str(), GetFileExInfoStandard, &info);
	return info;
}

system::io_error rename_file(const path& file_path, const path& new_name) {
	if (MoveFileExW(file_path.c_str(), new_name.c_str(), MOVEFILE_REPLACE_EXISTING))
		return system::io_error::success;

	return system::io_error::last();
}

} // namespace detail

__FILESYSTEM_IO_NAMESPACE_END
