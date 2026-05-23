#pragma once 

#include <filesystem/io/File.h>
#include <src/filesystem/io/MutableBuffer.h>
#include <src/filesystem/io/ConstBuffer.h>

__FILESYSTEM_IO_NAMESPACE_BEGIN

filesystem_nodiscard file __fs_create_file(const path& __path, __fs_win_file_access_flags __access_flags,
	__fs_share_mode_flags __share_mode_flags, __fs_win_file_creation_disposition __creation_disposition,
	__fs_win_file_attributes __attributes);

filesystem_nodiscard sizetype __fs_read_file(file& __file, mutable_buffer __buffer, sizetype __byte_offset);
filesystem_nodiscard sizetype __fs_read_file_async(file& __file, mutable_buffer __buffer, sizetype __byte_offset);

filesystem_nodiscard sizetype __fs_write_file(file& __file, const_buffer __buffer, sizetype __byte_offset);
filesystem_nodiscard sizetype __fs_append_file(file& __file, const_buffer __buffer);



filesystem_nodiscard bool __fs_remove_file(const path& __path);

__FILESYSTEM_IO_NAMESPACE_END
