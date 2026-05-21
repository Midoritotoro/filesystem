#include <src/filesystem/io/FileOperationsImpl.h>


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

filesystem_nodiscard sizetype __fs_read_file(file& __file, void* __buffer, dword_t __bytes_count,
	dword_t __byte_offset) throw()
{
	dword_t __readed_bytes = 0;
	ReadFile(__file.native_handle().native_handle(), __buffer, __bytes_count, &__readed_bytes, NULL);
}

__FILESYSTEM_IO_NAMESPACE_END
