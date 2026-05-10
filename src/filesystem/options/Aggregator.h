#pragma once 

#include <src/filesystem/options/Concepts.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

struct unknown_key { 
    using type = unknown_key; 
};

template <concepts::option ... _Options_> 
struct aggregator:
    _Options_...
{
    constexpr aggregator(const _Options_& ... __options) noexcept:
        _Options_(__options)... 
    {}

    using _Options_::operator()...;

    template <concepts::keyword _Keyword_> 
    constexpr filesystem_always_inline auto operator()(const _Keyword_&) const noexcept {
        return unknown_key{};
    }
};


__FILESYSTEM_OPTIONS_NAMESPACE_END
