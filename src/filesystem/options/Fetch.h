#pragma once 

#include <src/filesystem/options/Concepts.h>
#include <src/filesystem/options/TypeOr.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <concepts::keyword _Keyword_, concepts::option ... _Options_>
constexpr filesystem_always_inline decltype(auto) fetch(const _Keyword_& keyword, const _Options_& ... opts) noexcept {
    const auto opts = settings(opts...);
    return opts[keyword];
}
    
template <concepts::keyword _Keyword_, class _Value_, concepts::option ... _Options_>
constexpr filesystem_always_inline decltype(auto) fetch(
    const type_or<_Keyword_, _Value_>& keyword, const _Options_& ... opts) noexcept
{
    const auto opts = settings(opts...);
    return opts[keyword];
}

template <class _Key_, concepts::settings _Settings_>
constexpr filesystem_always_inline decltype(auto) fetch(const _Key_& key, const _Settings_& settings_) noexcept {
    return settings_[key];
}

namespace detail {
    template <auto _Keyword_, class ... _Sources_>
    struct fetch_t;

    template <auto _Keyword_, concepts::option ... _Options_>
    struct fetch_t<_Keyword_, _Options_...> {
        using type = decltype(fetch(_Keyword_, std::declval<_Options_>()...));
    };

    template <auto _Keyword_, concepts::settings _Settings_>
    struct fetch_t<_Keyword_, _Settings_> {
        using type = decltype(fetch(_Keyword_, std::declval<_Settings_>()));
    };
} // namespace detail

template <auto _Keyword_, class ... _Sources_>
using fetch_t = typename detail::fetch_t<_Keyword_, _Sources_...>::type;

__FILESYSTEM_OPTIONS_NAMESPACE_END
