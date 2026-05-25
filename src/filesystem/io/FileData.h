#pragma once 

#include <filesystem/compatibility/Compatibility.h>
#include <src/filesystem/Flags.h>

__FILESYSTEM_IO_NAMESPACE_BEGIN

constexpr inline auto __fs_max_path = 260;

enum class __fs_win_file_attributes_data : u32 {
    __read_only             = FILE_ATTRIBUTE_READONLY,
    __hidden                = FILE_ATTRIBUTE_HIDDEN,
    __system                = FILE_ATTRIBUTE_SYSTEM,
    __directory             = FILE_ATTRIBUTE_DIRECTORY,
    __archive               = FILE_ATTRIBUTE_ARCHIVE,
    __device                = FILE_ATTRIBUTE_DEVICE,
    __normal                = FILE_ATTRIBUTE_NORMAL,
    __temporary             = FILE_ATTRIBUTE_TEMPORARY,
    __sparse_file           = FILE_ATTRIBUTE_SPARSE_FILE,
    __reparse_point         = FILE_ATTRIBUTE_REPARSE_POINT,
    __invalid               = INVALID_FILE_ATTRIBUTES,
};

enum class __fs_win_file_flags_data : u32 {
    __none = 0,
    __overlapped = FILE_FLAG_OVERLAPPED,
    __backup_semantics = FILE_FLAG_BACKUP_SEMANTICS,
    __open_reparse_point = FILE_FLAG_OPEN_REPARSE_POINT
};

FS_DECLARE_FLAGS(__fs_win_file_attributes, __fs_win_file_attributes_data);
FS_DECLARE_FLAGS(__fs_win_file_flags, __fs_win_file_flags_data);

enum class __win_fs_reparse_tag : u32 {
    __none          = 0,
    __mount_point   = 0xA0000003L,
    __symlink       = 0xA000000CL
};

using __win_fs_filetime = FILETIME;

struct __win_file_info {
    __win_file_info() noexcept {}
    __win_file_info(const BY_HANDLE_FILE_INFORMATION& __info) noexcept {
        __attrs = __info.dwFileAttributes;
        __creation_time = __info.ftCreationTime;
        __last_access_time = __info.ftLastAccessTime;
        __last_write_time = __info.ftLastWriteTime;
        __file_size_high = __info.nFileSizeHigh;
        __file_size_low = __info.nFileSizeLow;
    }

    dword_t __attrs;

    __win_fs_filetime __creation_time;
    __win_fs_filetime __last_access_time;
    __win_fs_filetime __last_write_time;

    dword_t __file_size_high;
    dword_t __file_size_low;
};

enum class __fs_win_share_mode : u32 {
	__none      = 0,
	__delete    = FILE_SHARE_DELETE,
	__read      = FILE_SHARE_READ, 
	__write     = FILE_SHARE_WRITE
};

enum class __fs_win_file_access_mode : u32 {
    __read                      = GENERIC_READ,
    __write                     = GENERIC_WRITE,
    __delete                    = DELETE,
    __file_read_attributes      = FILE_READ_ATTRIBUTES,
    __file_write_attributes     = FILE_WRITE_ATTRIBUTES,
    __all_attributes            = __file_read_attributes | __file_write_attributes | __delete,
    __file_generic_write        = FILE_GENERIC_WRITE
};


enum class __fs_win_file_creation_disposition : u32 {
	__create_always     = CREATE_ALWAYS,
	__create_new        = CREATE_NEW,
	__open_always       = OPEN_ALWAYS,
	__open_existing     = OPEN_EXISTING,
	__truncate_existing = TRUNCATE_EXISTING
};

FS_DECLARE_FLAGS(__fs_share_mode_flags, __fs_win_share_mode);
FS_DECLARE_FLAGS(__fs_win_file_access_flags, __fs_win_file_access_mode);

__FILESYSTEM_IO_NAMESPACE_END
