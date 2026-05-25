#pragma once 

#include <src/filesystem/options/Option.h>
#include <src/filesystem/options/Settings.h>
#include <src/filesystem/options/Merge.h>
#include <src/filesystem/options/Drop.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <concepts::settings _Settings_ = settings<>>
struct options: _Settings_ {
    constexpr filesystem_always_inline options() noexcept:
        _Settings_{} 
    {}

    template <concepts::option ... _Options_>
    constexpr filesystem_always_inline explicit options(_Options_ && ... opts) noexcept:
        _Settings_(std::forward<_Options_>(opts) ...)
    {}

    template <class ... _Options_>
    constexpr filesystem_always_inline explicit options(const settings<_Options_...>& opts) noexcept :
        _Settings_(opts)
    {}

    template <concepts::keyword _Keyword_>
    constexpr filesystem_always_inline auto drop(const _Keyword_& keyword) const noexcept {
        auto dropped = fs::options::drop(keyword, *this);
        return options<decltype(dropped)>{dropped};
    }

    template <concepts::keyword _Keyword0_, concepts::keyword ... _Keywords_>
    constexpr filesystem_always_inline auto drop(const _Keyword0_& kw0, const _Keywords_& ... kws) const noexcept {
        auto dropped = fs::options::drop(kw0, *this);
        return options<decltype(dropped)>{dropped}.drop(kws...);
    }

    template <concepts::keyword _Keywords_>
    constexpr filesystem_always_inline auto extract(const _Keywords_& kws) const noexcept {
        auto value = (*this)[kws];
        auto dropped = fs::options::drop(kws, *this);

        return std::tuple{value, options<decltype(dropped)>{dropped}};
    }
};

template <concepts::option ... _Options_>
options(_Options_&& ... opts) -> options<decltype(settings(std::forward<_Options_>(opts) ...))>;

template <class ... _Options_>
options(const settings<_Options_...>&) -> options<settings<_Options_...>>;

template <concepts::settings _S0_, concepts::settings _S1_>
constexpr static filesystem_always_inline auto merge_prefer_first(
    const options<_S0_>& base, const options<_S1_>& new_options) noexcept
{
    auto result_options = fs::options::merge(new_options, base);
    return options<decltype(result_options)>{result_options};
}

template <class _Type_>
concept callable_options = concepts::settings<_Type_>;

template <class _Type_>
concept callable_option = concepts::option<_Type_>;

template <auto _Option_> 
struct exact_option {
    constexpr filesystem_always_inline auto process(const auto& base, concepts::exactly<_Option_> auto const& opts) const noexcept {
        return fs::options::merge_prefer_first(base, options{ opts });
    }

    constexpr filesystem_always_inline auto default_to(auto const& base) const {
        return base;
    }
};

__FILESYSTEM_OPTIONS_NAMESPACE_END
