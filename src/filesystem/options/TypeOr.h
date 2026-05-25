#pragma once 

#include <src/filesystem/options/Concepts.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <concepts::keyword _Type_, class _Value_> 
struct type_or {
    _Value_ _value;

    template <concepts::option ... _Options_>
    constexpr filesystem_always_inline decltype(auto) operator()(_Options_&&... options) const noexcept {
        return fetch(*this, std::forward<_Options_>(options)...); 
    }
};

__FILESYSTEM_OPTIONS_NAMESPACE_END
