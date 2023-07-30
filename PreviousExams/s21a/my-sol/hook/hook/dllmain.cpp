#include "pch.h"
#include <Windows.h>
#include <string>
#include <stdio.h>

// Add additional includes if needed
using namespace std;

// for logging. format: log_file << ... << std::endl;
#include <fstream> 
using std::ofstream;
ofstream log_file("log.txt");

// Typedef for the hooked function signature, such as:
// typedef INT(WINAPI* FUNC_PTR)(SOCKET, char*, int, int); (WINAPI is for _stdcall)
// or in the simple case:
typedef LPVOID FUNC_PTR;

// Ptr to the original function
FUNC_PTR original_func_address;
// Ptr to return to after hook
// LPVOID to_return_address;

// Global variables
CHAR OrigOpcode[6] = "\x90\x90\x90\x90\x90"; // Restores overriden bytes from hooked function
CHAR JmpOpcode[6] = "\xE9\x90\x90\x90\x90"; // Inserted into hooked function, in order to jmp to hook
DWORD lpProtect = 0;

// Helper function to remove the hook (for the current call):
void _stdcall remove_hook() {
	VirtualProtect((char*)original_func_address, 0x5, PAGE_EXECUTE_READWRITE, &lpProtect);
	memcpy((char*)original_func_address, &OrigOpcode, 0x5);
	VirtualProtect((char*)original_func_address, 0x5, PAGE_EXECUTE_READ, &lpProtect);
}

// Helper function to restore the hook (for the next calls)
void _stdcall restore_hook() {
	VirtualProtect((char*)original_func_address, 0x5, PAGE_EXECUTE_READWRITE, &lpProtect);
	memcpy((char*)original_func_address, &JmpOpcode, 0x5);
	VirtualProtect((char*)original_func_address, 0x5, PAGE_EXECUTE_READ, &lpProtect);
}

int cnt = 0;
int next_is_nested_invoke = 0;

int _stdcall getcnt(){
    cnt++;
    log_file << "cnt: " << cnt << std::endl;
    return cnt;
}

int x;
char* tgt;
char* _stdcall get_call_tgt(char* call) {
    if(call[0] != '\xE8') {
        log_file << "get_call_tgt called on non-'call' instruction! call[0]=" << std::hex << (unsigned char)(call[0]) << std::endl;
        return NULL;
    }
    x = *(int*)(call + 1);
    log_file << "x: " << std::hex << x << std::endl;
    tgt = call + 5 + x;
    return tgt;
}


char* called_from;
char* call_tgt;
int arg_offset;
int arg_value;

// Hook function. Might use helper functions in C, i.e. void _stdcall helper(int num) {}
__declspec(naked) void funcHook() {
    __asm {
        mov eax, [ebp+8]
        mov arg_value, eax
    }
    log_file << "arg was: " << std::hex << arg_value << std::endl;
    
    if(!next_is_nested_invoke){
        next_is_nested_invoke = 1;
        __asm {
            mov eax, [ebp+4]
            mov called_from, eax
        }
        called_from -= 5;
        log_file << "called_from: " << std::hex << (unsigned)called_from << std::endl;
        call_tgt = get_call_tgt(called_from);
        log_file << "call_tgt: " << std::hex << (unsigned)call_tgt << std::endl;
        arg_offset = getcnt();
        log_file << "arg_offset: " << std::hex << (unsigned)arg_offset << std::endl;
        __asm {
            mov eax, arg_offset
            add [ebp+8], eax
            mov eax, [ebp+8]
            push eax
            call call_tgt
        
            mov eax, x
        }
        log_file << "return value - outer call: " << std::hex << x << std::endl;
        __asm {    
            leave
            ret
        }
    } else {
        next_is_nested_invoke = 0;
        __asm {
            mov eax, [ebp-4]
            mov x, eax
        }
        log_file << "return value - inner call: " << std::hex << x << std::endl;
        __asm {
            leave
            ret
        }
    }
}


void setHook() {
    log_file << "setHook called" << std::endl;
	// HMODULE h = GetModuleHandle(L"<dll_with_func_to_hook_name>.dll");
	HMODULE h = GetModuleHandle(L"hme.exe");
	LPVOID JumpTo;

	if (h == NULL) {
		log_file << "can't find module\n";
		return;
	}

    original_func_address = (void*)0x401508;//(FUNC_PTR)GetProcAddress(h, "sub_401460");
    // Another option: original_func_address = (char*)h + <offset> if h == our_program.exe, for example.
	if (original_func_address == NULL) {
		// can't find function
        log_file << "could not find target function\n";
		return;
	}
    log_file << "original_func_address: " << (unsigned)original_func_address << std::endl;

	JumpTo = (FUNC_PTR)((char*)&funcHook - ((char*)original_func_address + 5)); // The "+5" part is for the offset to be calculated relatively to the address AFTER jmp
	memcpy(JmpOpcode + 1, &JumpTo, 0x4); // prepare the jmp opcode

	// save old bytes - save this arr globally if needed to be restored in hook_func
	memcpy(&OrigOpcode, (char*)original_func_address, 0x5); // override the first five bytes with jmp

	// override these bytes
	restore_hook();

	// save address to return to after hook. Can be used directly if hook is written in C.
	// to_return_address = (LPVOID)((char*)original_func_address); // can be changed to original_func_address+<some offset>
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
    // log_file << "DllMain called" << std::endl;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        // Sleep(20*1000);
		setHook();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}