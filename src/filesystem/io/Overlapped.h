#pragma once 

#include <filesystem/compatibility/Compatibility.h>
#include <src/filesystem/io/Exceptions.h>

__FILESYSTEM_IO_NAMESPACE_BEGIN

struct _Overlapped : OVERLAPPED {
	_Overlapped() {
		Internal = 0;
		InternalHigh = 0;
		Offset = 0;
		OffsetHigh = 0;
		
        hEvent = CreateEventW(0, TRUE, FALSE, 0);

        if (hEvent) hEvent = reinterpret_cast<HANDLE>(reinterpret_cast<DWORD_PTR>(hEvent) | 1);
        else throw filesystem_error("Failed to create overlapped event", system::make_error_code(__get_last_io_error()));
	}

	~_Overlapped() {
		if (hEvent) CloseHandle(hEvent);
	}

	void set_offset(sizetype __offset) noexcept {
		Offset = static_cast<dword_t>(__offset & 0xFFFFFFFFu);
		if constexpr (sizeof(sizetype) > sizeof(dword_t)) OffsetHigh = static_cast<dword_t>((__offset >> 32u) & 0xFFFFFFFFu);
	}
};

__FILESYSTEM_IO_NAMESPACE_END
