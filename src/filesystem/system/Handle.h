#pragma once 

#include <filesystem/compatibility/Compatibility.h>

__FILESYSTEM_SYSTEM_NAMESPACE_BEGIN

class handle {
public:
	using native_handle_type = system_handle_t;
	using deleter_type = system_bool_t(*)(native_handle_type);

	handle() noexcept {}

	~handle() noexcept {
		destroy();
	}

	handle(const handle& __other) noexcept = delete;
	handle& operator=(const handle& __other) noexcept = delete;

	handle(native_handle_type __handle) noexcept : _native_handle(__handle) {}
	handle(handle&& __other) noexcept : _native_handle(__other._native_handle), _deleter(std::move(__other._deleter)) {
		__other._native_handle = INVALID_HANDLE_VALUE;
	}

	handle& operator=(handle&& __other) noexcept {
		if (this != &__other) {
			destroy();
			_native_handle = __other._native_handle;
			_deleter = std::move(__other._deleter);
			__other._native_handle = INVALID_HANDLE_VALUE;
		}

		return *this;
	}

	handle& operator=(native_handle_type __other) noexcept {
		_native_handle = __other;
		return *this;
	}

	handle(native_handle_type __handle, deleter_type __deleter) noexcept :
		_native_handle(__handle), _deleter(std::move(__deleter))
	{}

	void set_deleter(deleter_type __deleter) noexcept {
		_deleter = std::move(__deleter);
	}

	filesystem_nodiscard deleter_type deleter() const noexcept {
		return _deleter;
	}

	void set_native(native_handle_type __handle, bool __delete_previous = true) noexcept {
		if (__delete_previous) destroy();
		_native_handle = __handle;
	}

	filesystem_nodiscard native_handle_type native() noexcept {
		return _native_handle;
	}

	void destroy() noexcept {
		if (!available()) return;
		
		_deleter(_native_handle);
		_native_handle = INVALID_HANDLE_VALUE;
	}

	filesystem_nodiscard bool available() const noexcept {
		return (_native_handle != INVALID_HANDLE_VALUE && _native_handle != NULL);
	}
	
	friend bool operator==(const handle& __x, const handle& __y) noexcept { 
		return __x._native_handle == __y._native_handle;
	}
	
	friend bool operator!=(const handle& __x, const handle& __y) noexcept {
		return __x._native_handle != __y._native_handle;
	}
protected:
	native_handle_type _native_handle = INVALID_HANDLE_VALUE;
	deleter_type _deleter = &CloseHandle;
};

__FILESYSTEM_SYSTEM_NAMESPACE_END
