// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Shlwapi.h>
#include <stdio.h>

LPVOID hooked_function;
CHAR  JmpOpcode[7] = "\xE9\x90\x90\x90\x90\x90";
CHAR  prevOpcode[6] = "\x90\x90\x90\x90\x90";
int   counter = 0;
CHAR  tz[10] = "123456789";


void _stdcall returnstate() {
	DWORD lpProtect = 0;

	VirtualProtect((char*)hooked_function, 0x6, PAGE_EXECUTE_READWRITE, &lpProtect);
	memcpy((char*)hooked_function, &prevOpcode, 0x6);
	VirtualProtect((char*)hooked_function, 0x6, PAGE_EXECUTE_READ, &lpProtect);
}

CHAR _stdcall xortheoutput(CHAR c) {
	//printf("to xor: %c counter %d xoring with %c xored reasult %c\n", c, counter,taz[counter%9],c ^ taz[counter%9]);
	CHAR res =  c ^ tz[counter % 9];
	counter++;
	return res;
}

void _stdcall restorehook() {
	DWORD lpProtect = 0;

	VirtualProtect((char*)hooked_function, 0x6, PAGE_EXECUTE_READWRITE, &lpProtect);
	memcpy((char*)hooked_function, &JmpOpcode, 0x6);
	VirtualProtect((char*)hooked_function, 0x6, PAGE_EXECUTE_READ, &lpProtect);
}


__declspec(naked) void secretHook()
{
	static LPVOID originalRet;
	__asm {
		pop originalRet
		push	hook_end

		call returnstate

		jmp	hooked_function

		hook_end:

		push eax
		call xortheoutput
		push eax
		call restorehook
		pop eax
		push originalRet
		ret
	}
}



void setSecretHook() {
	HMODULE prog = GetModuleHandle(L"secret.exe");
	DWORD lpProtect = 0;
	LPVOID JumpTo;

	if (prog == NULL) {
		MessageBox(0, L"Error Occurred: Prog", 0, 0);
		return;
	}

	hooked_function = (char*)prog + 0x1803;

	JumpTo = (LPVOID)((char*)&secretHook - ((char*)hooked_function + 5));
	memcpy(JmpOpcode + 1, &JumpTo, 0x4);

	VirtualProtect((char*)hooked_function, 0x6, PAGE_EXECUTE_READWRITE, &lpProtect);
	memcpy(&prevOpcode, (char*)hooked_function, 0x6);

	memcpy((char*)hooked_function, &JmpOpcode, 0x6);
	VirtualProtect((char*)hooked_function, 0x6, PAGE_EXECUTE_READ, &lpProtect);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		setSecretHook();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}