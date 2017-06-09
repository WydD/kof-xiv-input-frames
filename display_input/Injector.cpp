// Injector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <cstring>

#include <easyhook.h>

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Loading input display injection!\n";

	HWND winhandle = FindWindow(NULL, L"The King of Fighters XIV");
	if (!winhandle) {
		std::cout << "KOF XIV is not running!\n";


		std::wcout << "Press Enter to exit\n";
		std::wstring input;
		std::getline(std::wcin, input);
		return 1;
	}
	DWORD processId;
	GetWindowThreadProcessId(winhandle, &processId);

	WCHAR* dllToInject = L"input_display_hook.dll";
	wprintf(L"Attempting to inject: %s\n\n", dllToInject);

	// Inject dllToInject into the target process Id, passing 
	// freqOffset as the pass through data.
	NTSTATUS nt = RhInjectLibrary(
		processId,   // The process to inject into
		0,           // ThreadId to wake up upon injection
		EASYHOOK_INJECT_DEFAULT,
		NULL, // 32-bit
		dllToInject,		 // 64-bit not provided
		NULL,
		0
	);

	if (nt != 0)
	{
		printf("RhInjectLibrary failed with error code = %d\n", nt);
		PWCHAR err = RtlGetLastErrorString();
		std::wcout << err << "\n";
	}
	else
	{
		std::wcout << L"Library injected successfully.\n";
	}

	std::wcout << "Everything is alright\nPress Enter to exit";
	std::wstring input;
	std::getline(std::wcin, input);
	return 0;
}