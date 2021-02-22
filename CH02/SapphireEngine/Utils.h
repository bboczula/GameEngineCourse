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