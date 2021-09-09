#include <stdio.h>
#include <tchar.h>
#include <windows.h>


#include <stdio.h>
#include <windows.h>
#include <tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
	STARTUPINFO si={0,};
	PROCESS_INFORMATION pi;
	
	si.cb=sizeof(si);

	TCHAR command[]=_T("_5_KernerlObjProb2.exe");

	CreateProcess(NULL,     // 프로세스 생성.
			      command,
				  NULL,
				  NULL, 
				  TRUE, 
				  0, 
				  NULL, 
				  NULL, 
				  &si,
				  &pi
	);  //CreateProcess

	_tprintf(_T("End Process!"));
	CloseHandle(pi.hProcess);

	return 0;
}
