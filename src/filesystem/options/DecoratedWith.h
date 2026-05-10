#pragma once 

#include <src/filesystem/options/Concepts.h>
#include <src/filesystem/options/Foldable.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <class  _OptionsValues_, class ... _Options_>
struct decorated_with: 
    _OptionsValues_, _Options_...
{
    using _Options_::process...;

    constexpr decorated_with() noexcept {}
    constexpr decorated_with(_OptionsValues_ __values) noexcept:
        _OptionsValues_(std::move(__values)) 
    {}

    template <class __Options_>
    filesystem_nodiscard constexpr filesystem_always_inline auto operator[](__Options_ __options) const noexcept
        requires(requires(const _OptionsValues_& __values) { this->process(__values, __options); })
    {
        return process(static_cast<const _OptionsValues_&>(*this), __options);
    }

    filesystem_nodiscard constexpr filesystem_always_inline auto options() const noexcept {
        return fold_left(accumulate_decorations{}, std::tuple<_Options_...>{},
            static_cast<const _OptionsValues_&>(*this));
    }
};

__FILESYSTEM_OPTIONS_NAMESPACE_END