#pragma once 

#include <src/filesystem/options/Concepts.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <concepts::keyword _Keyword_, class _Value_>
struct option {
    using stored_value_type = std::decay_t<_Value_>;
    using keyword_type = _Keyword_;

    constexpr filesystem_always_inline stored_value_type operator()(const keyword_type&) const noexcept {
        return _contents;
    }

    stored_value_type _contents;
};

__FILESYSTEM_OPTIONS_NAMESPACE_END
