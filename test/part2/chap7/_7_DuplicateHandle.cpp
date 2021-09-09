#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
    HANDLE hProcess;
    TCHAR cmdString[1024];

    DuplicateHandle(
        GetCurrentProcess(), GetCurrentProcess(),
        GetCurrentProcess(), &hProcess, 0,
        TRUE, DUPLICATE_SAME_ACCESS
    );


    //_sprintf(cmdString, _T("%s %u"), _T("_8_ChildProcess.exe"), (unsigned) hProcess);
    // 이때 HANDLE 타입을 unsigned 형태로 형변환하는 과정에서 에러가 난다... 왜지..?
    _stprintf(cmdString, _T("%s %p"), _T("_8_ChildProcess.exe"), hProcess);

    STARTUPINFO si = {0,};
    PROCESS_INFORMATION pi;

    si.cb = sizeof(si);

    BOOL isSuccess = CreateProcess(
        NULL, cmdString, NULL, NULL, TRUE,
        CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi
    );

    if(isSuccess == FALSE)
    {
        _tprintf(_T("CreateProcess failed\n"));
        return -1;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    _tprintf(_T("[Parent Process]\n"));
    _tprintf(_T("Ooooooooooooooooooopps!\n"));

    return 0;
}