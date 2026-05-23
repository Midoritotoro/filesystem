#pragma once

#include <type_traits>
#include <filesystem/Types.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

struct accumulate_decorations {
    filesystem_always_inline constexpr auto operator()(auto __accumulator, const auto& __m) const {
        return __m.default_to(__accumulator); 
    }
};

template <class _Function_, class _Type_> 
struct foldable {
    _Function_ _function;
    _Type_ _value;

    template <class _OtherType_>
    friend constexpr filesystem_always_inline auto operator>>(
        foldable&& __x, foldable<_Function_, _OtherType_>&& __y) noexcept
    {
        using _ReturnType = decltype(std::invoke(__x._function, __x._value, __y._value));
        return foldable<_Function_, std::decay_t<_ReturnType>>{__x._function,
            std::invoke(__x._function, __x._value, __y._value)};
    }

    template <class _OtherType_>
    friend constexpr filesystem_always_inline decltype(auto) operator<<(
        foldable&& __x, foldable<_Function_, _OtherType_>&& __y) noexcept 
    {
        using _ReturnType = decltype(std::invoke(__x._function, __x._value, __y._value));
        return foldable<_Function_, std::decay_t<_ReturnType>>{__x._function,
            std::invoke(__x._function, __x._value, __y._value)};
    }
};

template <class _Function_, class _Type_>
foldable(const _Function_&, _Type_&&) -> foldable<_Function_, _Type_>;

template <class _Function_, class _Type_, class _Value_>
constexpr filesystem_always_inline auto fold_left(_Function_&& __f, _Type_&& __t, _Value_ __init) {
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
        return (foldable{ __f, __init } >> ... >> foldable{
            __f, std::get<I>(std::forward<_Type_>(__t))})._value;
    }(std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<_Type_>>>());
}

__FILESYSTEM_OPTIONS_NAMESPACE_END
