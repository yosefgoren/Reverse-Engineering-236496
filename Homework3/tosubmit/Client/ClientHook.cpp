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
#include <map>
#include <string>
#include <set>
#include <string>
#include <vector>
#include <sstream>

#include "dec.cpp"

using std::ofstream;
using std::endl;
using std::wstring;

ofstream log_file("log-sol.txt");


// change this to the proper signature of the hook
typedef int(*HOOK_TYPE)(const char*);

HOOK_TYPE orig_tgt;


int doHook(const char* str) {
    if (std::set<char>({'W', '[', 'y'}).count(str[0]) == 0){
        std::stringstream test(str);
        std::vector<std::string> seglist;
        std::string output = "";
        std::string segment;
        while (std::getline(test, segment, '\n'))
        {
            if (segment.size() > 0 && segment[segment.size() - 1] == 0x0d) {// '\r'
                segment = segment.substr(0, segment.size() - 1);//remove last
            }
            if (segment.size() < 1) {
                output += "\n";
                continue;
            }
            output += manualDecrypt(segment) + "\n";
        }
        return orig_tgt(output.c_str());
    }
    return orig_tgt(str);
}



void setHook() {
    log_file << "dll attached" << endl;

    HMODULE curr_prog = GetModuleHandle(NULL); // get the handle of the main process
    HMODULE target_dll = GetModuleHandle(L"msvcrt.dll"); // get the handle of the dll conatining the function we want to override
    DWORD lpProtect = 0; 
    
    DWORD IAT_Func_Offset = 0x0000A258;
    //0x0000925C for srand in secure_pipe.exe
    LPVOID JumpTo;
    LPDWORD IAT_ADDRESS;

    if ((curr_prog == NULL) || (target_dll == NULL)){
        log_file << "couldnt get handles";
        return;
    }
   
    orig_tgt = (HOOK_TYPE) GetProcAddress(target_dll, "puts");
    if (orig_tgt == NULL) {
        log_file << "couldnt find function" << endl;
        return;
    };
    IAT_ADDRESS = (LPDWORD)(curr_prog + IAT_Func_Offset / 4);
    printf("overwriting IAT address: 0x%08x\n", IAT_ADDRESS);
    if ((*IAT_ADDRESS) != (DWORD)orig_tgt) {
        log_file << "IAT contents does not match - maybe check the offset again" << endl;
        return;
    }

    log_file << "changing IAT entry" << endl << "IAT address: " << IAT_ADDRESS << endl;
    JumpTo = (LPVOID)((char*)&doHook);
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

