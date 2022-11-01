#pragma once

#include <Windows.h>
#include <comdef.h>

#define FRAME_COUNT 2

inline UINT Align(UINT location, UINT align)
{
	if ((0 == align) || (align & (align - 1)))
	{
		exit(1);
	}

	return ((location + (align - 1)) & ~(align - 1));
}

inline void ExitIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		_com_error error(hr);
		MessageBox(nullptr, error.ErrorMessage(), L"Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

template <class T>
void SafeRelease(T** comObjectAddresssOf)
{
	if (*comObjectAddresssOf)
	{
		(*comObjectAddresssOf)->Release();
		*comObjectAddresssOf = nullptr;
	}
}