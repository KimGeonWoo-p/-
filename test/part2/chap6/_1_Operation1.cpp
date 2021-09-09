#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
    STARTUPINFO si = {0,};
    PROCESS_INFORMATION pi;
    si.cb = sizeof(si);
    TCHAR command[] = _T("_2_Operation2.exe");

    CreateProcess (
        NULL, command, NULL, NULL,
        TRUE, 0, NULL, NULL, &si, &pi
    );

    while(1)
    {
        for(DWORD i = 0; i < 10000; i++)
        {
            for(DWORD i = 0; i < 10000; i++); //BUSY WAITING
        }
        //=> 루프를 도는 동안 프로세스가 멈추어야함. 
        // 그렇다고 sleep을 넣는다면 프로세스는 blocked상태가 됨
        _fputts(_T("_1_Operation1.exe\n"), stdout);
    }
    return 0;
}