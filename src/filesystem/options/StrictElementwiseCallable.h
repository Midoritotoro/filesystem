#pragma once 

#include <src/filesystem/options/Concepts.h>
#include <src/filesystem/options/Fetch.h>
#include <src/filesystem/options/Callable.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

struct ignore {
    template <class _Type_> operator _Type_() {
        return _Type_{};
    }
};

template <auto _Keyword_, class _Options_, class _Type_>
concept match_option = concepts::same_as<_Type_, fetch_t<_Keyword_, _Options_>>;

inline constexpr struct { 
    filesystem_always_inline auto operator()(auto, auto __x) const {
        return __x; 
    } 
} return_2nd = {};

template <template <class> class _Function_, class _OptionsValues_, class ... _Options_>
struct strict_elementwise_callable: callable<_Function_, _OptionsValues_, _Options_...> {
    using base_t = callable<_Function_, _OptionsValues_, _Options_...>;
    using func_t =  _Function_<_OptionsValues_>;

    template <callable_options __Options_, class _Type_, class ... _Types_>
    constexpr filesystem_always_inline auto behaviour(const __Options_& __options, 
        _Type_ __first, _Types_ ... __args) const noexcept
    {
        if constexpr (requires{ func_t::deferred_call(__options, __first, __args...); }) return func_t::deferred_call(__options, __first, __args...);
        else return ignore{};
    }
};

template <class _Callable_, class ... _Args_>
constexpr filesystem_always_inline auto __dispatch_call(const _Callable_& __callable, _Args_&& ... __args) noexcept {
    using _ReturnType = decltype(__callable(std::forward<_Args_>(__args)...));
    if constexpr (std::is_void_v<_ReturnType>) __callable.behavior(__callable.options(), std::forward<_Args_>(__args)...);
    else return __callable.behavior(__callable.options(), std::forward<_Args_>(__args)...);
}

__FILESYSTEM_OPTIONS_NAMESPACE_END
