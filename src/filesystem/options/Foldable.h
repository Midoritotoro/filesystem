#pragma once

#include <type_traits>
#include <filesystem/Types.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

struct accumulate_decorations {
    filesystem_always_inline constexpr auto operator()(auto accumulator, const auto& m) const {
        return m.default_to(accumulator); 
    }
};

template <class _Function_, class _Type_> 
struct foldable {
    _Function_ _function;
    _Type_ _value;

    template <class _OtherType_>
    friend constexpr filesystem_always_inline auto operator>>(
        foldable&& x, foldable<_Function_, _OtherType_>&& y) noexcept
    {
        using _ReturnType = decltype(std::invoke(x._function, x._value, y._value));
        return foldable<_Function_, std::decay_t<_ReturnType>>{x._function,
            std::invoke(x._function, x._value, y._value)};
    }

    template <class _OtherType_>
    friend constexpr filesystem_always_inline decltype(auto) operator<<(
        foldable&& x, foldable<_Function_, _OtherType_>&& y) noexcept 
    {
        using _ReturnType = decltype(std::invoke(x._function, x._value, y._value));
        return foldable<_Function_, std::decay_t<_ReturnType>>{x._function,
            std::invoke(x._function, x._value, y._value)};
    }
};

template <class _Function_, class _Type_>
foldable(const _Function_&, _Type_&&) -> foldable<_Function_, _Type_>;

template <class _Function_, class _Type_, class _Value_>
constexpr filesystem_always_inline auto fold_left(_Function_&& f, _Type_&& t, _Value_ init) {
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
        return (foldable{ f, init } >> ... >> foldable{
            f, std::get<I>(std::forward<_Type_>(t))})._value;
    }(std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<_Type_>>>());
}

__FILESYSTEM_OPTIONS_NAMESPACE_END
