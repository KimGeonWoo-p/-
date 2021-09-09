#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define     SlOT_NAME    _T("\\\\.\\mailslot\\mailbox")
#define     SlOT_NAME_1    _T("\\\\.\\mailslot\\mailbox1")

int _tmain(int argc, TCHAR* argv[])
{
    HANDLE hMailSlot;
    TCHAR message[50];
    DWORD bytesWritten;

    HANDLE hMailSlot1;
    TCHAR messageBox[50];
    DWORD bytesRead;

    //★★
    hMailSlot = CreateMailslot(
        SlOT_NAME,
        0,
        MAILSLOT_WAIT_FOREVER,
        NULL
    );
    
    if(hMailSlot==INVALID_HANDLE_VALUE)
    {
        _fputts(_T("Unable to create mailslot!\n"), stdout);
        return 1;
    }

    hMailSlot1 = CreateFile(
        SlOT_NAME_1,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if(hMailSlot1==INVALID_HANDLE_VALUE)
    {
        _fputts(_T("Unable to create mailslot!\n"), stdout);
        return 1;
    }
    
    _fputts(_T("******** MESSAGE ********\n"), stdout);

    while(1)
    {
        _fputts(_T("MY CMD >> "), stdout);
        _fgetts(message, sizeof(message)/sizeof(TCHAR),stdin);

        if(!WriteFile(hMailSlot1, message,
        _tcsclen(message)*sizeof(TCHAR), &bytesWritten, NULL))
        {
            _fputts(_T("Unable to write!"), stdout);
            break;
        }

        if(!ReadFile(hMailSlot, messageBox, sizeof(TCHAR)*50, &bytesRead, NULL))
        {
            _fputts(_T("Unable to read!"), stdout);
            CloseHandle(hMailSlot);
            return 1;
        }

        if (!_tcsncmp(messageBox, _T("exit"), 4))
        {
            _fputts(_T("Good Bye!"), stdout);
            break;
        }

        messageBox[bytesRead/sizeof(TCHAR)]=0;
        _fputts(messageBox, stdout);
    }

    CloseHandle(hMailSlot);

    CloseHandle(hMailSlot1);
    return 0;
}