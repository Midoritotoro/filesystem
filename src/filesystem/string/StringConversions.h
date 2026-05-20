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
concept str_type = requires { 
	typename std::basic_string<typename _String_::value_type,
		typename _String_::traits_type, typename _String_::allocator_type>; };

template <class _String1_, class _String2_>
concept compatible_strings = str_type<_String1_> && str_type<_String2_> &&
	(std::is_same_v<typename _String1_::value_type, typename _String2_::value_type>) &&
	(std::is_same_v<typename _String1_::traits_type, typename _String2_::traits_type>) &&
	(std::is_same_v<typename _String1_::allocator_type, typename _String2_::allocator_type>);

template <str_type _OutStr_, std::input_iterator _It_>
filesystem_always_inline void __dispatch_cvts(_OutStr_& __out_str, _It_ __in_begin, _It_ __in_end, system::__code_page __page) noexcept {
	using _InChar = std::iter_value_t<_It_>;
	_Char_traits<_InChar>::__cvt(__page, __out_str.data(), std::to_address(__in_begin), std::distance(__in_begin, __in_end));
}

template <str_type _InStr_, str_type _OutStr_>
filesystem_always_inline void __dispatch_cvts(_OutStr_& __out_str, const _InStr_& __in_str, system::__code_page __page) noexcept {
	using _InChar = typename _InStr_::value_type;
	_Char_traits<_InChar>::__cvt(__page, __out_str.data(), __in_str.data(), __in_str.size());
}

template <class _Options_>
struct _Configurable_str_cvt : fs::options::strict_elementwise_callable<_Configurable_str_cvt, _Options_> {
	template <str_type _OutStr_, str_type _InStr_>
	filesystem_nodiscard filesystem_always_inline void operator()(_OutStr_& __out_str, const _InStr_& __in_str,
		system::__code_page __page = system::__current_thread_code_page()) const noexcept
	{
		return fs::options::__dispatch_call(*this, __out_str, __in_str, __page);
	}

	template <str_type _OutStr_, std::input_iterator _It_>
	filesystem_nodiscard filesystem_always_inline void operator()(_OutStr_& __out_str, _It_ __begin, _It_ __end,
		system::__code_page __page = system::__current_thread_code_page()) const noexcept
	{
		return fs::options::__dispatch_call(*this, __out_str, __begin, __end, __page);
	}

	template <str_type _OutStr_, str_type _InStr_>
	filesystem_nodiscard static filesystem_always_inline auto deferred_call(auto __opts, 
		_OutStr_& __out_str, const _InStr_& __in_str, system::__code_page __page) noexcept 
	{
		return __dispatch_cvts<_OutStr_>(__out_str, __in_str, __page);
	}
	
	template <str_type _OutStr_, std::input_iterator _It_>
	filesystem_nodiscard static filesystem_always_inline auto deferred_call(auto __opts,
		_OutStr_& __out_str, _It_ __in_begin, _It_ __in_end, system::__code_page __page) noexcept
	{
		return __dispatch_cvts<_OutStr_>(__out_str, __in_begin, __in_end, __page);
	}

	using callable_tag_type = _Configurable_str_cvt;
};

constexpr auto __str_cvt = fs::options::functor<_Configurable_str_cvt>;

__FILESYSTEM_NAMESPACE_END
