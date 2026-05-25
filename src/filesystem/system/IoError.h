#pragma once

#if !defined(_SILENCE_CXX23_UNIX_STREAMS_DEPRECATION_WARNING)
#  define _SILENCE_CXX23_UNIX_STREAMS_DEPRECATION_WARNING
#endif // !defined(_SILENCE_CXX23_UNIX_STREAMS_DEPRECATION_WARNING)

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
    static constexpr underlying_type invalid_function       = ERROR_INVALID_FUNCTION;
    static constexpr underlying_type insufficient_buffer    = ERROR_INSUFFICIENT_BUFFER;
    static constexpr underlying_type not_supported          = ERROR_NOT_SUPPORTED;
    static constexpr underlying_type io_pending             = ERROR_IO_PENDING;
    static constexpr underlying_type handle_eof             = ERROR_HANDLE_EOF;
    static constexpr underlying_type internal_error         = 0xFFFFF000;

    struct internal {
        static constexpr underlying_type more_data = ERROR_MORE_DATA;
    };

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

    template <class ... _IoError_>
    io_error translate_to_success(_IoError_ ... __errors) noexcept {
        if (((_value == __errors) || ...))
            return io_error::success;

        return _value;
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
        switch (__ev) {
            case io_error::success:                 return "Success";
            case io_error::file_not_found:          return "File not found";
            case io_error::path_not_found:          return "Path not found";
            case io_error::access_denied:           return "Access denied";
            case io_error::sharing_violation:       return "Sharing violation (file in use)";
            case io_error::file_exists:             return "File already exists";
            case io_error::already_exists:          return "File or directory already exists";
            case io_error::directory_not_empty:     return "Directory is not empty";
            case io_error::invalid_name:            return "Invalid filename or path";
            case io_error::bad_path:                return "Bad or malformed path";
            case io_error::invalid_parameter:       return "Invalid parameter";
            case io_error::insufficient_buffer:     return "Insufficient buffer size";
            case io_error::not_supported:           return "Operation not supported on this OS/version";
            case io_error::io_pending:              return "I/O operation pending (async)";
            case io_error::handle_eof:              return "End of file reached";
            case io_error::invalid_function:        return "Invalid function";
            case io_error::internal_error:          return "Internal filesystem error (unknown Win32 code)";
            default:                                return "Unknown filesystem error (code: " + std::to_string(__ev) + ")";
        }
    }

    filesystem_always_inline std::error_condition default_error_condition(i32 __ev) const noexcept override {
        switch (__ev) {
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
            case io_error::io_pending:          return std::errc::operation_in_progress;
            case io_error::handle_eof:          return std::errc::no_message_available;
            case io_error::invalid_function:    return std::errc::function_not_supported;
            default:                            return std::error_condition(__ev, *this);
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
    return std::error_code(__err.value(), io_error_category());
}

inline std::string_view to_string(io_error e) noexcept {
    switch (e.value()) {
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
        case io_error::io_pending:             return "I/O pending";
        case io_error::handle_eof:             return "Handle EOF";
        case io_error::internal_error:         return "Internal error";
        default:                               return "Unknown";
    }
}

inline io_error __from_win32_error(system::__fs_win_error __err) noexcept {
    switch (__err) {
        case system::__fs_win_error::__success:             return io_error::success;
        case system::__fs_win_error::__file_not_found:      return io_error::file_not_found;
        case system::__fs_win_error::__path_not_found:      return io_error::path_not_found;
        case system::__fs_win_error::__access_denied:       return io_error::access_denied;
        case system::__fs_win_error::__sharing_violation:   return io_error::sharing_violation;
        case system::__fs_win_error::__file_exists:         return io_error::file_exists;
        case system::__fs_win_error::__already_exists:      return io_error::already_exists;
        case system::__fs_win_error::__directory_not_empty: return io_error::directory_not_empty;
        case system::__fs_win_error::__invalid_name:        return io_error::invalid_name;
        case system::__fs_win_error::__bad_pathname:        return io_error::bad_path;
        case system::__fs_win_error::__invalid_parameter:   return io_error::invalid_parameter;
        case system::__fs_win_error::__insufficient_buffer: return io_error::insufficient_buffer;
        case system::__fs_win_error::__not_supported:       return io_error::not_supported;
        case system::__fs_win_error::__error_io_pending:    return io_error::io_pending;
        case system::__fs_win_error::__error_handle_eof:    return io_error::handle_eof;
        default:                                            return io_error::internal_error;
    }
}

__FILESYSTEM_SYSTEM_NAMESPACE_END

namespace std {
    template <> 
    struct is_error_code_enum<::fs::system::io_error>: true_type {}; 
}