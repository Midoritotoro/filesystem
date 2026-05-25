#pragma once 

#include <src/filesystem/options/Aggregator.h>
#include <src/filesystem/options/Keywords.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <concepts::option ... _Options_> 
struct settings {
    using settings_type = void;
    using base = aggregator<_Options_...>;

    constexpr settings(const _Options_& ... options) noexcept: 
        _content(options...) 
    {}

    static filesystem_always_inline constexpr std::ptrdiff_t size() noexcept {
        return sizeof...(_Options_); 
    }

    template <concepts::keyword _Key_>
    static filesystem_always_inline constexpr auto contains(const _Key_& keyword) noexcept {
        using found = decltype((std::declval<base>())(_Key_{}));
        return !concepts::same_as<found, unknown_key>;
    }
        
    template <concepts::keyword ... _Keys_>
    static filesystem_always_inline constexpr auto contains_any(_Keys_ ... keywords) noexcept {
        return (contains(keywords) || ...);
    }

    template <concepts::keyword ... _Keys_>
    static filesystem_always_inline constexpr auto contains_only(const _Keys_& ...) noexcept {
        using current_keys = keys<typename _Options_::keyword_type...>;
        using acceptable_keys = keys<_Keys_...>;
        using unique_set = typename uniques_<current_keys, acceptable_keys>::type;
        return is_equivalent<unique_set, acceptable_keys>::value;
    }

    template <concepts::keyword... _Keys_>
    static filesystem_always_inline constexpr auto contains_none(_Keys_ ... keywords) noexcept {
        return !contains_any(keywords...);
    }

    template <concepts::keyword _Key_> 
    constexpr filesystem_always_inline auto operator[](const _Key_& keyword) const noexcept {
        return _content(keyword);
    }

    template <concepts::keyword ... _Keys_> 
    constexpr filesystem_always_inline auto operator[](const _Keys_& ... keywords) const noexcept {
        return (_content(keywords), ...);
    }

    template <class _Keyword_>
    constexpr filesystem_always_inline auto operator[](const flag_keyword<_Keyword_>&) const noexcept {
        return contains(flag_keyword<_Keyword_>{});
    }

    template <concepts::keyword _Key_, class _Value_>
    constexpr filesystem_always_inline auto operator[](type_or<_Key_, _Value_> value) const {
        if constexpr(contains(_Key_{})) return (*this)[_Key_{}];
        else if constexpr(requires(_Value_ t) { t.perform(); }) return value.value.perform();
        else return value.value;
    }
        
    base _content;
};

template <concepts::option ... _Options_>
settings(const _Options_& ... options) -> settings<_Options_...>;

__FILESYSTEM_OPTIONS_NAMESPACE_END
