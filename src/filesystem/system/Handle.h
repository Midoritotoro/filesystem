#pragma once 

#include <filesystem/compatibility/Compatibility.h>

__FILESYSTEM_SYSTEM_NAMESPACE_BEGIN

class handle {
public:
	using native_handle_type = system_handle_t;
	using deleter_type = system_bool_t(*)(native_handle_type);

	handle() noexcept {}
	~handle() noexcept {}

	handle(const handle& other) noexcept : _native_handle(other._native_handle), _deleter(other._deleter) {}

	handle(native_handle_type handle) noexcept : _native_handle(handle) {}
	handle(handle&& other) noexcept : _native_handle(other._native_handle), _deleter(std::move(other._deleter)) {
		other._native_handle = INVALID_HANDLE_VALUE;
	}

	handle& operator=(const handle& other) noexcept {
		_native_handle = other._native_handle;
		_deleter = other._deleter;
		return *this;
	}

	handle& operator=(handle&& other) noexcept {
		if (this != &other) {
			destroy();
			_native_handle = other._native_handle;
			_deleter = std::move(other._deleter);
			other._native_handle = INVALID_HANDLE_VALUE;
		}

		return *this;
	}

	handle& operator=(native_handle_type other) noexcept {
		_native_handle = other;
		return *this;
	}

	handle(native_handle_type handle, deleter_type deleter) noexcept :
		_native_handle(handle), _deleter(std::move(deleter))
	{}

	void set_deleter(deleter_type deleter) noexcept {
		_deleter = std::move(deleter);
	}

	filesystem_nodiscard deleter_type deleter() const noexcept {
		return _deleter;
	}

	void set_native(native_handle_type handle, bool delete_previous = true) noexcept {
		if (delete_previous) destroy();
		_native_handle = handle;
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
	
	friend bool operator==(const handle& x, const handle& y) noexcept { 
		return x._native_handle == y._native_handle;
	}
	
	friend bool operator!=(const handle& x, const handle& y) noexcept {
		return x._native_handle != y._native_handle;
	}
protected:
	native_handle_type _native_handle = INVALID_HANDLE_VALUE;
	deleter_type _deleter = &CloseHandle;
};

__FILESYSTEM_SYSTEM_NAMESPACE_END
