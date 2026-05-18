#pragma once 

#include <filesystem/compatibility/Compatibility.h>


__FILESYSTEM_IO_NAMESPACE_BEGIN

enum class __win_file_attributes: u32 {
    __readonly = FILE_ATTRIBUTE_READONLY,
    __hidden = FILE_ATTRIBUTE_HIDDEN,
    __system = FILE_ATTRIBUTE_SYSTEM,
    __directory = FILE_ATTRIBUTE_DIRECTORY,
    __archive = FILE_ATTRIBUTE_ARCHIVE,
    __device = FILE_ATTRIBUTE_DEVICE,
    __normal = FILE_ATTRIBUTE_NORMAL,
    __temporary = FILE_ATTRIBUTE_TEMPORARY,
    __sparse_file = FILE_ATTRIBUTE_SPARSE_FILE,
    __reparse_point = FILE_ATTRIBUTE_REPARSE_POINT,
    __invalid = INVALID_FILE_ATTRIBUTES
};

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
    __win_file_attributes __attributes = __invalid;

    __win_fs_filetime __creation_time;
    __win_fs_filetime __last_access_time;
    __win_fs_filetime __last_write_time;

    u32 __file_size_high;
    u32 __file_size_low;

    __win_fs_reparse_tag __reparse_point_tag;
    u32 __reserved1;

    wchar_t __file_name[__std_fs_max_path];
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

__FILESYSTEM_IO_NAMESPACE_END
