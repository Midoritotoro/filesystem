#pragma once 

#include <filesystem/io/File.h>
#include <src/filesystem/io/MutableBuffer.h>
#include <src/filesystem/io/ConstBuffer.h>
#include <src/filesystem/system/IoError.h>


__FILESYSTEM_IO_NAMESPACE_BEGIN

filesystem_nodiscard std::pair<file, system::io_error> __fs_create_file(const path&,
	__fs_win_file_access_flags, __fs_share_mode_flags, __fs_win_file_creation_disposition,
	__fs_win_file_attributes, __fs_win_file_flags);

filesystem_nodiscard std::pair<sizetype, system::io_error> __fs_read_file(file&, mutable_buffer, sizetype);
filesystem_nodiscard sizetype __fs_read_file_async(file&, mutable_buffer, sizetype);

filesystem_nodiscard std::pair<sizetype, system::io_error> __fs_write_file(file&, const_buffer, sizetype);
filesystem_nodiscard std::pair<sizetype, system::io_error> __fs_append_file(file&, const_buffer);

filesystem_nodiscard __win_file_info __fs_file_info(file&);
filesystem_nodiscard __win_file_info __fs_file_info(const path&);

filesystem_nodiscard bool __fs_file_exists(const path&);

filesystem_nodiscard std::pair<bool, system::io_error> __fs_remove_file(const path&);
filesystem_nodiscard bool __fs_change_file_attributes(const path&, __fs_win_file_attributes);

__FILESYSTEM_IO_NAMESPACE_END
