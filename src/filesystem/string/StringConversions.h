#pragma once 

#include <src/filesystem/Options.h>
#include <src/filesystem/system/WindowsErrors.h>
#include <src/filesystem/system/CodePages.h>
#include <string>
#include <ranges>
#include <algorithm>

__FILESYSTEM_NAMESPACE_BEGIN

#if defined(__cpp_char8_t)
  constexpr auto __has_utf8 = false;
#endif

template <class _String_>
concept string_type = requires { 
	typename std::basic_string<typename _String_::value_type,
		typename _String_::traits_type, typename _String_::allocator_type>; };

template <class _String1_, class _String2_>
concept compatible_strings = string_type<_String1_> && string_type<_String2_> && 
	(std::is_same_v<typename _String1_::value_type, typename _String2_::value_type>) &&
	(std::is_same_v<typename _String1_::traits_type, typename _String2_::traits_type>) &&
	(std::is_same_v<typename _String1_::allocator_type, typename _String2_::allocator_type>);

template <string_type _InStr_, string_type _OutStr_>
filesystem_always_inline _OutStr_ __dispatch_cvts(const _InStr_& __str, system::__code_page __page) noexcept {
	using _InChar = typename _InStr_::value_type;
	using _OutChar = typename _OutStr::value_type;

	constexpr auto __wch_to_ch = std::is_same_v<_InChar, wchar_t> && std::is_same_v<_OutChar, char>;
	constexpr auto __wch_to_utf8 = std::is_same_v<_InChar, wchar_t> && std::is_same_v<_OutChar, char8_t>;
	constexpr auto __wch_to_utf16 = std::is_same_v<_InChar, wchar_t> && std::is_same_v<_OutChar, char16_t>;
	constexpr auto __wch_to_utf32 = std::is_same_v<_InChar, wchar_t> && std::is_same_v<_OutChar, char32_t>;

	constexpr auto __utf8_to_ch = std::is_same_v<_InChar, char8_t> && std::is_same_v<_OutChar, char>;
	constexpr auto __utf8_to_wch = std::is_same_v<_InChar, char8_t> && std::is_same_v<_OutChar, wchar_t>;
	constexpr auto __utf8_to_utf16 = std::is_same_v<_InChar, char8_t> && std::is_same_v<_OutChar, char16_t>;
	constexpr auto __utf8_to_utf32 = std::is_same_v<_InChar, char8_t> && std::is_same_v<_OutChar, char32_t>;

	constexpr auto __utf16_to_ch = std::is_same_v<_InChar, char16_t> && std::is_same_v<_OutChar, char>;
	constexpr auto __utf16_to_wch = std::is_same_v<_InChar, char16_t> && std::is_same_v<_OutChar, wchar_t>;
	constexpr auto __utf16_to_utf8 = std::is_same_v<_InChar, char16_t> && std::is_same_v<_OutChar, char8_t>;
	constexpr auto __utf16_to_utf32 = std::is_same_v<_InChar, char16_t> && std::is_same_v<_OutChar, char32_t>;

	constexpr auto __utf32_to_ch = std::is_same_v<_InChar, char32_t> && std::is_same_v<_OutChar, char>;
	constexpr auto __utf32_to_wch = std::is_same_v<_InChar, char32_t> && std::is_same_v<_OutChar, wchar_t>;
	constexpr auto __utf32_to_utf8 = std::is_same_v<_InChar, char32_t> && std::is_same_v<_OutChar, char8_t>;
	constexpr auto __utf32_to_utf16 = std::is_same_v<_InChar, char32_t> && std::is_same_v<_OutChar, char16_t>;

	if constexpr (compatible_strings<_InStr_, _OutStr_>) return __str;
	else if constexpr (__utf8_to_wch || __utf8_to_utf16) {
		std::wstring __out_wstr;
		__out_wstr.reserve(__str.size());

		const auto __written = MultiByteToWideChar(static_cast<u32>(__page), MB_ERR_INVALID_CHARS, 
			__str.data(), __str.size(), __out_wstr.data(), __out_wstr.size());

		filesystem_debug_assert(__written == __out_wstr.size());
		return __out_wstr;
	}
	else if constexpr (std::is_same_v<_InChar, char> || __wch_to_utf16 || __utf16_to_wch) {
		_OutStr_ __out_str; __out_str.reserve(__str.size());
		std::ranges::copy(__str, __out_str);
		return __out_str;
	}
	else if constexpr (__wch_to_ch || __wch_to_utf8 || __utf16_to_ch || __utf16_to_utf8) {
		std::string __out_str;
		__out_str.reserve(__str.size());

		const auto __written = WideCharToMultiByte(static_cast<u32>(__page), WC_NO_BEST_FIT_CHARS,
			reinterpret_cast<LPCWCH>(__str.data()), __str.size(), reinterpret_cast<LPSTR>(__out_str.data()),
			__out_str.size(), NULL, TRUE);

		filesystem_debug_assert(__written = __out_str.size());
		return __out_str;
	}
	else if constexpr (__wch_to_utf32) {}
	else if constexpr (__utf8_to_ch) {}
	else if constexpr (__utf8_to_wch) {}
	else if constexpr (__utf8_to_utf16) {}
	else if constexpr (__utf8_to_utf32) {}
	else if constexpr (__utf16_to_utf32) {}
	else if constexpr (__utf32_to_ch) {}
	else if constexpr (__utf32_to_wch) {}
	else if constexpr (__utf32_to_utf16) {}
	else if constexpr (__utf32_to_utf8) {}
}

template <string_type _OutputString_>
struct _Configurable_str_cvt {
	template <class _Options_>
	struct __impl : fs::options::strict_elementwise_callable<__impl, _Options_> {
		template <string_type _InStr_>
		filesystem_nodiscard filesystem_always_inline _OutStr_ operator()(const _InStr_& __str,
			system::__code_page __page = system::__current_thread_code_page()) const noexcept
		{
			return fs::options::__dispatch_call(*this, __str, __page);
		}

		template <string_type _InStr_>
		filesystem_nodiscard static filesystem_always_inline auto deferred_call(auto __opts, 
			const _InStr_& __str, __code_page __page) noexcept 
		{
			return __dispatch_cvts<_OutStr_>(__str, __page);
		}

		using callable_type_tag = __impl;
	};
};

template <class _OutStr_> constexpr auto __str_cvt = raze::options::functor<_Configurable_str_cvt<_OutStr_>>;

__FILESYSTEM_NAMESPACE_END
