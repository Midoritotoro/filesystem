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
    filesystem_always_inline auto operator()(auto, auto x) const {
        return x; 
    } 
} return_2nd = {};

template <template <class> class _Function_, class _OptionsValues_, class ... _Options_>
struct strict_elementwise_callable: callable<_Function_, _OptionsValues_, _Options_...> {
    using base_t = callable<_Function_, _OptionsValues_, _Options_...>;
    using func_t =  _Function_<_OptionsValues_>;

    template <callable_options Options_, class _Type_, class ... _Types_>
    constexpr filesystem_always_inline auto behaviour(const Options_& options, 
        _Type_ first, _Types_ ... args) const
    {
        if constexpr (requires{ func_t::deferred_call(options, first, args...); }) return func_t::deferred_call(options, first, args...);
        else return ignore{};
    }
};

template <class _Callable_, class ... _Args_>
constexpr filesystem_always_inline auto dispatch_call(const _Callable_& callable, _Args_&& ... args)
    fs_noexcept_if(callable.behavior(callable.options(), std::forward<_Args_>(args)...))
{
    using _ReturnType = decltype(callable(std::forward<_Args_>(args)...));
    if constexpr (std::is_void_v<_ReturnType>) callable.behavior(callable.options(), std::forward<_Args_>(args)...);
    else return callable.behavior(callable.options(), std::forward<_Args_>(args)...);
}

__FILESYSTEM_OPTIONS_NAMESPACE_END
