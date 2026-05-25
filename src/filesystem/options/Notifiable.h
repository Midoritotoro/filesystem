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

struct ignore_none_t {};
constexpr inline ignore_none_t ignore_none = {};

template <class _OnComplete_>
struct on_completion {
    using callback_type = _OnComplete_;

    on_completion(_OnComplete_ on_complete):
        _on_complete(on_complete)
    {}

	_OnComplete_ callback() const {
		return _on_complete;
	}

    _OnComplete_ _on_complete;
};

template <class _Type_>
concept on_completion_expression = requires { typename _Type_::callback_type; };

struct completion_callback_option {
    filesystem_always_inline constexpr auto process(const auto& base,
        concepts::exactly<completion_callback_key> auto opts) const
    {
        return fs::options::merge_prefer_first(base, options{ opts });
    }

    filesystem_always_inline constexpr auto process(const auto& base,
        on_completion_expression auto option) const noexcept
    {
        return process(base, completion_callback_key = option);
    }

    filesystem_always_inline constexpr auto default_to(const auto& base) const {
        return fs::options::merge_prefer_first(options { completion_callback_key = ignore_none }, base);
    }
};

template <template <class> class _Functor_, class _OptionsValues_, class ... _Options_>
struct notifiable : callable<_Functor_, _OptionsValues_, completion_callback_option, _Options_...> {
    using base_t = callable<_Functor_, _OptionsValues_, completion_callback_option, _Options_...>;
    using func_t = _Functor_<_OptionsValues_>;

    template <class ... _Args_>
    filesystem_always_inline constexpr auto behavior(_Args_&& ... args) const
        fs_noexcept_if(func_t::deferred_call(std::forward<_Args_>(args)...))
    {
        return func_t::deferred_call(std::forward<_Args_>(args)...);
    }
};

__FILESYSTEM_OPTIONS_NAMESPACE_END
