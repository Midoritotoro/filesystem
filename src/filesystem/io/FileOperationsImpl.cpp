#include <src/filesystem/io/FileOperationsImpl.h>
#include <src/filesystem/io/Overlapped.h>

#include <print>

__FILESYSTEM_IO_NAMESPACE_BEGIN

file __fs_create_file(const path& __path, __fs_win_file_access_flags __access_flags,
	__fs_share_mode_flags __share_mode_flags, __fs_win_file_creation_disposition __creation_disposition,
	__fs_win_file_attributes __attributes) 
{
	auto __handle = CreateFileW(__path.native().c_str(), __access_flags.value(), __share_mode_flags.value(),
		NULL, static_cast<dword_t>(__creation_disposition), __attributes.value(), NULL);

	std::println("access = {:08X}", __access_flags.value());
	std::println("share  = {:08X}", __share_mode_flags.value());
	std::println("disp   = {:08X}", static_cast<DWORD>(__creation_disposition));
	std::println("attrs  = {:08X}", __attributes.value());

	return file(__path, __handle);
}

file __fs_open_file(const path& __path, __fs_win_file_access_flags __access_flags, 
	__fs_share_mode_flags __share_mode_flags, __fs_win_file_attributes __attributes)
{
	return __fs_create_file(__path, __access_flags, __share_mode_flags,
		__fs_win_file_creation_disposition::__open_existing, __attributes);
}

filesystem_nodiscard bool __is_running_asynchronously(system_bool_t __ok, system::__fs_win_error __error) noexcept {
	return __error == system::__fs_win_error::__error_io_pending
		|| __error == system::__fs_win_error::__error_more_data;
}

filesystem_nodiscard bool __is_eof(system::__fs_win_error __error) noexcept {
	return __error == system::__fs_win_error::__error_handle_eof;
}

sizetype __wait_for_async_read(_Overlapped& __overlapped, system::handle& __handle) {
	dword_t __transferred_bytes = 0;
	const auto __ok = GetOverlappedResult(__handle.native(), &__overlapped, &__transferred_bytes, TRUE);

	if (!__ok)
		return (__get_last_win_error() == system::__fs_win_error::__error_more_data) ? __transferred_bytes : 0;

	return __transferred_bytes;
}

sizetype __fs_read_file(file& __file, mutable_buffer __buffer, sizetype __byte_offset) {
	sizetype __readed_bytes = 0;
	_Overlapped __overlapped;
	
	while (__readed_bytes < __buffer.size()) {
		__overlapped.set_offset(__byte_offset + __readed_bytes);

		auto __ok = ReadFile(__file.handle().native(), reinterpret_cast<char*>(__buffer.data()) +
			__readed_bytes, __buffer.size() - __readed_bytes, NULL, &__overlapped);

		const auto __last_error = __get_last_win_error();

		if (!__ok && !__is_running_asynchronously(__ok, __last_error) && !__is_eof(__last_error)) {
			throw filesystem_error("Failed to read file", __get_last_error_code());
		}

		__readed_bytes += __wait_for_async_read(__overlapped, __file.handle());
	}

	return __readed_bytes;
}

sizetype __fs_read_file_async(file& __file, mutable_buffer __buffer, sizetype __byte_offset) {
	return 0;
}

filesystem_nodiscard sizetype __fs_write_file(file& __file, buffer __buffer, sizetype __byte_offset) {
	return 0;
}

__FILESYSTEM_IO_NAMESPACE_END
