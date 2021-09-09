// 두 변수의 메모리 공간상의 거리를 계산하는 코드
//32비트식 처리

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

UINT CalDistance(UINT a, UINT b)
{
    return a - b;
}

// 만약 64비트 기반으로 만든다면..
/*
UINT64 CalDistance(UINT64 a, UINT64 b)
{
    return a - b;
}
*/ 
int main()
{
    INT val1 = 10;
    INT val2 = 20;

    _tprintf(_T("Position %u, %u\n"), (UINT) &val1, (UINT) &val2);
    _tprintf(_T("distance : %u\n"), CalDistance((UINT) &val1, (UINT) &val2));

    return 0;
}
