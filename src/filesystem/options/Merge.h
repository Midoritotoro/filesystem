#pragma once 

#include <src/filesystem/options/Concepts.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <concepts::option ... _Options_>
struct settings;

template <concepts::option ... _Keys1_, concepts::option ... _Keys2_>
constexpr filesystem_always_inline auto merge(
    const settings<_Keys1_...>& opts1, 
    const settings<_Keys2_...>& definition) noexcept
{
    auto selector = [] <class _Key_, class _Options_> (const _Key_&, const _Options_& opts, const auto& d) {
        constexpr _Key_ key;
        if constexpr(_Options_::contains(key)) return (key = opts[key]);
        else return (key = d[key]);
    };

    auto select = [&] <class ... _Keys_> (const keys<_Keys_...>&, const auto& os, const auto& ds) {
        return settings(selector(_Keys_{}, os, ds)...);
    };

    return select(typename uniques_<keys<typename _Keys1_::keyword_type...>,
        keys<typename _Keys2_::keyword_type...>>::type{}, opts1, definition);
}

__FILESYSTEM_OPTIONS_NAMESPACE_END
