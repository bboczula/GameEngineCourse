#pragma once

#include <Windows.h>
#include <comdef.h>

inline void ExitIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		_com_error error(hr);
		MessageBox(NULL, error.ErrorMessage(), L"Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

inline void ExitIfTrue(bool condition, LPCWSTR msg)
{
	if (condition)
	{
		MessageBox(NULL, msg, L"Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

template <class T>
void SafeRelease(T** comObjectAddresssOf)
{
	if (*comObjectAddresssOf)
	{
		(*comObjectAddresssOf)->Release();
		*comObjectAddresssOf = NULL;
	}
}