#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h>
#include <tlhelp32.h>

#define STR_LEN   256
#define CMD_TOKEN_NUM   10

TCHAR ERROR_CMD[] = _T("'%s'은(는) 실행할 수 있는 프로그램이 아닙니다.\n");

int CmdReadTokenize(void);
int CmdProcessing(int tokenNum);
int CmdSnapShot(void);
TCHAR *StrLower(TCHAR *);
int KillProcess(void);

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM] [STR_LEN];
TCHAR seps[] = _T(" ,\t\n");

int _tmain(int argc, TCHAR * argv[])
{
    //한글 입력받기 위해
    _tsetlocale(LC_ALL, _T("KOREAN"));

    if (argc > 2)
    {
        for(int i = 1; i < argc; i++)
            _tcscpy(cmdTokenList[i-1], argv[i]);
        
        CmdProcessing(argc - 1);
    }

    DWORD isExit;
    while(1)
    {
        int tokenNum = CmdReadTokenize();

        if (tokenNum == 0)
            continue;
        
        isExit = CmdProcessing(tokenNum);
        if (isExit == TRUE)
        {
            _fputts(_T("명령어 처리를 종료합니다.\n"), stdout);
            break;
        }
    }
    return 0;
}

/**************************************************************************************************
 함수: TCHAR int CmdProcessing(void)
 기능: 명령어를 입력받아서 해당 명령어에 지정되어있는 기능을 수행한다.
        exit가 입력되면 true를 반환하고 이는 프로그램 종료로 이어진다.
**************************************************************************************************/

int CmdReadTokenize(void)
{
    TCHAR* token;

    _fputts(_T("Best command prompt>> "), stdout);
    _getts(cmdString);

    token = _tcstok(cmdString, seps);

    int tokenNum = 0;

    while (token != NULL)
    {
        _tcscpy(cmdTokenList[tokenNum++], StrLower(token));
        token = _tcstok(NULL, seps);
    }

    return tokenNum;
}

int CmdProcessing(int tokenNum)
{
    BOOL isRun;
    STARTUPINFO si = {0,};
    PROCESS_INFORMATION pi;
    TCHAR cmdStringWithOptions[STR_LEN] = {0,};
    TCHAR optString[STR_LEN] = {0,};
    si.cb = sizeof(si);
    
    if (!_tcscmp(cmdTokenList[0], _T("exit")))  return true;

    else if (!_tcscmp(cmdTokenList[0], _T("start"))) 
    {
        if (tokenNum>1)
        {
            for(int i = 1; i < tokenNum; i++)
                _stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);
                _stprintf(cmdStringWithOptions, _T("%s %s"), _T("CommandPrompt_4.exe"), optString);
        }

        else
            _stprintf(cmdStringWithOptions, _T("%s"), _T("CommandPrompt_4.exe"));
        
        isRun = CreateProcess(
            NULL, cmdStringWithOptions, NULL, NULL, TRUE,
            0, NULL, NULL, &si, &pi
        );
        
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    else if (!_tcscmp(cmdTokenList[0], _T("echo"))) 
    {
        for(int i = 1; i < tokenNum; i++)
            _stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);

        _tprintf(_T("echo message: %s\n"), optString);
    }

    else if (!_tcscmp(cmdTokenList[0], _T("lp"))) 
    {
        CmdSnapShot();
    }

    else if (!_tcscmp(cmdTokenList[0], _T("kp"))) 
    {
        KillProcess();
    }

    else if (!_tcscmp(cmdTokenList[0], _T("sort"))) 
    {
        if(!_tcscmp(cmdTokenList[1], _T(">")))
    {
        TCHAR* fName = cmdTokenList[2];
        SECURITY_ATTRIBUTES fileSec = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
        fileSec.bInheritHandle = TRUE;

        HANDLE hFile = CreateFile(
            fName, GENERIC_WRITE, FILE_SHARE_READ,
            &fileSec, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
        );

        si.hStdOutput = hFile;
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
        si.dwFlags |= STARTF_USESTDHANDLES;
        
        isRun = CreateProcess(
            NULL, cmdTokenList[0], NULL, NULL,
            TRUE, 0, NULL, NULL, &si, &pi
        );

        WaitForSingleObject(pi.hProcess, INFINITE);
    }

    else
    {
        isRun = CreateProcess(
            NULL, cmdTokenList[0], NULL, NULL,
            TRUE, 0, NULL, NULL, &si, &pi
        );

        WaitForSingleObject(pi.hProcess, INFINITE);
    }

    if (!isRun)
    {
        _tprintf(_T("can not open!\n"));
        return -1;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
    }

    else
    {
        _tcscpy(cmdStringWithOptions, cmdTokenList[0]);

        for(int i = 1; i < tokenNum; i++)
            _stprintf(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[i]);

        isRun = CreateProcess(
            NULL, cmdStringWithOptions, NULL, NULL, TRUE,
            0, NULL, NULL, &si, &pi
        );

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    return 0;
}

/**************************************************************************************************
 함수: TCHAR * StrLower(TCHAR *pStr)
 기능: 문자열 내에 존재하는 모든 대문자를 소문자로 변경한다.
        변경된 문자열의 포인터를 반환한다.
**************************************************************************************************/

TCHAR * StrLower(TCHAR *pStr)
{
    TCHAR *ret = pStr;
    while (*pStr)
    {
        if(_istupper(*pStr))
            *pStr = _totlower(*pStr);
        pStr++;
    }
    return ret;
}

int CmdSnapShot(void)
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE)
    {
        _tprintf(_T("CreateToolhelp32Snapshot error!\n"));
        return -1;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if(!Process32First(hProcessSnap, &pe32))
    {
        _tprintf(_T("Process32First error!\n"));
        CloseHandle(hProcessSnap);
        return -1;
    }

    do
    {
        _tprintf(_T("%25s %5d\n"), pe32.szExeFile, pe32.th32ProcessID);
    } while(Process32Next(hProcessSnap, &pe32));

    return 0;
}

int KillProcess()
{
    DWORD kProcessId;
    HANDLE hkProcess;
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL isKilled = FALSE;
    
    if(hProcessSnap == INVALID_HANDLE_VALUE)
    {
        _tprintf(_T("CreateToolhelp32Snapshot error!\n"));
        return -1;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if(Process32First(hProcessSnap, &pe32))
    {
        if (!_tcscmp(cmdTokenList[1], pe32.szExeFile))
        {
            kProcessId = pe32.th32ProcessID;
            hkProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, kProcessId);
            TerminateProcess(hkProcess, -1);
        }
        isKilled = TRUE;
    }
    
    else
    {
        _tprintf(_T("Process32First error!\n"));
        CloseHandle(hProcessSnap);
        return -1;
    }

    do
    {
        if (!_tcscmp(cmdTokenList[1], pe32.szExeFile))
        {
            kProcessId = pe32.th32ProcessID;
            hkProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, kProcessId);
            TerminateProcess(hkProcess, -1);
            isKilled = TRUE;
            break;
        }
    } while(Process32Next(hProcessSnap, &pe32));
    CloseHandle(hkProcess);
    CloseHandle(hProcessSnap);

    if(isKilled)
        return 0;

    _tprintf(_T("rewrite the process name!\n"));
    return -1;
}
