#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#define BUF_SIZE 1024

int _tmain(int argc, TCHAR* argv[])
{
    TCHAR value[BUF_SIZE];

    if(GetEnvironmentVariable(_T("GOOD"), value, BUF_SIZE) > 0)
        _tprintf(_T("[%s = %s]\n"), _T("GOOD"), value);

    if(GetEnvironmentVariable(_T("HEY"), value, BUF_SIZE) > 0)
        _tprintf(_T("[%s = %s]\n"), _T("HEY"), value);
    
    if(GetEnvironmentVariable(_T("BIG"), value, BUF_SIZE) > 0)
        _tprintf(_T("[%s = %s]\n"), _T("BIG"), value);
    
    Sleep(10000);

    return 0;
}