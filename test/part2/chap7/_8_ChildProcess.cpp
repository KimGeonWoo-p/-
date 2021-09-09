#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{   
    // HANDLE hParent = (HANDLE) _ttoi(argv[1]);
    // HANDLE 타입을 unsigned로 형변환하는 과정에서 오류가 났다.
    // HANDLE 타입은 void* 형태로 선언된다.
    // 그리하여 나는 %p의 형태로 hProcess를 자식프로세스로 전달했다.
    HANDLE hParent = (HANDLE) argv[1];
    DWORD isSuccess = WaitForSingleObject(hParent, INFINITE);

    _tprintf(_T("[Child Process]\n"));
    if(isSuccess == FALSE)
    {
        _tprintf(_T("WAIT_FAILED returned!"));
        Sleep(10000);
        return -1;
    }

    else
    {
        _tprintf(
            _T("General Lee said, \"Don't inform ")
            _T("the enemy my death\"")
        );
        Sleep(10000);
        return 0;
    }
}