#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, TCHAR * argv[])
{
    HANDLE hMailSlot;

    TCHAR message[50];
    DWORD bytesWritten;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    hMailSlot = CreateFile(
        SLOT_NAME,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        &sa,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if(hMailSlot == INVALID_HANDLE_VALUE)
    {
        _fputts(_T("Unable to create Mailslot!\n"), stdout);
        return 1;
    }

    _tprintf(_T("Inheritable Handle : %d\n"), hMailSlot);

    FILE* file = _tfopen(_T("InheritableHandle.txt"), _T("wt"));
    _ftprintf(file, _T("%d"), hMailSlot);
    fclose(file);

    STARTUPINFO si = {0,};
    PROCESS_INFORMATION pi;
    si.cb = sizeof(si);

    TCHAR command[] = _T("_6_MailSender2_2.exe");

    CreateProcess(
        NULL,
        command,
        NULL,
        NULL,
        TRUE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &pi
    );

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