#include <stdio.h>
#include <tchar.h>
#include <windows.h>

UINT_PTR CalDistance(UINT_PTR a, UINT_PTR b)
{
    return a - b;
}

int _tmain()
{
    INT val1 = 10;
    INT val2 = 20;

    _tprintf(_T("Position %u, %u\n"), (UINT_PTR) &val1, (UINT_PTR) &val2);
    _tprintf(_T("distance : %u\n"), CalDistance((UINT_PTR) &val1, (UINT_PTR) &val2));

    return 0;
}
