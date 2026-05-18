#pragma once 

#include <filesystem/compatibility/Compatibility.h>


__FILESYSTEM_SYSTEM_NAMESPACE_BEGIN

constexpr inline auto __fs_max_path = 260;

enum class __win_error : u32 {
    __success = ERROR_SUCCESS,
    __invalid_function = ERROR_INVALID_FUNCTION,
    __file_not_found = ERROR_FILE_NOT_FOUND,
    __path_not_found = ERROR_PATH_NOT_FOUND,
    __access_denied = ERROR_ACCESS_DENIED,
    __not_enough_memory = ERROR_NOT_ENOUGH_MEMORY,
    __no_more_files = ERROR_NO_MORE_FILES,
    __sharing_violation = ERROR_SHARING_VIOLATION,
    __not_supported = ERROR_NOT_SUPPORTED,
    __error_bad_netpath = ERROR_BAD_NETPATH,
    __error_netname_deleted = ERROR_NETNAME_DELETED,
    __file_exists = ERROR_FILE_EXISTS,
    __invalid_parameter = ERROR_INVALID_PARAMETER,
    __insufficient_buffer = ERROR_INSUFFICIENT_BUFFER,
    __invalid_name = ERROR_INVALID_NAME,
    __directory_not_empty = ERROR_DIR_NOT_EMPTY,
    __bad_pathname = ERROR_BAD_PATHNAME,
    __already_exists = ERROR_ALREADY_EXISTS,
    __filename_exceeds_range = ERROR_FILENAME_EXCED_RANGE,
    __directory_name_is_invalid = ERROR_DIRECTORY,
    __reparse_tag_invalid = ERROR_REPARSE_TAG_INVALID
};

__FILESYSTEM_SYSTEM_NAMESPACE_END

