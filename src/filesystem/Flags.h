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

struct flags_zero_helper_struct {};
using flags_zero_helper = void(flags_zero_helper_struct::*)();

template <class _EnumType_>
class flags {
public:
	using enum_type = _EnumType_;
	using type = std::underlying_type_t<_EnumType_>;

	constexpr flags() = default;
	constexpr flags(flags_zero_helper) noexcept {}
	
	constexpr flags(enum_type v) noexcept: 
		_value(static_cast<type>(v))
	{}
	
	static constexpr flags from_raw(type v) noexcept {
		return flags(static_cast<enum_type>(v));
	}

	constexpr auto value() const noexcept {
		return _value;
	}

	constexpr operator type() const noexcept {
		return value();
	}

	constexpr auto& operator|=(flags x) noexcept {
		_value |= x.value();
		return *this;
	}

	constexpr auto& operator&=(flags x) noexcept {
		_value &= x.value();
		return *this;
	}

	constexpr auto& operator^=(flags x) noexcept {
		_value ^= x.value();
		return *this;
	}

	constexpr auto operator~() const noexcept {
		return from_raw(~value());
	}

	constexpr auto operator|(flags x) const noexcept {
		return (flags(*this) |= x);
	}

	constexpr auto operator&(flags x) const noexcept {
		return (flags(*this) &= x);
	}

	constexpr auto operator^(flags x) const noexcept {
		return (flags(*this) ^= x);
	}

	constexpr auto operator|(enum_type x) const noexcept {
		return (flags(*this) |= x);
	}

	constexpr auto operator&(enum_type x) const noexcept {
		return (flags(*this) &= x);
	}

	constexpr auto operator^(enum_type x) const noexcept {
		return (flags(*this) ^= x);
	}

	constexpr auto operator==(enum_type x) const noexcept {
		return (value() == static_cast<type>(x));
	}

	constexpr auto operator!=(enum_type x) const noexcept {
		return !(*this == x);
	}

	constexpr auto operator<(enum_type x) const noexcept {
		return value() < static_cast<type>(x);
	}

	constexpr auto operator>(enum_type x) const noexcept {
		return (x < *this);
	}

	constexpr auto operator<=(enum_type x) const noexcept {
		return !(x < *this);
	}

	constexpr auto operator>=(enum_type x) const noexcept {
		return !(*this < x);
	}

	constexpr inline bool test_flag(enum_type flag) const noexcept {
		return test_flags(flag);
	}

	constexpr inline bool test_flags(flags flags) const noexcept { 
		return flags._value ? ((_value & flags._value) == flags._value) : _value == 0;
	}

	constexpr inline bool test_any_flag(enum_type flag) const noexcept {
		return test_any_flags(flag); 
	}

	constexpr inline bool test_any_flags(flags flags) const noexcept {
		return (_value & flags._value) != 0;
	}

private:
	type _value = 0;
};

template <class _ExtendedEnum_, class _Enum_ = extended_flags_t<_ExtendedEnum_>>
inline constexpr auto extended_flag_convert(_ExtendedEnum_ v) noexcept {
	return static_cast<_Enum_>(v);
}

template <class _ExtendedEnum_, class _Enum_ = extended_flags_t<_ExtendedEnum_>>
inline constexpr auto extended_flags_convert(_ExtendedEnum_ v) noexcept {
	return flags<_Enum_>(extended_flag_convert(v));
}

template <class _Enum_>
constexpr auto make_flags(_Enum_ v) noexcept {
	return flags<_Enum_>(v);
}

template <class _Enum_>
inline constexpr auto operator|(_Enum_ x, flags<_Enum_> y) noexcept
	requires(std::is_enum<_Enum_>::value && is_flag_type(_Enum_{}))
{
	return x | y;
}

template <class _Enum_>
inline constexpr auto operator&(_Enum_ x, flags<_Enum_> y) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return x & y;
}

template <class _Enum_>
inline constexpr auto operator^(_Enum_ x, flags<_Enum_> y) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{}))
{
	return x ^ y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(flags<extended_flags_t<_ExtendedEnum_>> x, _ExtendedEnum_ y) noexcept {
	return x | extended_flags_convert(y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ x, flags<extended_flags_t<_ExtendedEnum_>> y) noexcept {
	return x | y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator&(flags<extended_flags_t<_ExtendedEnum_>> x, _ExtendedEnum_ y) noexcept {
	return x & extended_flags_convert(y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator&(_ExtendedEnum_ x, flags<extended_flags_t<_ExtendedEnum_>> y) noexcept {
	return x & y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator^(flags<extended_flags_t<_ExtendedEnum_>> x, _ExtendedEnum_ y) noexcept {
	return x ^ extended_flags_convert(y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator^(_ExtendedEnum_ x, flags<extended_flags_t<_ExtendedEnum_>> y) noexcept {
	return x ^ y;
}

template <class _ExtendedEnum_>
inline constexpr auto& operator&=(flags<extended_flags_t<_ExtendedEnum_>>& x, _ExtendedEnum_ y) noexcept {
	return (x &= extended_flags_convert(y));
}

template <class _ExtendedEnum_>
inline constexpr auto& operator|=(flags<extended_flags_t<_ExtendedEnum_>>& x, _ExtendedEnum_ y) noexcept {
	return (x |= extended_flags_convert(y));
}

template <class _ExtendedEnum_>
inline constexpr auto& operator^=(flags<extended_flags_t<_ExtendedEnum_>>& x, _ExtendedEnum_ y) noexcept {
	return (x ^= extended_flags_convert(y));
}

template <class _ExtendedEnum_>
inline constexpr auto operator==(flags<extended_flags_t<_ExtendedEnum_>> x, _ExtendedEnum_ y) noexcept {
	return x == extended_flags_convert(y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator==(_ExtendedEnum_ x, flags<extended_flags_t<_ExtendedEnum_>> y) noexcept {
	return (x == y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator!=(flags<extended_flags_t<_ExtendedEnum_>> x, _ExtendedEnum_ y) noexcept {
	return !(x == y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator!=(_ExtendedEnum_ x, flags<extended_flags_t<_ExtendedEnum_>> y) noexcept {
	return !(x == y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator<(flags<extended_flags_t<_ExtendedEnum_>> x, _ExtendedEnum_ y) noexcept {
	return x < extended_flags_convert(y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator<(_ExtendedEnum_ x, flags<extended_flags_t<_ExtendedEnum_>> y) noexcept {
	return extended_flags_convert(x) < y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator>(flags<extended_flags_t<_ExtendedEnum_>> x, _ExtendedEnum_ y) noexcept {
	return (y < x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator>(_ExtendedEnum_ x, flags<extended_flags_t<_ExtendedEnum_>> y) noexcept {
	return (y < x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator<=(flags<extended_flags_t<_ExtendedEnum_>> x, _ExtendedEnum_ y) noexcept {
	return !(y < x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator<=(_ExtendedEnum_ x, flags<extended_flags_t<_ExtendedEnum_>> y) noexcept {
	return !(y < x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator>=(flags<extended_flags_t<_ExtendedEnum_>> x, _ExtendedEnum_ y) noexcept {
	return !(x < y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator>=(_ExtendedEnum_ x, flags<extended_flags_t<_ExtendedEnum_>> y) noexcept {
	return !(x < y);
}

template <class _Enum_>
inline constexpr auto operator!(_Enum_ x) noexcept 
	requires(std::is_enum<_Enum_>::value && is_flag_type(_Enum_{})) 
{
	return !make_flags(x);
}

template <class _Enum_>	
inline constexpr auto operator~(_Enum_ x) noexcept
	requires(std::is_enum<_Enum_>::value && is_flag_type(_Enum_{})) 
{
	return ~make_flags(x);
}

template <class _Enum_>
inline constexpr auto operator|(_Enum_ x, _Enum_ y) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{}))
{
	return make_flags(x) | y;
}

template <class _Enum_>
inline constexpr auto operator|(_Enum_ x, flags_zero_helper) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return make_flags(x);
}

template <class _Enum_>
inline constexpr auto operator|(flags_zero_helper, _Enum_ y) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return make_flags(y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ x, _ExtendedEnum_ y) noexcept {
	return extended_flags_convert(x) | y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ x, extended_flags_t<_ExtendedEnum_> y) noexcept {
	return extended_flags_convert(x) | y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(extended_flags_t<_ExtendedEnum_> x, _ExtendedEnum_ y) noexcept {
	return x | y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(flags_zero_helper, _ExtendedEnum_ y) noexcept {
	return extended_flag_convert(y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ x, flags_zero_helper) noexcept {
	return extended_flag_convert(x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator~(_ExtendedEnum_ x) noexcept {
	return ~extended_flags_convert(x);
}

__FILESYSTEM_NAMESPACE_END

template <class _Enum_>
inline constexpr auto operator!(_Enum_ x) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return !fs::make_flags(x);
}

template <class _Enum_>
inline constexpr auto operator~(_Enum_ x) noexcept 
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return ~fs::make_flags(x);
}

template <class _Enum_>
inline constexpr auto operator|(_Enum_ x, _Enum_ y) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return fs::make_flags(x) | y;
}

template <class _Enum_>
inline constexpr auto operator|(_Enum_ x, fs::flags_zero_helper) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return fs::make_flags(x);
}

template <class _Enum_>
inline constexpr auto operator|(fs::flags_zero_helper, _Enum_ x) noexcept
	requires(std::is_enum<_Enum_>::value&& is_flag_type(_Enum_{})) 
{
	return fs::make_flags(x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ x, _ExtendedEnum_ y) noexcept {
	return fs::extended_flags_convert(x) | y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ x, fs::extended_flags_t<_ExtendedEnum_> y) noexcept {
	return fs::extended_flags_convert(x) | y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(fs::extended_flags_t<_ExtendedEnum_> x, _ExtendedEnum_ y) noexcept {
	return x | y;
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(fs::flags_zero_helper, _ExtendedEnum_ y) noexcept {
	return fs::extended_flag_convert(y);
}

template <class _ExtendedEnum_>
inline constexpr auto operator|(_ExtendedEnum_ x, fs::flags_zero_helper) noexcept {
	return fs::extended_flag_convert(x);
}

template <class _ExtendedEnum_>
inline constexpr auto operator~(_ExtendedEnum_ y) noexcept {
	return ~fs::extended_flags_convert(y);
}

using fs::flags;

#define FS_DECLARE_FLAGS(_Flags, _Enum) \
	using _Flags = flags<_Enum>; \
	inline constexpr bool is_flag_type(_Enum) { return true; }