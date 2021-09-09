#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR * argv[])
{
    HANDLE hMailSlot;

    TCHAR message[50];
    DWORD bytesWritten;

    FILE* file = _tfopen(_T("InheritableHandle.txt"), _T("rt"));
    _ftscanf(file, _T("%d"), &hMailSlot);
    fclose(file);

    _tprintf(_T("Inheritable Handle : %d\n"), hMailSlot);

    STARTUPINFO si = {0,};
    PROCESS_INFORMATION pi;
    si.cb = sizeof(si);

    while(1)
    {
        _fputts(_T("My Cmd >> "), stdout);
        _fgetts(message, sizeof(message) / sizeof(TCHAR), stdin);

        if(!WriteFile(
            hMailSlot,
            message,
            _tcsclen(message) * sizeof(TCHAR),
            &bytesWritten,
            NULL
        ))
        {
            _fputts(_T("Unable to Write!\n"), stdout);
            _gettchar();
            CloseHandle(hMailSlot);
            return 1;
        }

        if(!_tcscmp(message, _T("exit")))
        {
            _fputts(_T("Good bye!"), stdout);
            break;
        }
    }
    CloseHandle(hMailSlot);
    return 0;
}