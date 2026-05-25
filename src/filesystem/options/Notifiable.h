#pragma once 

#include <src/filesystem/options/Callable.h>
#include <src/filesystem/system/IoError.h>

namespace fs::io {
    class file;
}

__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

struct completion_callback_key_t : as_keyword<completion_callback_key_t> {
    using as_keyword<completion_callback_key_t>::operator=;
};

constexpr inline completion_callback_key_t completion_callback_key = {};

struct __ignore_none {};
constexpr inline __ignore_none ignore_none = {};

template <class _OnComplete_>
struct on_completion {
    using callback_type = _OnComplete_;

    on_completion(_OnComplete_ __on_complete):
        _on_complete(__on_complete)
    {}

	_OnComplete_ callback() const {
		return _on_complete;
	}

    _OnComplete_ _on_complete;
};

template <class _Type_>
concept on_completion_expression = requires { typename _Type_::callback_type; };

struct completion_callback_option {
    filesystem_always_inline constexpr auto process(const auto& __base,
        concepts::exactly<completion_callback_key> auto __options) const
    {
        return fs::options::merge_prefer_first(__base, options{ __options });
    }

    filesystem_always_inline constexpr auto process(const auto& __base,
        on_completion_expression auto __option) const noexcept
    {
        return process(__base, completion_callback_key = __option);
    }

    filesystem_always_inline constexpr auto default_to(const auto& __base) const {
        return fs::options::merge_prefer_first(options { completion_callback_key = ignore_none }, __base);
    }
};

template <template <class> class _Functor_, class _OptionsValues_, class ... _Options_>
struct notifiable : callable<_Functor_, _OptionsValues_, completion_callback_option, _Options_...> {
    using base_t = callable<_Functor_, _OptionsValues_, completion_callback_option, _Options_...>;
    using func_t = _Functor_<_OptionsValues_>;

    template <class ... _Args_>
    filesystem_always_inline constexpr auto behavior(_Args_&& ... __args) const
        fs_noexcept_if(func_t::deferred_call(std::forward<_Args_>(__args)...))
    {
        return func_t::deferred_call(std::forward<_Args_>(__args)...);
    }
};

__FILESYSTEM_OPTIONS_NAMESPACE_END
