// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Shlwapi.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <ios>
#include <string>


using std::ofstream;
using std::endl;
using std::wstring;
using std::string;

ofstream log_file("log.txt");

typedef int(*HOOK_TYPE)(int x);
HOOK_TYPE f;

LPVOID hooked_function_end;
char* originalRet;
CHAR  JmpOpcode[6] = "\xE9\x90\x90\x90\x90";
boolean calledFromItself = false;
int i = 1;
int x;
CHAR  tz[10] = "123456789";

char* findCallAddress(char* address) {
	if (address[0] != '\xE8') {
		return NULL;
	}
	int x = *(int*)(address + 1);
	return address + x + 5;
}

__declspec(naked) void secretHook()
{
	log_file << "entered hook" << endl;
	if (calledFromItself) {
		calledFromItself = false;
		i++;

		__asm {
			mov eax, [ebp - 4]
			leave
			pop ecx
			leave
			retn
		}
	}
	else {
		__asm {
			mov eax, [ebp + 8]
			mov x, eax
			mov eax, [ebp + 4]
			mov originalRet, eax
		}

		f = (HOOK_TYPE)findCallAddress(originalRet - 5);

		if (f == NULL) {
			log_file << "oops" << endl;
			exit(1);
		}

		calledFromItself = true;
		f(x + i);
	}
}



void setSecretHook() {
	log_file << "got to dllmain" << endl;
	HMODULE prog = GetModuleHandle(L"S21A-secret.exe");
	DWORD lpProtect = 0;
	LPVOID JumpTo;

	if (prog == NULL) {
		MessageBox(0, L"Error Occurred: Prog", 0, 0);
		return;
	}

	hooked_function_end = (char*)prog + 0x150D;

	log_file << "Jump to (basic): ";
	for (int i = 0; i < 5; ++i)
		log_file << std::hex << (int)JmpOpcode[i];
	 log_file << endl;

	JumpTo = (LPVOID)((char*)&secretHook - ((char*)hooked_function_end));
	memcpy(JmpOpcode + 1, &JumpTo, 0x4);

	log_file << "Jump to (after inserting diff): ";
	for (int i = 0; i < 5; ++i)
		log_file << std::hex << (int)JmpOpcode[i];
	log_file << endl;

	VirtualProtect((char*)hooked_function_end - 5, 0x5, PAGE_EXECUTE_READWRITE, &lpProtect);
	memcpy((char*)hooked_function_end - 5, &JmpOpcode, 0x5);

	log_file << "implanted the bytes " << int((char*)JmpOpcode) << endl;
	log_file << "in the adress " << int((char*)hooked_function_end) << endl;
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