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
    constexpr decorated_with(_OptionsValues_ values):
        _OptionsValues_(std::move(values)) 
    {}

    template <class Options_>
    filesystem_nodiscard constexpr filesystem_always_inline auto operator[](Options_ options) const
        requires(requires(const _OptionsValues_& values) { this->process(values, options); })
    {
        return process(static_cast<const _OptionsValues_&>(*this), options);
    }

    filesystem_nodiscard constexpr filesystem_always_inline auto options() const {
        return fold_left(accumulate_decorations{}, std::tuple<_Options_...>{},
            static_cast<const _OptionsValues_&>(*this));
    }
};

__FILESYSTEM_OPTIONS_NAMESPACE_END