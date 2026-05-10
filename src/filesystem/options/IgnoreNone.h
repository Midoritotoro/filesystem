#pragma once 

#include <src/filesystem/options/Concepts.h>
#include <src/filesystem/options/As.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

struct __ignore_none {
    static constexpr bool is_inverted = true;
    static constexpr bool is_complete = true;

    template <class _Type_> 
    filesystem_always_inline _Type_ mask(const as<_Type_>& __target) const noexcept {
        return _Type_{};
    }

    template <class _Type_> 
    constexpr filesystem_always_inline std::ptrdiff_t offset(const as<_Type_>&) const noexcept {
        return 0;
    }

    template <class _Type_>
    constexpr filesystem_always_inline std::ptrdiff_t roffset(const as<_Type_>&) const noexcept {
        return 0;
    }

    template <class _Type_>
    constexpr filesystem_always_inline std::ptrdiff_t count(const as<_Type_>&) const noexcept {
        return _Type_::size();
    }

    filesystem_always_inline constexpr bool friend operator==(__ignore_none const&, __ignore_none const&) noexcept = default;
};

constexpr inline __ignore_none ignore_none = {};


__FILESYSTEM_OPTIONS_NAMESPACE_END
