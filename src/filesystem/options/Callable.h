#pragma once 

#include <src/filesystem/options/Merge.h>
#include <src/filesystem/options/Options.h>
#include <src/filesystem/options/DecoratedWith.h>

__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

struct __decorator 
{};

template <class _ConditionType_>
concept condition_type = std::is_same_v<_ConditionType_, bool>;

template <class _ID_>
concept decorator = std::derived_from<_ID_, __decorator>;

template <template<class> class _Functor_, class _OptionsValues_, class ... _Options_>
struct callable:
    decorated_with<_OptionsValues_, _Options_...>
{
    using base = decorated_with<_OptionsValues_, _Options_...>;

    template <callable_options __Options_> 
    filesystem_always_inline constexpr auto operator[](const __Options_& __options) const noexcept {
        auto __merged = fs::options::merge(__options, this->options());
        fs::options::options<decltype(__merged)> __new_options{__merged};
        return _Functor_<decltype(__new_options)>{__new_options};
    }

    template <class _Type_>
    filesystem_always_inline constexpr auto operator[](_Type_ __t) const noexcept
        requires(requires(const base& __base) { __base[__t];}) 
    {
        auto __new_traits = base::operator[](__t);
        return _Functor_<decltype(__new_traits)>{__new_traits};
    }

    template <class _Type_>
    filesystem_always_inline void operator[](const _Type_& __t) const noexcept
        requires(!callable_options<_Type_> && !decorator<_Type_> &&
            !requires(const base& __base) { __base[__t]; }) = delete;

    template <class ... Args>
    filesystem_always_inline constexpr auto behavior(Args&& ... __args) const noexcept {
        return _Functor_<_OptionsValues_>::deferred_call(std::forward<Args>(__args)...);
    }

    template <class ... Args>
    filesystem_always_inline constexpr auto retarget(Args&& ... __args) const noexcept {
        return _Functor_<_OptionsValues_>::deferred_call(this->options(), std::forward<Args>(__args)...);
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
