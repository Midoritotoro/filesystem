#pragma once

#include <filesystem/compatibility/Compatibility.h>


__FILESYSTEM_NAMESPACE_BEGIN

template <class _Type_>
constexpr inline bool is_nonbool_integral_v = std::is_integral_v<_Type_> && !std::is_same_v<std::remove_cv_t<_Type_>, bool>;

template <class _Type_> struct is_character: std::false_type {};
template <> struct is_character<char> : std::true_type {};
template <> struct is_character<wchar_t> : std::true_type {};
template <> struct is_character<char8_t> : std::true_type {};
template <> struct is_character<char16_t> : std::true_type {};
template <> struct is_character<char32_t> : std::true_type {};

template <class _Type_>
constexpr inline bool is_character_v = is_character<_Type_>::value;

template <class _Type_>
concept character_type = is_character_v<_Type_>;

__FILESYSTEM_NAMESPACE_END
