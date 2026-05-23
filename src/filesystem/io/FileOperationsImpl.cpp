#include <src/filesystem/io/FileOperationsImpl.h>
#include <src/filesystem/io/Overlapped.h>

#include <print>

__FILESYSTEM_IO_NAMESPACE_BEGIN

enum class __file_disposition : dword_t {
    __delete = FILE_DISPOSITION_FLAG_DELETE,
    __posix_semantics = FILE_DISPOSITION_FLAG_POSIX_SEMANTICS,
    __ignore_readonly_attr = FILE_DISPOSITION_FLAG_IGNORE_READONLY_ATTRIBUTE
};

file __fs_create_file(const path& __path, __fs_win_file_access_flags __access_flags,
	__fs_share_mode_flags __share_mode_flags, __fs_win_file_creation_disposition __creation_disposition,
	__fs_win_file_attributes __attributes) 
{
	auto __handle = CreateFileW(__path.native().c_str(), __access_flags.value(), __share_mode_flags.value(),
		NULL, static_cast<dword_t>(__creation_disposition), __attributes.value(), NULL);

	return file(__path, __handle);
}

filesystem_nodiscard bool __is_running_asynchronously(system_bool_t __ok, system::__fs_win_error __error) noexcept {
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

		if (!__ok && !__is_running_asynchronously(__ok, __last_error) && !__is_eof(__last_error))
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

		if (!__ok && !__is_running_asynchronously(__ok, __last_error))
			throw filesystem_error("Failed to write file", __make_std_error_code(__last_error));

		__written_bytes += __wait_for_async_io(__overlapped, __file.handle());
	}

	return __written_bytes;
}

sizetype __fs_append_file(file& __file, const_buffer __buffer) {
	return __fs_write_file(__file, __buffer, static_cast<sizetype>(-1));
}

bool __fs_remove_file(const path& __path) {
    constexpr auto __flags = __fs_win_file_attributes_data::__backup_semantics | __fs_win_file_attributes_data::__open_reparse_point;

	
}

__FILESYSTEM_IO_NAMESPACE_END
