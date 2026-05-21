#pragma once 

#include <filesystem/compatibility/Compatibility.h>
#include <src/filesystem/io/Exceptions.h>

__FILESYSTEM_IO_NAMESPACE_BEGIN

struct _Overlapped : OVERLAPPED {
	_Overlapped() noexcept {
		Internal = 0;
		InternalHigh = 0;
		Offset = 0;
		OffsetHigh = 0;
		
        hEvent = CreateEventW(0, TRUE, FALSE, 0);

        if (hEvent) hEvent = reinterpret_cast<HANDLE>(reinterpret_cast<DWORD_PTR>(hEvent) | 1);
        else throw filesystem_error("Failed to create overlapped event", __get_last_error_code());
	}

	~_Overlapped() {
		if (hEvent) CloseHandle(hEvent);
	}
};

__FILESYSTEM_IO_NAMESPACE_END
