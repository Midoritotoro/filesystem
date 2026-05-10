#pragma once 

#include <filesystem/compatibility/Compatibility.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <class _Type_>
struct as {
    using type = _Type_;

    constexpr as() noexcept {}
    explicit constexpr as(const _Type_&)  noexcept {}
};

template <>
struct as<void> {
    using type = void;
    constexpr as() noexcept {}
};

__FILESYSTEM_OPTIONS_NAMESPACE_END
