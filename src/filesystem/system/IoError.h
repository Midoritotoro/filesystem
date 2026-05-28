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

class io_error {
public:
	using underlying_type = dword_t;

    enum class category : i32  {
        known,
        internal
    };

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
    static constexpr underlying_type invalid_handle         = ERROR_INVALID_HANDLE;

    struct internal {
        static constexpr underlying_type more_data = ERROR_MORE_DATA;
    };

	constexpr io_error() noexcept : _value(success) {}
	constexpr io_error(underlying_type value) noexcept : _value(value) {}

	constexpr underlying_type value() const noexcept { 
        return _value; 
    }

    constexpr category type() const noexcept {
        return is_public_code(_value) ? category::known : category::internal;
    }

    constexpr bool is_public() const noexcept {
        return type() == category::known;
    }

    constexpr bool is_internal() const noexcept {
        return type() == category::internal;
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
    io_error translate_to_success(_IoError_ ... errors) noexcept {
        if (((_value == errors) || ...))
            return io_error::success;

        return _value;
    }

    static io_error last() noexcept { 
        return io_error(GetLastError());
    }
private:
    static constexpr bool is_public_code(underlying_type value) noexcept {
        switch (value) {
            case success:
            case file_not_found:
            case path_not_found:
            case access_denied:
            case sharing_violation:
            case file_exists:
            case already_exists:
            case directory_not_empty:
            case invalid_name:
            case bad_path:
            case invalid_parameter:
            case invalid_function:
            case insufficient_buffer:
            case not_supported:
            case io_pending:
            case handle_eof:
            case invalid_handle:
            case internal::more_data: return true;
            default: return false;
        }
    }
    
	underlying_type _value;
};

class io_error_category_ final : public std::error_category {
public:
    filesystem_always_inline const char* name() const noexcept override { 
        return "fs.io";
    }

    filesystem_always_inline std::string message(i32 ev) const noexcept override {
        switch (ev) {
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
            case io_error::invalid_handle:          return "Invalid handle";
            case io_error::internal::more_data:     return "Internal filesystem error (unknown Win32 code)";
            default:                                return "Unknown filesystem error (code: " + std::to_string(ev) + ")";
        }
    }

    filesystem_always_inline std::error_condition default_error_condition(i32 ev) const noexcept override {
        switch (ev) {
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
            case io_error::invalid_handle:      return std::errc::invalid_argument;
            default:                            return std::error_condition(ev, *this);
        }
    }

    filesystem_always_inline bool equivalent(i32 code, const std::error_condition& condition) const noexcept override {
        return default_error_condition(code) == condition || std::error_condition(code, *this) == condition;
    }
};

const io_error_category_ io_category_instance {};

inline const std::error_category& io_error_category() noexcept {
    return io_category_instance;
}

inline std::error_code make_error_code(io_error error) noexcept {
    return std::error_code(error.value(), io_error_category());
}

__FILESYSTEM_SYSTEM_NAMESPACE_END

namespace std {
    template <> 
    struct is_error_code_enum<::fs::system::io_error>: true_type {}; 
}