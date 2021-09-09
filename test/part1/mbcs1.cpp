#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

extern int wmain( int, wchar_t** )
{
    TCHAR str[] = _T("1234567");
    int size = sizeof(str);
    printf("string lenfth : %d\n", size);
    return 0;
}

int main()
{
    struct Args
    {
        int n;
        wchar_t** p;

        ~Args() {  if( p != 0 ) { ::LocalFree( p ); } }
        Args(): p(  ::CommandLineToArgvW( ::GetCommandLine(), &n ) ) {}
    };

    Args    args;

    if( args.p == 0 )
    {
        return EXIT_FAILURE;
    }
    return wmain( args.n, args.p );
}