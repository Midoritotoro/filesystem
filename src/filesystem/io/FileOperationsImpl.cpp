#include <src/filesystem/io/FileOperationsImpl.h>
#include <src/filesystem/io/Overlapped.h>

#include <print>

__FILESYSTEM_IO_NAMESPACE_BEGIN

enum class __file_disposition : dword_t {
    __delete				= FILE_DISPOSITION_FLAG_DELETE,
    __posix_semantics		= FILE_DISPOSITION_FLAG_POSIX_SEMANTICS,
    __ignore_readonly_attr	= FILE_DISPOSITION_FLAG_IGNORE_READONLY_ATTRIBUTE
};

system::__fs_win_error __set_delete_flag(system::handle __handle) noexcept {
	FILE_DISPOSITION_INFO_EX __info = { 0 };
	__info.Flags = FILE_DISPOSITION_FLAG_DELETE | FILE_DISPOSITION_FLAG_POSIX_SEMANTICS;

	if (SetFileInformationByHandle(__handle.native(), FileDispositionInfoEx, &__info, sizeof(__info)))
		return system::__fs_win_error::__success;

	const auto __last_error = __get_last_win_error();

	switch (__last_error) {
		case system::__fs_win_error::__invalid_parameter:
		case system::__fs_win_error::__invalid_function:
		case system::__fs_win_error::__not_supported: break;
		case system::__fs_win_error::__access_denied:
		default: return __last_error;
	}

	FILE_DISPOSITION_INFO __disposition_info = { TRUE };

	if (SetFileInformationByHandle(__handle.native(), FileDispositionInfo, &__disposition_info, sizeof(__disposition_info)))
		return system::__fs_win_error::__success;

	return __get_last_win_error();
}

std::pair<bool, system::__fs_win_error> __fs_try_delete_via_disposition_ex(system::handle __handle) noexcept {
	FILE_DISPOSITION_INFO_EX __info = { 0 };
	__info.Flags = FILE_DISPOSITION_FLAG_DELETE | FILE_DISPOSITION_FLAG_POSIX_SEMANTICS
		| FILE_DISPOSITION_FLAG_IGNORE_READONLY_ATTRIBUTE;

	if (SetFileInformationByHandle(__handle.native(), FileDispositionInfoEx, &__info, sizeof(__info)))
		return { true, system::__fs_win_error::__success };

	const auto __last_error = __get_last_win_error();

	switch (__last_error) {
		case system::__fs_win_error::__invalid_parameter:
		case system::__fs_win_error::__invalid_function:
		case system::__fs_win_error::__not_supported: return { false,  system::__fs_win_error::__not_supported };
	}

	return { false, __last_error };
}

system::__fs_win_error __fs_try_clear_readonly_for_delete(system::handle __handle) noexcept {
	FILE_BASIC_INFO __info;

	if (!GetFileInformationByHandleEx(__handle.native(), FileBasicInfo, &__info, sizeof(__info)))
		return __get_last_win_error();

	const auto __is_readonly = (__info.FileAttributes & FILE_ATTRIBUTE_READONLY) != 0;

	if (__is_readonly) {
		__info.FileAttributes ^= FILE_ATTRIBUTE_READONLY;

		if (!SetFileInformationByHandle(__handle.native(), FileBasicInfo, &__info, sizeof(__info)))
			return __get_last_win_error();

		const auto __error = __set_delete_flag(__handle.native());

		if (__error == system::__fs_win_error::__success) {
			return system::__fs_win_error::__success;
		}
		else if (__error == system::__fs_win_error::__access_denied) {
			__info.FileAttributes |= FILE_ATTRIBUTE_READONLY;

			if (!SetFileInformationByHandle(__handle.native(), FileBasicInfo, &__info, sizeof(__info)))
				return __get_last_win_error();
		}

		return __error;
	}
	
	return system::__fs_win_error::__success;
}

std::pair<file, system::io_error> __fs_create_file(const path& __path, __fs_win_file_access_flags __access_flags,
	__fs_share_mode_flags __share_mode_flags, __fs_win_file_creation_disposition __creation_disposition,
	__fs_win_file_attributes __attributes, __fs_win_file_flags __flags)
{
	system::handle __handle = CreateFileW(__path.native().c_str(), __access_flags.value()
		| __flags.value(), __share_mode_flags.value(), NULL, 
		static_cast<dword_t>(__creation_disposition), __attributes.value(), NULL);

	if (!__handle.available()) return { file(), __get_last_win_error() };
	else return { file(__path, __handle), system::__fs_win_error::__success };
}

filesystem_nodiscard bool __is_running_asynchronously(system::__fs_win_error __error) noexcept {
	return __error == system::__fs_win_error::__error_io_pending
		|| __error == system::__fs_win_error::__error_more_data;
}

filesystem_nodiscard bool __is_eof(system::__fs_win_error __error) noexcept {
	return __error == system::__fs_win_error::__error_handle_eof;
}

sizetype __wait_for_async_io(_Overlapped& __overlapped, system::handle __handle) {
	dword_t __transferred_bytes = 0;
	const auto __ok = GetOverlappedResult(__handle.native(), &__overlapped, &__transferred_bytes, TRUE);

	if (!__ok) return (__get_last_win_error() == system::__fs_win_error::__error_more_data) ? __transferred_bytes : 0;
	return __transferred_bytes;
}

sizetype __fs_read_file(file& __file, mutable_buffer __buffer, sizetype __byte_offset) {
	sizetype __readed_bytes = 0;
	_Overlapped __overlapped;
	
	while (__readed_bytes < __buffer.size()) {
		__overlapped.set_offset(__byte_offset + __readed_bytes);

		auto __ok = ReadFile(__file.handle().native(), __buffer +
			__readed_bytes, __buffer.size() - __readed_bytes, NULL, &__overlapped);

		const auto __last_error = __get_last_win_error();

		if (!__ok && !__is_running_asynchronously(__last_error) && !__is_eof(__last_error))
			throw filesystem_error("Failed to read file", __make_std_error_code(__last_error));

		__readed_bytes += __wait_for_async_io(__overlapped, __file.handle());
		if (__is_eof(__get_last_win_error())) break;
	}

	return __readed_bytes;
}

sizetype __fs_read_file_async(file& __file, mutable_buffer __buffer, sizetype __byte_offset) {
	return 0;
}

sizetype __fs_write_file(file& __file, const_buffer __buffer, sizetype __byte_offset) {
	sizetype __written_bytes = 0;
	_Overlapped __overlapped;

	while (__written_bytes < __buffer.size()) {
		__overlapped.set_offset(__byte_offset + __written_bytes);

		auto __ok = WriteFile(__file.handle().native(), __buffer +
			__written_bytes, __buffer.size() - __written_bytes, NULL, &__overlapped);

		const auto __last_error = __get_last_win_error();

		if (!__ok && !__is_running_asynchronously(__last_error))
			throw filesystem_error("Failed to write file", __make_std_error_code(__last_error));

		__written_bytes += __wait_for_async_io(__overlapped, __file.handle());
	}

	return __written_bytes;
}

sizetype __fs_append_file(file& __file, const_buffer __buffer) {
	return __fs_write_file(__file, __buffer, static_cast<sizetype>(-1));
}

std::pair<bool, system::io_error> __fs_remove_file(const path& __path) {
	constexpr auto __flags = __fs_win_file_flags_data::__backup_semantics | __fs_win_file_flags_data::__open_reparse_point;

	auto [__file, __err] = __fs_create_file(__path, __fs_win_file_access_mode::__file_read_attributes |
		__fs_win_file_access_mode::__file_write_attributes | __fs_win_file_access_mode::__delete, __fs_win_share_mode::__none,
		__fs_win_file_creation_disposition::__open_existing, __fs_win_file_attributes_data::__normal, __flags);

	auto __has_permissions_to_change_file_attrs = false;

	if (__err)
		__has_permissions_to_change_file_attrs = true;
	else if (__err == system::io_error::access_denied) {
		auto [__for_delete, __err2] = __fs_create_file(__path, __fs_win_file_access_mode::__delete,
			__fs_win_share_mode::__none, __fs_win_file_creation_disposition::__open_existing,
			__fs_win_file_attributes_data::__normal, __flags);
		
		if (__err2 != system::io_error::access_denied) return { false, __err2 };
		__file = std::move(__for_delete);
	}
	else return { false, __err.value() == system::io_error::file_not_found ? system::io_error::success : __err };

	const auto [__ok, __err3] = __fs_try_delete_via_disposition_ex(__file.handle());

	if (!__ok && __err3 != system::io_error::not_supported)
		return { false, __err3 };

	const auto __err4 = __set_delete_flag(__file.handle());

	if (__err4 == system::io_error::success) {
		return { true, system::io_error::success };
	}
	else if (__err4 == system::io_error::access_denied && __has_permissions_to_change_file_attrs) {
		const auto __err5 = __fs_try_clear_readonly_for_delete(__file.handle());
		return { __err5 == system::io_error::success, __err5 };
	}

	return { false, __err4 };
}

bool __fs_change_file_attributes(const path& __path, __fs_win_file_attributes __attrs) {
	return SetFileAttributesW(__path.native().c_str(), __attrs.value());
}

__win_file_info __fs_file_info(file& __file) {
	BY_HANDLE_FILE_INFORMATION __info;
	GetFileInformationByHandle(__file.handle().native(), &__info);
	return __info;
}

__win_file_info __fs_file_info(const path& __path) {
	__win_file_info __info;
	GetFileAttributesExW(__path.native().c_str(), GetFileExInfoStandard, &__info);
	return __info;
}

bool __fs_file_exists(const path& __path) {
	WIN32_FILE_ATTRIBUTE_DATA __attrs;

	if (!GetFileAttributesExW(__path.native().c_str(), GetFileExInfoStandard, &__attrs))
		return false;

	return (__attrs.dwFileAttributes & ~INVALID_FILE_ATTRIBUTES);
}

__FILESYSTEM_IO_NAMESPACE_END
