#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h>

#define STR_LEN   256
#define CMD_TOKEN_NUM   10

TCHAR ERROR_CMD[] = _T("'%s'은(는) 실행할 수 있는 프로그램이 아닙니다.\n");

int CmdProcessing(void);
TCHAR *StrLower(TCHAR *);

int _tmain(int args, TCHAR * argv[])
{
    //한글 입력받기 위해
    _tsetlocale(LC_ALL, _T("KOREAN"));

    DWORD isExit;
    while(1)
    {
        isExit = CmdProcessing();
        if(isExit == true)
        {
            _tprintf(_T("명령어 처리를 종료합니다.\n"));
            break;
        }
    }
    return 0;
}

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");

/**************************************************************************************************
 함수: TCHAR int CmdProcessing(void)
 기능: 명령어를 입력받아서 해당 명령어에 지정되어있는 기능을 수행한다.
        exit가 입력되면 true를 반환하고 이는 프로그램 종료로 이어진다.
**************************************************************************************************/

int CmdProcessing(void)
{
    _fputts(_T("Best command prompt>> "), stdout);
    _getts(cmdString);
    TCHAR* token = _tcstok(cmdString, seps);
    int tokenNum = 0;
    while (token != NULL)
    {
        _tcscpy(cmdTokenList[tokenNum++], StrLower(token));
        token = _tcstok(NULL, seps);
    }

    if (!_tcscmp(cmdTokenList[0], _T("exit")))  return true;
    else if (!_tcscmp(cmdTokenList[0], _T("추가되는 명령어 1"))) {}
    else if (!_tcscmp(cmdTokenList[0], _T("추가되는 명령어 2"))) {}
    else _tprintf(ERROR_CMD, cmdTokenList[0]);
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