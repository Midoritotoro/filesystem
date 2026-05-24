#pragma once

#include <system_error>
#include <string>
#include <string_view>
#include <cstdint>

#include <filesystem/compatibility/Compatibility.h>

__FILESYSTEM_SYSTEM_NAMESPACE_BEGIN

enum class __fs_win_error : u32 {
    __success                   = ERROR_SUCCESS,
    __invalid_function          = ERROR_INVALID_FUNCTION,
    __file_not_found            = ERROR_FILE_NOT_FOUND,
    __path_not_found            = ERROR_PATH_NOT_FOUND,
    __access_denied             = ERROR_ACCESS_DENIED,
    __not_enough_memory         = ERROR_NOT_ENOUGH_MEMORY,
    __no_more_files             = ERROR_NO_MORE_FILES,
    __sharing_violation         = ERROR_SHARING_VIOLATION,
    __not_supported             = ERROR_NOT_SUPPORTED,
    __error_bad_netpath         = ERROR_BAD_NETPATH,
    __error_netname_deleted     = ERROR_NETNAME_DELETED,
    __file_exists               = ERROR_FILE_EXISTS,
    __invalid_parameter         = ERROR_INVALID_PARAMETER,
    __insufficient_buffer       = ERROR_INSUFFICIENT_BUFFER,
    __invalid_name              = ERROR_INVALID_NAME,
    __directory_not_empty       = ERROR_DIR_NOT_EMPTY,
    __bad_pathname              = ERROR_BAD_PATHNAME,
    __already_exists            = ERROR_ALREADY_EXISTS,
    __filename_exceeds_range    = ERROR_FILENAME_EXCED_RANGE,
    __directory_name_is_invalid = ERROR_DIRECTORY,
    __reparse_tag_invalid       = ERROR_REPARSE_TAG_INVALID,
    __error_io_pending          = ERROR_IO_PENDING,
    __error_more_data           = ERROR_MORE_DATA,
    __error_handle_eof          = ERROR_HANDLE_EOF
};

class io_error {
public:
	using underlying_type = dword_t;

    static constexpr underlying_type success                = ERROR_SUCCESS;
    static constexpr underlying_type file_not_found         = ERROR_FILE_NOT_FOUND;
    static constexpr underlying_type path_not_found         = ERROR_PATH_NOT_FOUND;
    static constexpr underlying_type access_denied          = ERROR_ACCESS_DENIED;
    static constexpr underlying_type sharing_violation      = ERROR_SHARING_VIOLATION;
    static constexpr underlying_type file_exists            = ERROR_FILE_EXISTS;
    static constexpr underlying_type already_exists         = ERROR_ALREADY_EXISTS;
    static constexpr underlying_type directory_not_empty    = ERROR_DIR_NOT_EMPTY;
    static constexpr underlying_type invalid_name           = ERROR_INVALID_NAME;
    static constexpr underlying_type bad_path               = ERROR_BAD_PATHNAME;
    static constexpr underlying_type invalid_parameter      = ERROR_INVALID_PARAMETER;
    static constexpr underlying_type insufficient_buffer    = ERROR_INSUFFICIENT_BUFFER;
    static constexpr underlying_type not_supported          = ERROR_NOT_SUPPORTED;
    static constexpr underlying_type network_unavailable    = ERROR_BAD_NETPATH;
    static constexpr underlying_type io_pending             = ERROR_IO_PENDING;
    static constexpr underlying_type handle_eof             = ERROR_HANDLE_EOF;
    static constexpr underlying_type internal_error         = 0xFFFFF000;

	constexpr io_error() noexcept : _value(success) {}
	constexpr io_error(underlying_type value) noexcept : _value(value) {}

	constexpr underlying_type value() const noexcept { 
        return _value; 
    }

	constexpr explicit operator bool() const noexcept {
		return _value == success;
	}

	bool operator==(const io_error& other) const noexcept {
		return _value == other._value;
	}

	bool operator!=(const io_error& other) const noexcept {
		return !(*this == other);
	}

    bool operator==(underlying_type value) const noexcept {
        return _value == value;
    }

	bool operator!=(underlying_type value) const noexcept {
		return !(*this == value);
	}
private:
	underlying_type _value;
};

class _Io_error_category final : public std::error_category {
public:
    filesystem_always_inline const char* name() const noexcept override { 
        return "fs.io";
    }

    filesystem_always_inline std::string message(i32 __ev) const noexcept override {
        switch (static_cast<io_error>(__ev)) {
            case io_error::success:                return "Success";
            case io_error::file_not_found:         return "File not found";
            case io_error::path_not_found:         return "Path not found";
            case io_error::access_denied:          return "Access denied";
            case io_error::sharing_violation:      return "Sharing violation (file in use)";
            case io_error::file_exists:            return "File already exists";
            case io_error::already_exists:         return "File or directory already exists";
            case io_error::directory_not_empty:    return "Directory is not empty";
            case io_error::invalid_name:           return "Invalid filename or path";
            case io_error::bad_path:               return "Bad or malformed path";
            case io_error::invalid_parameter:      return "Invalid parameter";
            case io_error::insufficient_buffer:    return "Insufficient buffer size";
            case io_error::not_supported:          return "Operation not supported on this OS/version";
            case io_error::network_unavailable:    return "Network path unavailable";
            case io_error::io_pending:             return "I/O operation pending (async)";
            case io_error::handle_eof:             return "End of file reached";
            case io_error::internal_error:         return "Internal filesystem error (unknown Win32 code)";
            default:                               return "Unknown filesystem error (code: " + std::to_string(ev) + ")";
        }
    }

    filesystem_always_inline std::error_condition default_error_condition(int ev) const noexcept override {
        switch (static_cast<io_error>(ev)) {
            case io_error::file_not_found:      return std::errc::no_such_file_or_directory;
            case io_error::path_not_found:      return std::errc::no_such_file_or_directory;
            case io_error::access_denied:       return std::errc::permission_denied;
            case io_error::sharing_violation:   return std::errc::device_or_resource_busy;
            case io_error::file_exists:         return std::errc::file_exists;
            case io_error::already_exists:      return std::errc::file_exists;
            case io_error::directory_not_empty: return std::errc::directory_not_empty;
            case io_error::invalid_name:        return std::errc::invalid_argument;
            case io_error::bad_path:            return std::errc::no_such_file_or_directory;
            case io_error::invalid_parameter:   return std::errc::invalid_argument;
            case io_error::insufficient_buffer: return std::errc::no_buffer_space;
            case io_error::not_supported:       return std::errc::not_supported;
            case io_error::network_unavailable: return std::errc::network_down;
            case io_error::io_pending:          return std::errc::operation_in_progress;
            case io_error::handle_eof:          return std::errc::no_message_available;
            default:                            return std::error_condition(ev, *this);
        }
    }

    filesystem_always_inline bool equivalent(i32 __code, const std::error_condition& __condition) const noexcept override {
        return default_error_condition(__code) == __condition || std::error_condition(__code, *this) == __condition;
    }
};

const _Io_error_category io_category_instance {};

inline const std::error_category& io_error_category() noexcept {
    return io_category_instance;
}

inline std::error_code make_error_code(io_error __err) noexcept {
    return std::error_code(static_cast<i32>(__err), io_error_category());
}

inline std::string_view to_string(io_error e) noexcept {
    switch (e) {
        case io_error::success:                return "Success";
        case io_error::file_not_found:         return "File not found";
        case io_error::path_not_found:         return "Path not found";
        case io_error::access_denied:          return "Access denied";
        case io_error::sharing_violation:      return "Sharing violation";
        case io_error::file_exists:            return "File exists";
        case io_error::already_exists:         return "Already exists";
        case io_error::directory_not_empty:    return "Directory not empty";
        case io_error::invalid_name:           return "Invalid name";
        case io_error::bad_path:               return "Bad path";
        case io_error::invalid_parameter:      return "Invalid parameter";
        case io_error::insufficient_buffer:    return "Insufficient buffer";
        case io_error::not_supported:          return "Not supported";
        case io_error::network_unavailable:    return "Network unavailable";
        case io_error::io_pending:             return "I/O pending";
        case io_error::handle_eof:             return "Handle EOF";
        case io_error::internal_error:         return "Internal error";
        default:                               return "Unknown";
    }
}

__FILESYSTEM_SYSTEM_NAMESPACE_END

namespace std {
    template <> 
    struct is_error_code_enum<::fs::system::io_error>: true_type {}; 
}