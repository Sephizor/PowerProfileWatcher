#include "pch.h"
#include "PowerProfileWatcher.h"
#include <windows.h>
#include <powrprof.h>
#include <thread>
#include <chrono>
#include <iostream>

PowerProfileWatcher::PowerProfileWatcher()
{
	GUID *activeScheme;
	GUID targetScheme;
	time_t currentTime;
	struct tm timeInfo;
	char timeBuffer[80];

	time(&currentTime);
	localtime_s(&timeInfo, &currentTime);
	strftime(timeBuffer, 80, "[%H:%M:%S]: ", &timeInfo);

	std::cout << timeBuffer << "Welcome to the power profile watcher" << std::endl;

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
		time(&currentTime);
		localtime_s(&timeInfo, &currentTime);
		WCHAR *activeSchemeName = NULL;
		DWORD activeSchemeResult = PowerGetActiveScheme(NULL, &activeScheme);

		if (activeSchemeResult == ERROR_SUCCESS)
		{
			DWORD activeSchemeNameBufferSize;
			if (PowerReadFriendlyName(NULL, activeScheme, NULL, NULL, NULL, &activeSchemeNameBufferSize) == ERROR_SUCCESS)
			{
				activeSchemeName = new WCHAR[activeSchemeNameBufferSize];
				PowerReadFriendlyName(NULL, activeScheme, NULL, NULL, (UCHAR*)activeSchemeName, &activeSchemeNameBufferSize);
			}
			if (*activeScheme != targetScheme)
			{
				if (activeSchemeName != NULL)
				{
					strftime(timeBuffer, 80, "[%H:%M:%S]: ", &timeInfo);
					std::wcout << timeBuffer
						<< L"The active scheme was changed from \""
						<< activeSchemeName
						<< "\" to \"Ultimate Performance\""
						<< std::endl;
				}
				PowerSetActiveScheme(NULL, &targetScheme);
			}
			delete[] activeSchemeName;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(60000));
	}
}