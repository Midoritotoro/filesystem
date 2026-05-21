#pragma once

#include <type_traits>
#include <filesystem/compatibility/Compatibility.h>

__FILESYSTEM_NAMESPACE_BEGIN

template <class _Enum_>
class flags;

template <class _ExtendedEnum_>
struct extended_flags;

template <class _ExtendedEnum_>
using extended_flags_t = typename extended_flags<_ExtendedEnum_>::type;

struct __flags_zero_helper_struct {};
using __flags_zero_helper = void(__flags_zero_helper_struct::*)();

template <class _EnumType_>
class flags {
public:
	using enum_type = _EnumType_;
	using type = std::underlying_type_t<_EnumType_>;

	constexpr flags() = default;
	constexpr flags(__flags_zero_helper) noexcept {}
	
	constexpr flags(enum_type __v) noexcept: 
		_value(static_cast<type>(__v))
	{}
	
	static constexpr flags from_raw(type __v) noexcept {
		return flags(static_cast<enum_type>(__v));
	}

	constexpr auto value() const noexcept {
		return _value;
	}

	constexpr operator type() const noexcept {
		return value();
	}

	constexpr auto& operator|=(flags __x) noexcept {
		_value |= __x.value();
		return *this;
	}

	constexpr auto& operator&=(flags __x) noexcept {
		_value &= __x.value();
		return *this;
	}

	constexpr auto& operator^=(flags __x) noexcept {
		_value ^= __x.value();
		return *this;
	}

	constexpr auto operator~() const noexcept {
		return from_raw(~value());
	}

	constexpr auto operator|(flags __x) const noexcept {
		return (flags(*this) |= __x);
	}

	constexpr auto operator&(flags __x) const noexcept {
		return (flags(*this) &= __x);
	}

	constexpr auto operator^(flags __x) const noexcept {
		return (flags(*this) ^= __x);
	}

	constexpr auto operator|(enum_type __x) const noexcept {
		return (flags(*this) |= __x);
	}

	constexpr auto operator&(enum_type __x) const noexcept {
		return (flags(*this) &= __x);
	}

	constexpr auto operator^(enum_type __x) const noexcept {
		return (flags(*this) ^= __x);
	}

	constexpr auto operator==(enum_type __x) const noexcept {
		return (value() == static_cast<type>(__x));
	}

	constexpr auto operator!=(enum_type __x) const noexcept {
		return !(*this == __x);
	}

	constexpr auto operator<(enum_type __x) const noexcept {
		return value() < static_cast<type>(__x);
	}

	constexpr auto operator>(enum_type __x) const noexcept {
		return (__x < *this);
	}

	constexpr auto operator<=(enum_type __x) const noexcept {
		return !(__x < *this);
	}

	constexpr auto operator>=(enum_type __x) const noexcept {
		return !(*this < __x);
	}

	constexpr inline bool test_flag(enum_type __flag) const noexcept {
		return test_flags(__flag);
	}

	constexpr inline bool test_flags(flags __flags) const noexcept { 
		return __flags._value ? ((_value & __flags._value) == __flags._value) : _value == 0;
	}

	constexpr inline bool test_any_flag(enum_type __flag) const noexcept {
		return test_any_flags(__flag); 
	}

	constexpr inline bool test_any_flags(flags __flags) const noexcept {
		return (_value & __flags._value) != 0;
	}

private:
	type _value = 0;
};

template <class _ExtendedEnum_, class _Enum_ = extended_flags_t<_ExtendedEnum_>>
inline constexpr auto extended_flag_convert(_ExtendedEnum_ __v) noexcept {
	return static_cast<_Enum_>(__v);
}

template <class _ExtendedEnum_ class _Enum_ = typename extended_flags<_ExtendedEnum_>::type>
inline constexpr auto extended_flags_convert(_ExtendedEnum_ __v) noexcept {
	return flags<_Enum_>(extended_flag_convert(__v));
}

template <class _Enum_>
constexpr auto make_flags(_Enum_ __v) noexcept {
	return flags<_Enum_>(__v);
}

template <class _Enum_>
inline constexpr auto operator|(_Enum_ __x, flags<_Enum_> __y) noexcept
	requires(std::is_enum<_Enum_>::value && is_flag_type(_Enum_{}))
{
	return __x | __y;
}

template <class _Enum_>
inline constexpr auto operator&(_Enum_ __x, flags<_Enum_> __y) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return __x & __y;
}

template <class _Enum_>
inline constexpr auto operator^(_Enum_ __x, flags<_Enum_> __y) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{}))
{
	return __x ^ __y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(flags<extended_flags_t<_ExtendedEnum_>> __x, _ExtendedEnum_ __y) noexcept {
	return __x | extended_flags_convert(__y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ __x, flags<extended_flags_t<_ExtendedEnum_>> __y) noexcept {
	return __x | __y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator&(flags<extended_flags_t<_ExtendedEnum_>> __x, _ExtendedEnum_ __y) noexcept {
	return __x & extended_flags_convert(__y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator&(_ExtendedEnum_ __x, flags<extended_flags_t<_ExtendedEnum_>> __y) noexcept {
	return __x & __y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator^(flags<extended_flags_t<_ExtendedEnum_>> __x, _ExtendedEnum_ __y) noexcept {
	return __x ^ extended_flags_convert(__y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator^(_ExtendedEnum_ __x, flags<extended_flags_t<_ExtendedEnum_>> __y) noexcept {
	return __x ^ __y;
}

template <class _ExtendedEnum_>
inline constexpr auto& operator&=(flags<extended_flags_t<_ExtendedEnum_>>& __x, _ExtendedEnum_ __y) noexcept {
	return (__x &= extended_flags_convert(__y));
}

template <class _ExtendedEnum_>
inline constexpr auto& operator|=(flags<extended_flags_t<_ExtendedEnum_>>& __x, _ExtendedEnum_ __y) noexcept {
	return (__x |= extended_flags_convert(__y));
}

template <class _ExtendedEnum_>
inline constexpr auto& operator^=(flags<extended_flags_t<_ExtendedEnum_>>& __x, _ExtendedEnum_ __y) noexcept {
	return (__x ^= extended_flags_convert(__y));
}

template <class _ExtendedEnum_>
inline constexpr auto operator==(flags<extended_flags_t<_ExtendedEnum_>> __x, _ExtendedEnum_ __y) noexcept {
	return __x == extended_flags_convert(__y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator==(_ExtendedEnum_ __x, flags<extended_flags_t<_ExtendedEnum_>> __y) noexcept {
	return (__x == __y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator!=(flags<extended_flags_t<_ExtendedEnum_>> __x, _ExtendedEnum_ __y) noexcept {
	return !(__x == __y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator!=(_ExtendedEnum_ __x, flags<extended_flags_t<_ExtendedEnum_>> __y) noexcept {
	return !(__x == __y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator<(flags<extended_flags_t<_ExtendedEnum_>> __x, _ExtendedEnum_ __y) noexcept {
	return __x < extended_flags_convert(__y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator<(_ExtendedEnum_ __x, flags<extended_flags_t<_ExtendedEnum_>> __y) noexcept {
	return extended_flags_convert(__x) < __y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator>(flags<extended_flags_t<_ExtendedEnum_>> __x, _ExtendedEnum_ __y) noexcept {
	return (__y < __x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator>(_ExtendedEnum_ __x, flags<extended_flags_t<_ExtendedEnum_>> __y) noexcept {
	return (__y < __x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator<=(flags<extended_flags_t<_ExtendedEnum_>> __x, _ExtendedEnum_ __y) noexcept {
	return !(__y < __x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator<=(_ExtendedEnum_ __x, flags<extended_flags_t<_ExtendedEnum_>> __y) noexcept {
	return !(__y < __x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator>=(flags<extended_flags_t<_ExtendedEnum_>> __x, _ExtendedEnum_ __y) noexcept {
	return !(__x < __y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator>=(_ExtendedEnum_ __x, flags<extended_flags_t<_ExtendedEnum_>> __y) noexcept {
	return !(__x < __y);
}

template <class _Enum_>
inline constexpr auto operator!(_Enum_ __x) noexcept 
	requires(std::is_enum<_Enum_>::value && is_flag_type(_Enum_{})) 
{
	return !make_flags(__x);
}

template <class _Enum_>	
inline constexpr auto operator~(_Enum_ __x) noexcept
	requires(std::is_enum<_Enum_>::value && is_flag_type(_Enum_{})) 
{
	return ~make_flags(__x);
}

template <class _Enum_>
inline constexpr auto operator|(_Enum_ __x, _Enum_ __y) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{}))
{
	return make_flags(__x) | __y;
}

template <class _Enum_>
inline constexpr auto operator|(_Enum_ __x, __flags_zero_helper) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return make_flags(__x);
}

template <class _Enum_>
inline constexpr auto operator|(__flags_zero_helper, _Enum_ __y) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return make_flags(__y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ __x, _ExtendedEnum_ __y) noexcept {
	return extended_flags_convert(__x) | __y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ __x, extended_flags_t<_ExtendedEnum_> __y) noexcept {
	return extended_flags_convert(__x) | __y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(extended_flags_t<_ExtendedEnum_> __x, _ExtendedEnum_ __y) noexcept {
	return __x | __y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(__flags_zero_helper, _ExtendedEnum_ __y) noexcept {
	return extended_flag_convert(__y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ __x, __flags_zero_helper) noexcept {
	return extended_flag_convert(__x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator~(_ExtendedEnum_ __x) noexcept {
	return ~extended_flags_convert(__y);
}

__FILESYSTEM_NAMESPACE_END

template <class _Enum_>
inline constexpr auto operator!(_Enum_ __x) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return !fs::make_flags(__x);
}

template <class _Enum_>
inline constexpr auto operator~(_Enum_ __x) noexcept 
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return ~fs::make_flags(__x);
}

template <class _Enum_>
inline constexpr auto operator|(_Enum_ __x, _Enum_ __y) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return fs::make_flags(__x) | __y;
}

template <class _Enum_>
inline constexpr auto operator|(_Enum_ __x, fs::__flags_zero_helper) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return fs::make_flags(__x);
}

template <class _Enum_>
inline constexpr auto operator|(fs::__flags_zero_helper, _Enum_ __x) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return base::make_flags(__x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ __x, _ExtendedEnum_ __y) noexcept {
	return fs::extended_flags_convert(__x) | __y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ __x, fs::extended_flags_t<_ExtendedEnum_> __y) noexcept {
	return fs::extended_flags_convert(__x) | __y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(fs::extended_flags_t<_ExtendedEnum_> __x, _ExtendedEnum_ __y) noexcept {
	return __x | __y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(fs::__flags_zero_helper, _ExtendedEnum_ __y) noexcept {
	return fs::extended_flag_convert(__y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ __x, fs::__flags_zero_helper) noexcept {
	return fs::extended_flag_convert(__x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator~(_ExtendedEnum_ __y) noexcept {
	return ~fs::extended_flags_convert(__y);
}

using fs::flags;

#define FS_DECLARE_FLAGS(_Flags, _Enum) \
	using _Flags = flags<_Enum>; \
	inline constexpr bool is_flag_type(_Enum) { return true; }