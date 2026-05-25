#pragma once 

#include <src/filesystem/options/SelectKeys.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <concepts::keyword _Keyword_, concepts::option ... _Options_>
constexpr filesystem_always_inline auto drop(const _Keyword_&, const settings<_Options_...>& settings_) noexcept {
    using selected_keys_t = typename select_keys<_Keyword_, settings<_Options_...>>::type;
    return [&]<class ... _Keys_>(keys<_Keys_...>) { 
        return settings { (_Keys_{} = settings_[_Keys_{}])...};
    }(selected_keys_t{});
}

__FILESYSTEM_OPTIONS_NAMESPACE_END
