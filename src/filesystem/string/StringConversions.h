#pragma once 

#include <src/filesystem/Options.h>
#include <src/filesystem/system/IoError.h>
#include <src/filesystem/string/CharTraits.h>
#include <string>
#include <ranges>
#include <algorithm>

__FILESYSTEM_NAMESPACE_BEGIN

#if defined(__cpp_char8_t)
  constexpr auto has_utf8 = false;
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
filesystem_always_inline void dispatch_cvts(_OutStr_& out_str, _It_ in_begin, _It_ in_end, system::code_page page) noexcept {
	using _InChar = std::iter_value_t<_It_>;
	char_traits<_InChar>::cvt(page, out_str.data(), std::to_address(in_begin), std::distance(in_begin, in_end));
}

template <str_type _InStr_, str_type _OutStr_>
filesystem_always_inline void dispatch_cvts(_OutStr_& out_str, const _InStr_& in_str, system::code_page page) noexcept {
	using _InChar = typename _InStr_::value_type;
	char_traits<_InChar>::cvt(page, out_str.data(), in_str.data(), in_str.size());
}

template <class _Options_>
struct configurable_str_cvt_t: fs::options::strict_elementwise_callable<configurable_str_cvt_t, _Options_> {
	template <str_type _OutStr_, str_type _InStr_>
	filesystem_nodiscard filesystem_always_inline void operator()(_OutStr_& out_str, const _InStr_& in_str,
		system::code_page page = system::current_thread_code_page()) const noexcept
	{
		return fs::options::dispatch_call(*this, out_str, in_str, page);
	}

	template <str_type _OutStr_, std::input_iterator _It_>
	filesystem_nodiscard filesystem_always_inline void operator()(_OutStr_& out_str, _It_ begin, _It_ end,
		system::code_page page = system::current_thread_code_page()) const noexcept
	{
		return fs::options::dispatch_call(*this, out_str, begin, end, page);
	}

	template <str_type _OutStr_, str_type _InStr_>
	filesystem_nodiscard static filesystem_always_inline auto deferred_call(auto opts, 
		_OutStr_& out_str, const _InStr_& in_str, system::code_page page) noexcept 
	{
		return dispatch_cvts<_OutStr_>(out_str, in_str, page);
	}
	
	template <str_type _OutStr_, std::input_iterator _It_>
	filesystem_nodiscard static filesystem_always_inline auto deferred_call(auto opts,
		_OutStr_& out_str, _It_ in_begin, _It_ in_end, system::code_page page) noexcept
	{
		return dispatch_cvts<_OutStr_>(out_str, in_begin, in_end, page);
	}

	using callable_tag_type = configurable_str_cvt_t;
};

constexpr auto str_cvt = fs::options::functor<configurable_str_cvt_t>;

__FILESYSTEM_NAMESPACE_END
