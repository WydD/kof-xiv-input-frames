#include "stdafx.h"

#include <easyhook.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <Psapi.h>

void(*DrawKeyHistory)(void *obj, int playerID, void* history, bool bFrameDraw) = NULL;
void HookedDrawKeyHistory(void *obj, int playerID, void* history, bool bFrameDraw)
{
	return DrawKeyHistory(obj, playerID, history, true);
}

// EasyHook will be looking for this export to support DLL injection. If not found then 
// DLL injection will fail.
extern "C" void __declspec(dllexport) __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo);

void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo)
{
	// Dark magic!
	BYTE lookup[] = { 0x48, 0x89, 0x5C, 0x24, 0x08, 0x48, 0x89, 0x6C, 0x24, 0x10, 0x48, 0x89, 0x74, 0x24, 0x18, 0x57, 0x48, 0x83, 0xEC, 0x30, 0x48, 0x8B, 0x41, 0x08, 0x48, 0x8B, 0xD9, 0x48, 0x83, 0xC1, 0x08, 0x41, 0x0F, 0xB6, 0xF9, 0x49, 0x8B, 0xE8, 0x8B, 0xF2, 0xFF, 0x10, 0x84, 0xC0, 0x75, 0x3D, 0x38, 0x43 };

	MODULEINFO moduleInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(TEXT("kofxiv.exe")), &moduleInfo, sizeof(moduleInfo));

	BYTE* end = (BYTE*)(moduleInfo.lpBaseOfDll) + moduleInfo.SizeOfImage;
	for (BYTE* current = (BYTE*)moduleInfo.lpBaseOfDll; current < end; ++current) {
		if (*current != lookup[0]) {
			continue;
		}
		BOOL found = true;
		for (int i = 1; i < sizeof(lookup); ++i) {
			if (current[i] != lookup[i]) {
				found = false;
				break;
			}
		}
		if (found) {
			DrawKeyHistory = (void(*)(void *, int, void*, bool))current;
			break;
		}
	}

	if (!DrawKeyHistory) {
		return;
	}

	// Perform hooking
	HOOK_TRACE_INFO hHook = { NULL }; // keep track of our hook

									  // Install the hook
	NTSTATUS result = LhInstallHook(
		DrawKeyHistory,
		HookedDrawKeyHistory,
		NULL,
		&hHook);

	// If the threadId in the ACL is set to 0,
	// then internally EasyHook uses GetCurrentThreadId()
	ULONG ACLEntries[1] = { 0 };

	// Disable the hook for the provided threadIds, enable for all others
	LhSetExclusiveACL(ACLEntries, 1, &hHook);

	return;
}