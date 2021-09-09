#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
    STARTUPINFO si = {0,};
    PROCESS_INFORMATION pi;
    si.cb = sizeof(si);

    TCHAR command[] = _T("_2_Operation2.exe");

    CreateProcess(
        nullptr, command, nullptr,nullptr, TRUE,
        0, nullptr, nullptr, &si, &pi
    );

    DWORD timing = 9;
    while(1)
    {
        for(DWORD i = 0; i < 10000; i++)
            for(DWORD i = 0; i < 10000; i++)

        _fputts(_T("Parent\n"), stdout);

        timing ++;
        if(timing == 2)
            SetPriorityClass(pi.hProcess, NORMAL_PRIORITY_CLASS);
    }
    return 0;
}