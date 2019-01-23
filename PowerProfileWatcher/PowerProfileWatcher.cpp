#include "pch.h"
#include "PowerProfileWatcher.h"
#include <windows.h>
#include <powrprof.h>
#include <thread>
#include <chrono>

PowerProfileWatcher::PowerProfileWatcher()
{
	GUID *activeScheme;
	GUID targetScheme;

	for (int i = 0; ; i++)
	{
		GUID enumerateBuffer;
		DWORD enumerateBufferSize = sizeof(enumerateBuffer);
		DWORD enumerateResult = PowerEnumerate(NULL, NULL, NULL, ACCESS_SCHEME, i, (UCHAR*)&enumerateBuffer, &enumerateBufferSize);
		if (enumerateResult == ERROR_SUCCESS)
		{
			DWORD nameBufferSize;
			if (PowerReadFriendlyName(NULL, &enumerateBuffer, NULL, NULL, NULL, &nameBufferSize) == ERROR_SUCCESS)
			{
				UCHAR *name = new UCHAR[nameBufferSize];
				DWORD nameResult = PowerReadFriendlyName(NULL, &enumerateBuffer, NULL, NULL, name, &nameBufferSize);
				if (nameResult == ERROR_SUCCESS)
				{
					if (wcscmp((WCHAR*)name, L"Ultimate Performance") == 0)
					{
						targetScheme = enumerateBuffer;
						delete[] name;
						break;
					}
				}
				delete[] name;
			}
		}
		else
		{
			break;
		}
	}

	while (true)
	{
		DWORD activeSchemeResult = PowerGetActiveScheme(NULL, &activeScheme);
		if (activeSchemeResult == ERROR_SUCCESS)
		{
			if (*activeScheme != targetScheme)
			{
				PowerSetActiveScheme(NULL, &targetScheme);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(60000));
	}
}