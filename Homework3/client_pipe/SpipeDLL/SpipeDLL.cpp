// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <Shlwapi.h>
#include <fstream>
#include <iostream>
#include <ios>
#include <string>
#include <algorithm>

using std::ofstream;
using std::endl;
using std::wstring;

ofstream log_file("log-sol.txt");


// change this to the proper signature of the hook
typedef void(*HOOK_TYPE)(unsigned int);

HOOK_TYPE orig_srand;

void print_srand_seed(unsigned int seed) {
    log_file << "seed is: " << std::hex << seed << std::endl;
    orig_srand(seed);
}

void setHook() {
    log_file << "dll attached" << endl;

    HMODULE curr_prog = GetModuleHandle(NULL); // get the handle of the main process
    HMODULE target_dll = GetModuleHandle(L"msvcrt.dll"); // get the handle of the dll conatining the function we want to override
    DWORD lpProtect = 0; 
    DWORD IAT_Func_Offset = 0x0000925C;
    LPVOID JumpTo;
    LPDWORD IAT_ADDRESS;

    if ((curr_prog == NULL) || (target_dll == NULL)){
        log_file << "couldnt get handles";
        return;
    }
   
    orig_srand = (HOOK_TYPE) GetProcAddress(target_dll, "srand");
    if (orig_srand == NULL) {
        log_file << "couldnt find function" << endl;
        return;
    };
    IAT_ADDRESS = (LPDWORD)(curr_prog + IAT_Func_Offset / 4);
    if ((*IAT_ADDRESS) != (DWORD)orig_srand) {
        log_file << "IAT contents does not match - maybe check the offset again" << endl;
        return;
    }

    log_file << "changing IAT entry" << endl << "IAT address: " << IAT_ADDRESS << endl;
    JumpTo = (LPVOID)((char*)&print_srand_seed);
    VirtualProtect(IAT_ADDRESS, 0x4, PAGE_EXECUTE_READWRITE, &lpProtect);
    memcpy(IAT_ADDRESS, &JumpTo, 0x4);
    VirtualProtect(IAT_ADDRESS, 0x4, PAGE_EXECUTE_READWRITE, &lpProtect);

    log_file << "finished setting hook\n";
}

BOOL APIENTRY DllMain( HMODULE hModule,
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

