#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
    SetEnvironmentVariable(_T("GOOD"), _T("MORNING"));
    SetEnvironmentVariable(_T("HEY"), _T("HO!"));
    SetEnvironmentVariable(_T("BIG"), _T("BANG"));

    STARTUPINFO si = {0,};
    PROCESS_INFORMATION pi = {0,};
    si.cb = sizeof(si);

    CreateProcess(
        NULL, _T("_13_EnviChild"), NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,
        NULL, // 부모프로세스의 환경변수 등록
        NULL, &si, &pi
    );

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}