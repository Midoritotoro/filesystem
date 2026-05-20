#pragma once 

#include <filesystem/compatibility/Compatibility.h>

__FILESYSTEM_SYSTEM_NAMESPACE_BEGIN

class handle {
public:
	using native_handle_type = system_handle_t;
	using deleter_type = system_bool_t(*)(native_handle_type);

	handle() noexcept {}
	~handle() noexcept {
		if (_auto_delete) destroy();
	}

	handle(native_handle_type __handle) noexcept : _native_handle(__handle) {}

	handle& operator=(const handle& __other) noexcept {
		_native_handle = __other._native_handle;
		return *this;
	}

	handle& operator=(const native_handle_type __other) noexcept {
		_native_handle = __other;
		return *this;
	}

	handle(native_handle_type __handle, bool __auto_delete, deleter_type __deleter) noexcept :
		_native_handle(__handle), _auto_delete(__auto_delete), _deleter(std::move(__deleter))
	{}

	void set_deleter(deleter_type __deleter) noexcept {
		_deleter = std::move(__deleter);
	}

	filesystem_nodiscard deleter_type deleter() const noexcept {
		return _deleter;
	}

	void set_auto_delete(bool __auto_delete) noexcept {
		_auto_delete = __auto_delete;
	}

	filesystem_nodiscard bool auto_delete() const noexcept {
		return _auto_delete;
	}

	void set_native_handle(native_handle_type __handle, bool __delete_previous = true) noexcept {
		if (__handle == _native_handle) return;
		if (__delete_previous) destroy();

		_native_handle = __handle;
	}

	filesystem_nodiscard native_handle_type native_handle() noexcept {
		return _native_handle;
	}

	bool destroy() noexcept {
		if (_native_handle == nullptr) return false;
		
		_deleter(_native_handle);
		_native_handle = nullptr;

		return true;
	}

	filesystem_nodiscard bool available() const noexcept {
		return (_native_handle != nullptr);
	}
	
	friend bool operator==(const handle& __x, const handle& __y) noexcept { 
		return __x._native_handle == __y._native_handle;
	}
	
	friend bool operator!=(const handle& __x, const handle& __y) noexcept {
		return __x._native_handle != __y._native_handle;
	}
protected:
	native_handle_type _native_handle = nullptr;
	deleter_type _deleter = nullptr;
	bool _auto_delete = true;
};

__FILESYSTEM_SYSTEM_NAMESPACE_END
