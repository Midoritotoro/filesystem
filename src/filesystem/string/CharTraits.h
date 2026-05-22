#pragma once 

#include <src/filesystem/IsCharacter.h>
#include <src/filesystem/system/CodePages.h>
#include <src/filesystem/Assert.h>
#include <type_traits>
#include <string>
#include <string_view>


__FILESYSTEM_NAMESPACE_BEGIN

template <character_type _Type_>
struct _Char_traits {};

inline i32 __cvt_wide_to_multi_byte(system::__code_page __page, dword_t __dw_flags,
	LPCWCH __wide_char_str, i32 __count_wide_chars, LPSTR __multi_byte_str,
	i32 __byte_size, LPCCH __default_char, LPBOOL __default_char_usage) noexcept
{
	return WideCharToMultiByte(static_cast<u32>(__page), __dw_flags, __wide_char_str,
		__count_wide_chars, __multi_byte_str, __byte_size, __default_char, __default_char_usage);
}

inline i32 __cvt_multi_byte_to_wide(system::__code_page __page, dword_t __dw_flags, LPCCH __multi_byte_str,
	i32 __byte_size, LPWSTR __wide_char_str, i32 __count_wide_chars) noexcept
{
	return MultiByteToWideChar(static_cast<u32>(__page), __dw_flags, __multi_byte_str,
		__byte_size, __wide_char_str, __count_wide_chars);
}

inline i32 __cvt_utf32_to_utf16(std::u16string_view __out_str, std::u32string_view __in_str) noexcept {
	return 0;
}

inline i32 __cvt_utf16_to_utf32(std::u16string_view __out_str, std::u32string_view __in_str) noexcept {
	return 0;
}


template <>
struct _Char_traits<char> {
	using value_type = char;

	filesystem_nodiscard static filesystem_always_inline bool is_low_surrogate(value_type) noexcept { return false; }
	filesystem_nodiscard static filesystem_always_inline bool is_high_surrogate(value_type) noexcept { return false; }
	filesystem_nodiscard static filesystem_always_inline bool is_surrogate(value_type) noexcept { return false; }

	template <character_type _ToChar_>
	filesystem_nodiscard static filesystem_always_inline void __cvt(
		system::__code_page __page, _ToChar_* __out_ch, value_type* __in_ch, u64 __len) noexcept 
	{
		std::copy(__in_ch, __in_ch + __len, __out_ch);
	}
};

template <>
struct _Char_traits<wchar_t> {
	using value_type = wchar_t;

	filesystem_nodiscard static filesystem_always_inline bool is_low_surrogate(value_type __ch) noexcept {
		return (__ch >= 0xDC00 && __ch <= 0xDFFF);
	}

	filesystem_nodiscard static filesystem_always_inline bool is_high_surrogate(value_type __ch) noexcept {
		return (__ch >= 0xD800 && __ch <= 0xDBFF);
	}

	filesystem_nodiscard static filesystem_always_inline bool is_surrogate(value_type __ch) noexcept {
		return (__ch >= 0xD800 && __ch <= 0xDFFF);
	}

	template <character_type _ToChar_>
	filesystem_nodiscard static filesystem_always_inline void __cvt(
		system::__code_page __page, _ToChar_* __out_ch, const value_type* __in_ch, u64 __len) noexcept
	{
		using _RawToType = std::remove_cvref_t<_ToChar_>;
		
		if constexpr (std::is_same_v<_RawToType, char>) {
			const auto __written = __cvt_wide_to_multi_byte(__page, (__page == system::__code_page::__utf8) ? WC_ERR_INVALID_CHARS : WC_NO_BEST_FIT_CHARS,
				reinterpret_cast<LPCWCH>(__in_ch), __len, reinterpret_cast<LPSTR>(__out_ch), __len << 1, NULL, NULL);

			filesystem_debug_assert(__written == __len);
		}
		else if constexpr (std::is_same_v<_RawToType, char8_t>) {
			const auto __written = __cvt_wide_to_multi_byte(system::__code_page::__utf8, WC_ERR_INVALID_CHARS,
				reinterpret_cast<LPCWCH>(__in_ch), __len, reinterpret_cast<LPSTR>(__out_ch), __len << 1, NULL, NULL);

			filesystem_debug_assert(__written == __len);
		}
		else if constexpr (std::is_same_v<_RawToType, value_type> || std::is_same_v<_RawToType, char16_t>) {
			std::copy(__in_ch, __in_ch + __len, __out_ch);
		}
		else if constexpr (std::is_same_v<_RawToType, char32_t>) {
			std::copy(__in_ch, __in_ch + __len, __out_ch);
		}
	}
};

template <>
struct _Char_traits<char8_t> {
	using value_type = char8_t;

	filesystem_nodiscard static filesystem_always_inline bool is_low_surrogate(value_type __ch) noexcept {
		return false;
	}

	filesystem_nodiscard static filesystem_always_inline bool is_high_surrogate(value_type __ch) noexcept {
		return false;
	}

	filesystem_nodiscard static filesystem_always_inline bool is_surrogate(value_type __ch) noexcept {
		return false;
	}

	template <character_type _ToChar_>
	filesystem_nodiscard static filesystem_always_inline void __cvt(
		system::__code_page __page, _ToChar_* __out_ch, const value_type* __in_ch, u64 __len) noexcept
	{
		using _RawToType = std::remove_cvref_t<_ToChar_>;

		if constexpr (std::is_same_v<_RawToType, char>) {
			for (u64 __i = 0; __i < __len; ++__i)
				*__out_ch++ = *__in_ch++ >= 0x80 ? *__in_ch : '?';
		}
		else {
			std::copy(__in_ch, __in_ch + __len, __out_ch);
		}
	}
};

template <>
struct _Char_traits<char16_t> {
	using value_type = wchar_t;

	filesystem_nodiscard static filesystem_always_inline bool is_low_surrogate(value_type __ch) noexcept {
		return (__ch >= 0xDC00 && __ch <= 0xDFFF);
	}

	filesystem_nodiscard static filesystem_always_inline bool is_high_surrogate(value_type __ch) noexcept {
		return (__ch >= 0xD800 && __ch <= 0xDBFF);
	}

	filesystem_nodiscard static filesystem_always_inline bool is_surrogate(value_type __ch) noexcept {
		return (__ch >= 0xD800 && __ch <= 0xDFFF);
	}

	template <character_type _ToChar_>
	filesystem_nodiscard static filesystem_always_inline void __cvt(
		system::__code_page __page, _ToChar_* __out_ch, const char16_t* __in_ch, u64 __len) noexcept
	{

	}
};

template <>
struct _Char_traits<char32_t> {
	using value_type = wchar_t;

	filesystem_nodiscard static filesystem_always_inline bool is_low_surrogate(value_type __ch) noexcept {
		return (__ch >= 0xDC00 && __ch <= 0xDFFF);
	}

	filesystem_nodiscard static filesystem_always_inline bool is_high_surrogate(value_type __ch) noexcept {
		return (__ch >= 0xD800 && __ch <= 0xDBFF);
	}

	filesystem_nodiscard static filesystem_always_inline bool is_surrogate(value_type __ch) noexcept {
		return (__ch >= 0xD800 && __ch <= 0xDFFF);
	}

	template <character_type _ToChar_>
	filesystem_nodiscard static filesystem_always_inline void __cvt(
		system::__code_page __page, _ToChar_* __out_ch, const char32_t* __in_ch, u64 __len) noexcept
	{

	}
};

__FILESYSTEM_NAMESPACE_END
