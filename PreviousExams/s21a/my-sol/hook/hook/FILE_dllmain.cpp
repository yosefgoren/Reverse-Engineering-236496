#include "pch.h"
#include <Windows.h>
#include <string>
#include <stdio.h>
std::string strprintf(const char* fmt, ...) noexcept{
    char buf[1000];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    return std::string(buf);
}

// Add additional includes if needed
using namespace std;

// for logging. format: log_file << ... << std::endl;
FILE* log_file = fopen("log.txt", "w");

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
int is_nested_invoke = 0;

int _stdcall getcnt(){
    cnt++;
    fprintf(log_file, "cnt: %d\n", cnt);
    return cnt;
}

char* _stdcall get_call_tgt(char* call) {
    if(call[0] != 0xE8) {
        return NULL;
    }
    char* tgt = call + 5 + *(int*)(call + 1);
    return tgt;
}

// Hook function. Might use helper functions in C, i.e. void _stdcall helper(int num) {}
__declspec(naked) void funcHook() {
    if(!is_nested_invoke){
        is_nested_invoke = 1;
        char* called_from;
        __asm {
            mov called_from, ebp
        }
        called_from -= 5;
        char* call_tgt = get_call_tgt(called_from);
        fprintf(log_file, "call_tgt: %x\n", call_tgt);
        int arg_offset = getcnt();
        // log_file << "arg_offset: " << strprintf("%x\n", arg_offset);
        fprintf(log_file, "arg_offset: %x\n", int(arg_offset));
        __asm {
            mov eax, arg_offset
            add [ebp+8], eax
            call call_tgt

            mov eax, [ebp-4]
            leave
            ret
        }
    } else {
        is_nested_invoke = 0;
        __asm {
            mov eax, [ebp-4]
            leave
            ret
        }
    }
}


void setHook() {
    if(log_file == NULL){
        return;
    }
    fprintf(log_file, "setHook called\n");
	// HMODULE h = GetModuleHandle(L"<dll_with_func_to_hook_name>.dll");
	HMODULE h = GetModuleHandle(L"hme.exe");
	LPVOID JumpTo;

	if (h == NULL) {
		fprintf(log_file, "can't find module\n");
		return;
	}

    original_func_address = (void*)0x401508;//(FUNC_PTR)GetProcAddress(h, "sub_401460");
    // Another option: original_func_address = (char*)h + <offset> if h == our_program.exe, for example.
	if (original_func_address == NULL) {
		// can't find function
        fprintf(log_file, "could not find target function\n");
		return;
	}
    fprintf(log_file, "original_func_address: %x\n", (unsigned int)original_func_address);

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
		setHook();
        break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}