#pragma once 

#include <filesystem/compatibility/Compatibility.h>
#include <src/filesystem/Flags.h>

__FILESYSTEM_IO_NAMESPACE_BEGIN

constexpr inline auto __fs_max_path = 260;

enum class __fs_win_file_attributes_data : u32 {
    __read_only = FILE_ATTRIBUTE_READONLY,
    __hidden = FILE_ATTRIBUTE_HIDDEN,
    __system = FILE_ATTRIBUTE_SYSTEM,
    __directory = FILE_ATTRIBUTE_DIRECTORY,
    __archive = FILE_ATTRIBUTE_ARCHIVE,
    __device = FILE_ATTRIBUTE_DEVICE,
    __normal = FILE_ATTRIBUTE_NORMAL,
    __temporary = FILE_ATTRIBUTE_TEMPORARY,
    __sparse_file = FILE_ATTRIBUTE_SPARSE_FILE,
    __reparse_point = FILE_ATTRIBUTE_REPARSE_POINT,
    __invalid = INVALID_FILE_ATTRIBUTES,
    __overlapped = FILE_FLAG_OVERLAPPED
};

FS_DECLARE_FLAGS(__fs_win_file_attributes, __fs_win_file_attributes_data);

enum class __win_fs_reparse_tag : u32 {
    __none = 0,
    __mount_point = 0xA0000003L,
    __symlink = 0xA000000CL
};

struct __win_fs_filetime {
    u32 __low = 0;
    u32 __high = 0;
};

struct __win_fs_find_data {
    __fs_win_file_attributes __attributes = __fs_win_file_attributes_data::__invalid;

    __win_fs_filetime __creation_time;
    __win_fs_filetime __last_access_time;
    __win_fs_filetime __last_write_time;

    u32 __file_size_high;
    u32 __file_size_low;

    __win_fs_reparse_tag __reparse_point_tag;
    u32 __reserved1;

    wchar_t __file_name[__fs_max_path];
    wchar_t __short_file_name[14];
};

enum class __win_fs_stats_flags : u32 {
    __none = 0,
    __follow_symlinks = 0x01,
    __attributes = 0x02,
    __reparse_tag = 0x04,
    __file_size = 0x08,
    __link_count = 0x10,
    __last_write_time = 0x20
};

enum class __fs_win_share_mode : u32 {
	__none = 0,
	__delete = FILE_SHARE_DELETE,
	__read = FILE_SHARE_READ, 
	__write = FILE_SHARE_WRITE
};

enum class __fs_win_file_access_mode : u32 {
	__read = GENERIC_READ,
	__write = GENERIC_WRITE,
};

enum class __fs_win_file_creation_disposition : u32 {
	__create_always = CREATE_ALWAYS,
	__create_new = CREATE_NEW,
	__open_always = OPEN_ALWAYS,
	__open_existing = OPEN_EXISTING,
	__truncate_existing = TRUNCATE_EXISTING
};

FS_DECLARE_FLAGS(__fs_share_mode_flags, __fs_win_share_mode);
FS_DECLARE_FLAGS(__fs_win_file_access_flags, __fs_win_file_access_mode);

__FILESYSTEM_IO_NAMESPACE_END
