#pragma once 

#include <filesystem/compatibility/Compatibility.h>
#include <src/filesystem/Flags.h>

__FILESYSTEM_IO_NAMESPACE_BEGIN

namespace detail {

enum class win_file_attributes_data : u32 {
    read_only       = FILE_ATTRIBUTE_READONLY,
    hidden          = FILE_ATTRIBUTE_HIDDEN,
    system          = FILE_ATTRIBUTE_SYSTEM,
    directory       = FILE_ATTRIBUTE_DIRECTORY,
    archive         = FILE_ATTRIBUTE_ARCHIVE,
    device          = FILE_ATTRIBUTE_DEVICE,
    normal          = FILE_ATTRIBUTE_NORMAL,
    temporary       = FILE_ATTRIBUTE_TEMPORARY,
    sparse_file     = FILE_ATTRIBUTE_SPARSE_FILE,
    reparse_point   = FILE_ATTRIBUTE_REPARSE_POINT,
    invalid         = INVALID_FILE_ATTRIBUTES,
};

enum class win_file_flags_data : u32 {
    none                = 0,
    overlapped          = FILE_FLAG_OVERLAPPED,
    backup_semantics    = FILE_FLAG_BACKUP_SEMANTICS,
    open_reparse_point  = FILE_FLAG_OPEN_REPARSE_POINT
};

enum class win_reparse_tag : u32 {
    none          = 0,
    mount_point   = 0xA0000003L,
    symlink       = 0xA000000CL
};

using win_filetime = FILETIME;

struct win_file_info {
    win_file_info() noexcept {}
    win_file_info(const BY_HANDLE_FILE_INFORMATION& info) noexcept {
        attrs = info.dwFileAttributes;
        creation_time = info.ftCreationTime;
        last_access_time = info.ftLastAccessTime;
        last_write_time = info.ftLastWriteTime;
        file_size_high = info.nFileSizeHigh;
        file_size_low = info.nFileSizeLow;
    }

    dword_t attrs;

    win_filetime creation_time;
    win_filetime last_access_time;
    win_filetime last_write_time;

    dword_t file_size_high;
    dword_t file_size_low;
};

enum class win_share_mode : u32 {
	none    = 0,
    delete_ = FILE_SHARE_DELETE,
	read    = FILE_SHARE_READ, 
	write   = FILE_SHARE_WRITE
};

enum class win_file_access_mode : u32 {
    read                    = GENERIC_READ,
    write                   = GENERIC_WRITE,
    delete_                 = DELETE,
    file_read_attributes    = FILE_READ_ATTRIBUTES,
    file_write_attributes   = FILE_WRITE_ATTRIBUTES,
    all_attributes          = file_read_attributes | file_write_attributes | delete_,
    file_generic_write      = FILE_GENERIC_WRITE
};


enum class win_file_creation_disposition : u32 {
	create_always     = CREATE_ALWAYS,
	create_new        = CREATE_NEW,
	open_always       = OPEN_ALWAYS,
	open_existing     = OPEN_EXISTING,
	truncate_existing = TRUNCATE_EXISTING
};

FS_DECLARE_FLAGS(win_file_attributes, win_file_attributes_data);
FS_DECLARE_FLAGS(win_file_flags, win_file_flags_data);
FS_DECLARE_FLAGS(share_mode_flags, win_share_mode);
FS_DECLARE_FLAGS(win_file_access_flags, win_file_access_mode);

}

__FILESYSTEM_IO_NAMESPACE_END
