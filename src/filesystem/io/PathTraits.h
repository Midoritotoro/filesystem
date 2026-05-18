#pragma once 

#include <filesystem/compatibility/Compatibility.h>
#include <string>

#if defined(filesystem_os_windows)
#  define __filesystem_native_text(__str) L##__str
#else
#  define __filesystem_native_text(__str) __str
#endif // defined(filesystem_os_windows)

__FILESYSTEM_IO_NAMESPACE_BEGIN

struct _Path_traits {
#if defined(filesystem_os_windows)
	using __value_type = wchar_t;
	static constexpr __value_type __preferred_separator = L'\\';
#else
	using __value_type = char;
	static constexpr __value_type __preferred_separator = '/'
#endif

	static constexpr __value_type __back_slash = __filesystem_native_text('\\');
	static constexpr __value_type __slash = __filesystem_native_text('/');
	static constexpr __value_type __dot = __filesystem_native_text('.');
	static constexpr __value_type __colon = __filesystem_native_text(':');

	using __string_type = std::basic_string<__value_type, std::char_traits<__value_type>, std::allocator<__value_type>>;
};

__FILESYSTEM_IO_NAMESPACE_END
