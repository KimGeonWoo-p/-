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
    TCHAR messageBox1[50];
    DWORD bytesRead1;

    //★★
    hMailSlot1 = CreateMailslot(
        SlOT_NAME_1,
        0,
        MAILSLOT_WAIT_FOREVER,
        NULL
    );
    
    if(hMailSlot1==INVALID_HANDLE_VALUE)
    {
        _fputts(_T("Unable to create mailslot!\n"), stdout);
        return 1;
    }

    Sleep(10000);

    hMailSlot = CreateFile(
        SlOT_NAME,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if(hMailSlot==INVALID_HANDLE_VALUE)
    {
        _fputts(_T("Unable to create mailslot!\n"), stdout);
        return 1;
    }

    _fputts(_T("******** MESSAGE ********\n"), stdout);

    while(1)
    {
        _fputts(_T("MY CMD >> "), stdout);
        _fgetts(message, sizeof(message)/sizeof(TCHAR),stdin);

        if(!WriteFile(hMailSlot, message,
        _tcsclen(message)*sizeof(TCHAR), &bytesWritten, NULL))
        {
            _fputts(_T("Unable to write!"), stdout);
            break;
        }

        if(!ReadFile(hMailSlot1, messageBox1, sizeof(TCHAR)*50, &bytesRead1, NULL))
        {
            _fputts(_T("Unable to read!"), stdout);
            CloseHandle(hMailSlot1);
            return 1;
        }

        if (!_tcsncmp(messageBox1, _T("exit"), 4))
        {
            _fputts(_T("Good Bye!"), stdout);
            break;
        }

        messageBox1[bytesRead1/sizeof(TCHAR)]=0;
        _fputts(messageBox1, stdout);
    }

    CloseHandle(hMailSlot);

    CloseHandle(hMailSlot1);
    return 0;
}