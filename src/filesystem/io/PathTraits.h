#pragma once 

#include <filesystem/compatibility/Compatibility.h>
#include <string>

#if defined(filesystem_os_windows)
#  define filesystem_native_text(str) L##str
#else
#  define filesystem_native_text(str) str
#endif // defined(filesystem_os_windows)

__FILESYSTEM_IO_NAMESPACE_BEGIN

struct Path_traits {
#if defined(filesystem_os_windows)
	using value_type = wchar_t;
	static constexpr value_type preferred_separator = L'\\';
#else
	using value_type = char;
	static constexpr value_type preferred_separator = '/'
#endif

	static constexpr value_type back_slash = filesystem_native_text('\\');
	static constexpr value_type slash = filesystem_native_text('/');
	static constexpr value_type dot = filesystem_native_text('.');
	static constexpr value_type colon = filesystem_native_text(':');

	using string_type = std::basic_string<value_type, std::char_traits<value_type>, std::allocator<value_type>>;
};

__FILESYSTEM_IO_NAMESPACE_END
