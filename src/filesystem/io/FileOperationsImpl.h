#pragma once 

#include <filesystem/io/File.h>
#include <src/filesystem/io/Exceptions.h>

__FILESYSTEM_IO_NAMESPACE_BEGIN

filesystem_nodiscard file __fs_create_file(const path& __path, __fs_win_file_access_flags __access_flags,
	__fs_share_mode_flags __share_mode_flags, __fs_win_file_creation_disposition __creation_disposition,
	__fs_win_file_attributes __attributes) throw();

filesystem_nodiscard file __fs_open_file(const path& __path, __fs_win_file_access_flags __access_flags,
	__fs_share_mode_flags __share_mode_flags, __fs_win_file_attributes __attributes) throw();

filesystem_nodiscard sizetype __fs_read_file(file& __file, void* __buffer, dword_t __bytes_count,
	dword_t __byte_offset) throw();

__FILESYSTEM_IO_NAMESPACE_END
