#pragma once 

#include <src/filesystem/Options.h>
#include <src/filesystem/system/WindowsErrors.h>
#include <src/filesystem/system/CodePages.h>
#include <src/filesystem/string/CharTraits.h>
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
	using _OutChar = typename _OutStr_::value_type;

	_OutStr_ __out_str;
	__out_str.reserve(__str.size());

	return _Char_traits<_InChar>::__cvt(__page, __out_str.data(), __str.data(), __str.size());
}

template <string_type _OutStr_>
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
			const _InStr_& __str, system::__code_page __page) noexcept 
		{
			return __dispatch_cvts<_OutStr_>(__str, __page);
		}

		using callable_type_tag = __impl;
	};
};

template <class _OutStr_> constexpr auto __str_cvt = fs::options::functor<typename _Configurable_str_cvt<_OutStr_>::__impl>;

__FILESYSTEM_NAMESPACE_END
