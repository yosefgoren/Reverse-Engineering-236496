#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.
#include <string.h>
int main(){
    // system("codes.exe ROBBER_CAPTURED JJ");
    PROCESS_INFORMATION pi;
    STARTUPINFOA Startup;
    ZeroMemory(&Startup, sizeof(Startup));
    ZeroMemory(&pi, sizeof(pi));
    std::string lpAppName = "client.exe";
    return 0;
}