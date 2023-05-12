echo int system(const char*); int main(){system(%1);} > tmp.c
tcc tmp.c -o challenge.exe
copy challenge.exe ..\..\..\..\..\challenge.exe