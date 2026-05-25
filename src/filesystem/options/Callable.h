#pragma once 

#include <src/filesystem/options/Merge.h>
#include <src/filesystem/options/Options.h>
#include <src/filesystem/options/DecoratedWith.h>

__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <template<class> class _Functor_, class _OptionsValues_, class ... _Options_>
struct callable:
    decorated_with<_OptionsValues_, _Options_...>
{
    using base = decorated_with<_OptionsValues_, _Options_...>;

    template <callable_options __Options_> 
    filesystem_always_inline constexpr auto operator[](const __Options_& opts) const noexcept {
        auto merged = fs::options::merge(opts, this->options());
        fs::options::options<decltype(merged)> new_options {merged};
        return _Functor_<decltype(new_options)>{new_options};
    }

    template <class _Type_>
    filesystem_always_inline constexpr auto operator[](_Type_ t) const noexcept
        requires(requires(const base& b) { b[t];}) 
    {
        auto new_traits = base::operator[](t);
        return _Functor_<decltype(new_traits)>{new_traits};
    }

    template <class _Type_>
    filesystem_always_inline void operator[](const _Type_& t) const noexcept
        requires(!callable_options<_Type_> && !requires(const base& b) { b[t]; }) = delete;

    template <class ... Args>
    filesystem_always_inline constexpr auto behavior(Args&& ... args) const 
        fs_noexcept_if(_Functor_<_OptionsValues_>::deferred_call(std::forward<Args>(args)...))
    {
        return _Functor_<_OptionsValues_>::deferred_call(std::forward<Args>(args)...);
    }

    template <class ... Args>
    filesystem_always_inline constexpr auto retarget(Args&& ... args) const 
        fs_noexcept_if(_Functor_<_OptionsValues_>::deferred_call(this->options(), std::forward<Args>(args)...))
    {
        return _Functor_<_OptionsValues_>::deferred_call(this->options(), std::forward<Args>(args)...);
    }

protected:
    filesystem_always_inline constexpr _Functor_<_OptionsValues_> const& derived() const noexcept {
        return static_cast<_Functor_<_OptionsValues_>const&>(*this); 
    }

    template <template<class> class _F2_, class _OV2_, class ... _O2_>
    friend filesystem_always_inline constexpr bool operator==(const callable&, const callable<_F2_, _OV2_, _O2_...>&) noexcept {
        return false; 
    }

    friend filesystem_always_inline constexpr bool operator==(const callable&, const callable&) noexcept {
        return true;  
    }
};


__FILESYSTEM_OPTIONS_NAMESPACE_END
