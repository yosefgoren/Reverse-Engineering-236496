#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
// Add additional includes if needed
// using namespace std;

 /*for logging. format: log_file << ... << std::endl;*/
 #include <fstream> 
 using std::ofstream;
ofstream log_file("log.txt");

// Typedef for the hooked function signature, such as:
// typedef INT(WINAPI* FUNC_PTR)(SOCKET, char*, int, int); (WINAPI is for _stdcall)
// or in the simple case:
typedef LPVOID FUNC_PTR;

// Ptr to the original function
FUNC_PTR original_func_address;

// Global variables
DWORD lpProtect = 0;
LPVOID IAT;
FUNC_PTR JumpTo;

// Helper function to remove the hook (for the current call):
void _stdcall remove_hook() {
	VirtualProtect((char*)IAT, 0x4, PAGE_EXECUTE_READWRITE, &lpProtect);
	memcpy(IAT, &original_func_address, 0x4);
	VirtualProtect((char*)IAT, 0x4, PAGE_EXECUTE_READ, &lpProtect);
}

// Helper function to restore the hook (for the next calls)
void _stdcall restore_hook() {
	VirtualProtect((char*)IAT, 0x4, PAGE_EXECUTE_READWRITE, &lpProtect);
	memcpy(IAT, &JumpTo, 0x4);
	VirtualProtect((char*)IAT, 0x4, PAGE_EXECUTE_READ, &lpProtect);
}

// Hook function. Might use helper functions in C, i.e. void _stdcall helper(int num) {}
// Might even use only a funcHook c function instead!
__declspec(naked) void funcHook() {
	// Restore overriden bytes
	// remove_hook();
	//MessageBox(NULL, L"from funcHook!", L"red alert!", MB_OK);

	// Assembly part. Might call restore_hook somewhere inside
	__asm {
		// ...
		// call restore_hook;
		// ...
		// ret; // maybe
	}
}

void setHook() {
	HMODULE start_addr = GetModuleHandle(NULL);
	HMODULE h2 = GetModuleHandle(L"user32.dll");

	if ((start_addr == NULL) || (h2 == NULL)) { return; }

	original_func_address = (FUNC_PTR)GetProcAddress(h2, "SetWindowTextW");
	if (original_func_address == NULL) {
		MessageBox(NULL, L"could not get orig func addr", L"red alert!", MB_OK);
		return;
	}

	int TGT_IAT_OFF = 0x1110;
	IAT = start_addr + TGT_IAT_OFF / 4; // Calc address of address to override in IAT
	log_file << std::hex << IAT << std::endl;
	MessageBox(NULL, L"got orig func addr", L"red alert!", MB_OK);

	JumpTo = (FUNC_PTR)((char*)&funcHook);
	restore_hook();
	MessageBox(NULL, L"finished injecting", L"red alert!", MB_OK);
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		setHook();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}