#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
    STARTUPINFO si1 = {0,};
    STARTUPINFO si2 = {0,};

    PROCESS_INFORMATION pi1;
    PROCESS_INFORMATION pi2;

    DWORD return_val1;
    DWORD return_val2;

    TCHAR command1[] = _T("_3_PartAdder.exe 1 5");
    TCHAR command2[] = _T("_3_PartAdder.exe 6 10");

    DWORD sum = 0;
    
    si1.cb = sizeof(si1);
    si2.cb = sizeof(si2);
    
    CreateProcess( NULL, command1,
        NULL, NULL, TRUE,
        0, NULL, NULL,
        &si1, &pi1
    );
    
    CreateProcess( NULL, command2,
        NULL, NULL, TRUE,
        0, NULL, NULL,
        &si2, &pi2
    );

    CloseHandle(pi2.hThread);
    CloseHandle(pi1.hThread);
    
    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    // 이 두 줄이 실행되기 이전에 두 개의 자식 프로세스가 종료코드를 반황하여 이미 종료한 상태여야함.
    GetExitCodeProcess(pi1.hProcess, &return_val1);
    GetExitCodeProcess(pi2.hProcess, &return_val2);

    if(return_val1 == -1 || return_val2 == -1)
        return -1;

    _tprintf(_T("val1 : %d\n"), return_val1);
    _tprintf(_T("val2 : %d\n"), return_val2);

    sum += return_val1;
    sum += return_val2;

    _tprintf(_T("total : %d\n"), sum);

    CloseHandle(pi1.hProcess);
    CloseHandle(pi2.hProcess);

    return 0;
}