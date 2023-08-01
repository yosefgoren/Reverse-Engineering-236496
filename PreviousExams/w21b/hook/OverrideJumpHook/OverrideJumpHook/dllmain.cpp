#include "pch.h"
#include <Windows.h>
#include <stdio.h>
using namespace std;

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

int num_additional_params;
bool hook_next_invoke = true;
int idx;
int fofset;

char* secret_func_addr = (char*)0x401796;

// Hook function. Might use helper functions in C, i.e. void _stdcall helper(int num) {}
__declspec(naked) void funcHook() {
	// Restore overriden bytes
	//remove_hook();
	__asm {
		mov eax, [ebp+8]
		mov num_additional_params, eax
	}
	log_file << "Hook seen " << num_additional_params << " additional_params...\n";

	if(hook_next_invoke){
		hook_next_invoke = false;
		for(idx = 0; idx < num_additional_params; ++idx){
			fofset = 0xC+idx*4;
			__asm {
				mov eax, fofset
				add eax, ebp
				add [eax], 2
			}
		}
		__asm {
			leave
			jmp secret_func_addr
		}
		// Assembly part. Should call restore_hook somewhere inside, can call original_func_addr
	} else {
		hook_next_invoke = true;
		__asm {
			mov eax, [ebp-4]
			leave
			ret
		}
	}
}

void setHook() {
    log_file << "starting setHook\n";
	// HMODULE h = GetModuleHandle(L"<dll_with_func_to_hook_name>.dll");
	HMODULE h = GetModuleHandle(L"hme.exe");
	LPVOID JumpTo;

	if (h == NULL) {
        log_file << "can't find module\n";
		return;
	}

	original_func_address = (FUNC_PTR)0x401815;//(FUNC_PTR)GetProcAddress(h, "secret_function");
    log_file << "found func at: " << hex << original_func_address << endl;
	// Another option: original_func_address = (char*)h + <offset> if h == our_program.exe, for example.
	if (original_func_address == NULL) {
        log_file << "can't find function\n";
		return;
	}

	JumpTo = (FUNC_PTR)((char*)&funcHook - ((char*)original_func_address + 5)); // The "+5" part is for the offset to be calculated relatively to the address AFTER jmp
	memcpy(JmpOpcode + 1, &JumpTo, 0x4); // prepare the jmp opcode

	// save old bytes - save this arr globally if needed to be restored in hook_func
	memcpy(&OrigOpcode, (char*)original_func_address, 0x5); // override the first five bytes with jmp

	// override these bytes
	restore_hook();
	log_file << "finished restoring hook.\n";

	// save address to return to after hook. Can be used directly if hook is written in C.
	// to_return_address = (LPVOID)((char*)original_func_address); // can be changed to original_func_address+<some offset>
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