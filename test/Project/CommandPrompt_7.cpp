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
void TypeTextFile(void);

int cntFiles = 0;
BOOL CopyDir(WIN32_FIND_DATA fData, TCHAR* source, TCHAR* dest);
BOOL XCOPY(TCHAR* source, TCHAR* dest);

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

    else if(!_tcscmp(cmdTokenList[0], _T("type")))
    {
        TypeTextFile();
    }

    else if(!_tcscmp(cmdTokenList[0], _T("xcopy")))
    {
        XCOPY(cmdTokenList[1], cmdTokenList[2]);
        _tprintf(_T("%d개의 파일이 복사됐습니다.\n"), cntFiles);
        cntFiles = 0;
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


// 프롬프트 6차
// 명령어 1 : type => 텍스트 파일 내용 출력
// 명령어 2 : type text.txt | sort => 파이프

void TypeTextFile()
{
    TCHAR cmdStringWithOption[STR_LEN] = {0,};
    BOOL isRun;

    if(!_tcscmp(cmdTokenList[2], _T("|")))
    {
        HANDLE hReadPipe, hWritePipe;

        SECURITY_ATTRIBUTES pipeSA = {
            sizeof(SECURITY_ATTRIBUTES), NULL, TRUE
        };

        CreatePipe(&hReadPipe, &hWritePipe, &pipeSA, 0);

        // process type을 위한 선언
        STARTUPINFO siType = {0, };
        PROCESS_INFORMATION piType;
        siType.cb = sizeof(siType);

        siType.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        siType.hStdError = GetStdHandle(STD_ERROR_HANDLE);
        siType.hStdOutput = hWritePipe;
        siType.dwFlags |= STARTF_USESTDHANDLES;

        _tcscpy(cmdStringWithOption, cmdTokenList[0]);
        _stprintf(cmdStringWithOption, _T("%s %s"),
        cmdStringWithOption, cmdTokenList[1]);

        isRun = CreateProcess(NULL, cmdStringWithOption, NULL, NULL, TRUE,
         0, NULL, NULL, &siType, &piType);

        CloseHandle(piType.hThread);
        CloseHandle(hWritePipe);

        // process sort를 위한 선언
        STARTUPINFO siSort = {0, };
        PROCESS_INFORMATION piSort;
        siSort.cb = sizeof(siSort);

        siSort.hStdInput = hReadPipe;
        siSort.hStdError = GetStdHandle(STD_ERROR_HANDLE);
        siSort.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        siSort.dwFlags |= STARTF_USESTDHANDLES;

        isRun = CreateProcess(NULL, cmdTokenList[3], NULL, NULL, TRUE,
         0, NULL, NULL, &siSort, &piSort);

        CloseHandle(piSort.hThread);
        CloseHandle(hReadPipe);

        WaitForSingleObject(piType.hProcess, INFINITE);
        WaitForSingleObject(piSort.hProcess, INFINITE);

        CloseHandle(piType.hProcess);
        CloseHandle(piSort.hProcess);
    }

    else
    {
        _tcscpy(cmdStringWithOption, cmdTokenList[0]);
        _stprintf(cmdStringWithOption, _T("%s %s"),
        cmdStringWithOption, cmdTokenList[1]);

        STARTUPINFO si = {0,};
        PROCESS_INFORMATION pi;
        si.cb = sizeof(si);

        isRun = CreateProcess(NULL, cmdStringWithOption, NULL, NULL,
        FALSE, 0, NULL, NULL, &si, &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

BOOL CopyDir(WIN32_FIND_DATA fData, TCHAR* source, TCHAR* dest)
{
    BOOL isSuccess = NULL;

    if(!_tcscmp(fData.cFileName, _T(".")) || !_tcscmp(fData.cFileName, _T("..")))
    {

    }

    else if(fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        TCHAR s_File[MAX_PATH];
        TCHAR d_File[MAX_PATH];
        _stprintf(s_File, _T("%s\\%s"), source, fData.cFileName);
        _stprintf(d_File, _T("%s\\%s"), dest, fData.cFileName);

        CreateDirectory(d_File, NULL);
        XCOPY(s_File, d_File);
    }

    else{
        TCHAR s_File[MAX_PATH];
        TCHAR d_File[MAX_PATH];
        _tcscpy(s_File, source);
        _tcscpy(d_File, dest);
        _stprintf(s_File, _T("%s\\%s"), s_File, fData.cFileName);
        _tprintf(_T("%s\n"), s_File);
        _stprintf(d_File, _T("%s\\%s"), d_File, fData.cFileName);

        isSuccess = CopyFile(s_File, d_File, FALSE);
        if(isSuccess == 0)
            return FALSE;

        cntFiles++;    
    }
    return TRUE;
}

BOOL XCOPY(TCHAR* s_File, TCHAR* d_File)
{
    WIN32_FIND_DATA fData;
    BOOL isSuccess = NULL;

    TCHAR firstFFStr[MAX_PATH];
    HANDLE hFind;

    _stprintf(firstFFStr, _T("%s\\%s"), _T("*"));
    hFind = FindFirstFile(firstFFStr, &fData);

    if(hFind == INVALID_HANDLE_VALUE)
        return FALSE;

    else
        CopyDir(fData, s_File, d_File);
    
    while(1)
    {
        if(!FindNextFile(hFind, &fData))
        {
            break;
        }

        else
        {
            isSuccess = CopyDir(fData, s_File, d_File);
            if(isSuccess == FALSE)
                break;
        }
    }

    FindClose(hFind);
    return TRUE;
}