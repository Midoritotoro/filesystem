#include <src/filesystem/io/FileOperationsImpl.h>
#include <src/filesystem/io/Overlapped.h>

__FILESYSTEM_IO_NAMESPACE_BEGIN

file __fs_create_file(const path& __path, __fs_win_file_access_flags __access_flags,
	__fs_share_mode_flags __share_mode_flags, __fs_win_file_creation_disposition __creation_disposition,
	__fs_win_file_attributes __attributes) throw()
{
	auto __handle = CreateFileW(__path.native().c_str(), __access_flags.value(), __share_mode_flags.value(),
		NULL, static_cast<dword_t>(__creation_disposition), __attributes.value(), NULL);

	return file(__path, __handle);
}

file __fs_open_file(const path& __path, __fs_win_file_access_flags __access_flags, 
	__fs_share_mode_flags __share_mode_flags, __fs_win_file_attributes __attributes) throw()
{
	return __fs_create_file(__path, __access_flags, __share_mode_flags,
		__fs_win_file_creation_disposition::__open_existing, __attributes);
}

sizetype __fs_read_file(file& __file, mutable_buffer __buffer, sizetype __byte_offset) throw() {
	dword_t __readed_bytes = 0;

	_Overlapped __overlapped;

	__overlapped.Offset = __byte_offset & 0xFFFFFFFFu;
	__overlapped.OffsetHigh = (__byte_offset >> 32u) & 0xFFFFFFFFu;

	const auto __ok = ReadFile(__file.native_handle().native_handle(), __buffer.data(), __buffer.size(), &__readed_bytes, &__overlapped);

	if (!__ok) throw filesystem_error("Failed to read file", __get_last_error_code());
	
	return __readed_bytes;
}

sizetype __fs_read_file_async(file& __file, mutable_buffer __buffer, sizetype __byte_offset) throw() {

}


__FILESYSTEM_IO_NAMESPACE_END
