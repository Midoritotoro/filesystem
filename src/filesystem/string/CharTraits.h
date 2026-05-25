#pragma once 

#include <src/filesystem/IsCharacter.h>
#include <src/filesystem/system/CodePages.h>
#include <src/filesystem/Assert.h>
#include <type_traits>
#include <string>
#include <string_view>


__FILESYSTEM_NAMESPACE_BEGIN

template <character_type _Type_>
struct char_traits {};

inline i32 cvt_wide_to_multi_byte(system::code_page page, dword_t dw_flags,
	LPCWCH wide_char_str, i32 count_wide_chars, LPSTR multi_byte_str,
	i32 byte_size, LPCCH default_char, LPBOOL default_char_usage) noexcept
{
	return WideCharToMultiByte(static_cast<u32>(page), dw_flags, wide_char_str,
		count_wide_chars, multi_byte_str, byte_size, default_char, default_char_usage);
}

inline i32 cvt_multi_byte_to_wide(system::code_page page, dword_t dw_flags, LPCCH multi_byte_str,
	i32 byte_size, LPWSTR wide_char_str, i32 count_wide_chars) noexcept
{
	return MultiByteToWideChar(static_cast<u32>(page), dw_flags, multi_byte_str,
		byte_size, wide_char_str, count_wide_chars);
}

inline i32 cvt_utf32_to_utf16(std::u16string_view out_str, std::u32string_view in_str) noexcept {
	return 0;
}

inline i32 cvt_utf16_to_utf32(std::u16string_view out_str, std::u32string_view in_str) noexcept {
	return 0;
}


template <>
struct char_traits<char> {
	using value_type = char;

	filesystem_nodiscard static filesystem_always_inline bool is_low_surrogate(value_type) noexcept { return false; }
	filesystem_nodiscard static filesystem_always_inline bool is_high_surrogate(value_type) noexcept { return false; }
	filesystem_nodiscard static filesystem_always_inline bool is_surrogate(value_type) noexcept { return false; }

	template <character_type _ToChar_>
	filesystem_nodiscard static filesystem_always_inline void cvt(
		system::code_page page, _ToChar_* out_ch, value_type* in_ch, u64 len) noexcept 
	{
		std::copy(in_ch, in_ch + len, out_ch);
	}
};

template <>
struct char_traits<wchar_t> {
	using value_type = wchar_t;

	filesystem_nodiscard static filesystem_always_inline bool is_low_surrogate(value_type ch) noexcept {
		return (ch >= 0xDC00 && ch <= 0xDFFF);
	}

	filesystem_nodiscard static filesystem_always_inline bool is_high_surrogate(value_type ch) noexcept {
		return (ch >= 0xD800 && ch <= 0xDBFF);
	}

	filesystem_nodiscard static filesystem_always_inline bool is_surrogate(value_type ch) noexcept {
		return (ch >= 0xD800 && ch <= 0xDFFF);
	}

	template <character_type _ToChar_>
	filesystem_nodiscard static filesystem_always_inline void cvt(
		system::code_page page, _ToChar_* out_ch, const value_type* in_ch, u64 len) noexcept
	{
		using _RawToType = std::remove_cvref_t<_ToChar_>;
		
		if constexpr (std::is_same_v<_RawToType, char>) {
			const auto written = cvt_wide_to_multi_byte(page, (page == system::code_page::utf8) ? WC_ERR_INVALID_CHARS : WC_NO_BEST_FIT_CHARS,
				reinterpret_cast<LPCWCH>(in_ch), len, reinterpret_cast<LPSTR>(out_ch), len << 1, NULL, NULL);

			filesystem_debug_assert(written == len);
		}
		else if constexpr (std::is_same_v<_RawToType, char8_t>) {
			const auto written = cvt_wide_to_multi_byte(system::code_page::utf8, WC_ERR_INVALID_CHARS,
				reinterpret_cast<LPCWCH>(in_ch), len, reinterpret_cast<LPSTR>(out_ch), len << 1, NULL, NULL);

			filesystem_debug_assert(written == len);
		}
		else if constexpr (std::is_same_v<_RawToType, value_type> || std::is_same_v<_RawToType, char16_t>) {
			std::copy(in_ch, in_ch + len, out_ch);
		}
		else if constexpr (std::is_same_v<_RawToType, char32_t>) {
			std::copy(in_ch, in_ch + len, out_ch);
		}
	}
};

template <>
struct char_traits<char8_t> {
	using value_type = char8_t;

	filesystem_nodiscard static filesystem_always_inline bool is_low_surrogate(value_type ch) noexcept {
		return false;
	}

	filesystem_nodiscard static filesystem_always_inline bool is_high_surrogate(value_type ch) noexcept {
		return false;
	}

	filesystem_nodiscard static filesystem_always_inline bool is_surrogate(value_type ch) noexcept {
		return false;
	}

	template <character_type _ToChar_>
	filesystem_nodiscard static filesystem_always_inline void cvt(
		system::code_page page, _ToChar_* out_ch, const value_type* in_ch, u64 len) noexcept
	{
		using _RawToType = std::remove_cvref_t<_ToChar_>;

		if constexpr (std::is_same_v<_RawToType, char>) {
			for (u64 i = 0; i < len; ++i)
				*out_ch++ = *in_ch++ >= 0x80 ? *in_ch : '?';
		}
		else {
			std::copy(in_ch, in_ch + len, out_ch);
		}
	}
};

template <>
struct char_traits<char16_t> {
	using value_type = wchar_t;

	filesystem_nodiscard static filesystem_always_inline bool is_low_surrogate(value_type ch) noexcept {
		return (ch >= 0xDC00 && ch <= 0xDFFF);
	}

	filesystem_nodiscard static filesystem_always_inline bool is_high_surrogate(value_type ch) noexcept {
		return (ch >= 0xD800 && ch <= 0xDBFF);
	}

	filesystem_nodiscard static filesystem_always_inline bool is_surrogate(value_type ch) noexcept {
		return (ch >= 0xD800 && ch <= 0xDFFF);
	}

	template <character_type _ToChar_>
	filesystem_nodiscard static filesystem_always_inline void cvt(
		system::code_page page, _ToChar_* out_ch, const char16_t* in_ch, u64 len) noexcept
	{

	}
};

template <>
struct char_traits<char32_t> {
	using value_type = wchar_t;

	filesystem_nodiscard static filesystem_always_inline bool is_low_surrogate(value_type ch) noexcept {
		return (ch >= 0xDC00 && ch <= 0xDFFF);
	}

	filesystem_nodiscard static filesystem_always_inline bool is_high_surrogate(value_type ch) noexcept {
		return (ch >= 0xD800 && ch <= 0xDBFF);
	}

	filesystem_nodiscard static filesystem_always_inline bool is_surrogate(value_type ch) noexcept {
		return (ch >= 0xD800 && ch <= 0xDFFF);
	}

	template <character_type _ToChar_>
	filesystem_nodiscard static filesystem_always_inline void cvt(
		system::code_page page, _ToChar_* out_ch, const char32_t* in_ch, u64 len) noexcept
	{

	}
};

__FILESYSTEM_NAMESPACE_END
