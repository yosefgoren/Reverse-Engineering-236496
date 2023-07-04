#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <Shlwapi.h>
#include <string>
int main(){
    // system("codes.exe ROBBER_CAPTURED JJ");
    PROCESS_INFORMATION pi;
    STARTUPINFOA Startup;
    ZeroMemory(&Startup, sizeof(Startup));
    ZeroMemory(&pi, sizeof(pi));
    std::string lpAppName = "codes.exe";
    const char* lpCmdLine = "client.exe DMSG";
    CreateProcessA((LPSTR)lpAppName.c_str(), (LPSTR)lpCmdLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &Startup, &pi)
    return 0;
}