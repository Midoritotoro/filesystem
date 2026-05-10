#pragma once 

#include <src/filesystem/options/Or.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <class _Condition_>
struct if_ {
    static constexpr bool is_inverted = false;
    static constexpr bool is_complete = false;

    using condition_type = _Condition_;

    if_(_Condition_ __condition) noexcept:
        _condition(__condition) 
    {}

    template <class _Type_>
    filesystem_always_inline auto mask(const as<_Type_>&) const noexcept {
        return _condition;
    }

    _Condition_ _condition;
};

__FILESYSTEM_OPTIONS_NAMESPACE_END
