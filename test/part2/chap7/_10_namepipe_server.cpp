#include <stdio.h>
#include <tchar.h>
#include <windows.h> 

#define BUF_SIZE 1024

int CommToClient(HANDLE);

int _tmain(int argc, TCHAR* argv[]) 
{
	LPTSTR pipeName = _T("\\\\.\\pipe\\simple_pipe"); 

	HANDLE hPipe;
	
	while(1)
	{
  	hPipe = CreateNamedPipe ( 
  		pipeName,            // 파이프 이름
  		PIPE_ACCESS_DUPLEX,       // 읽기,쓰기 모드 지정
  		PIPE_TYPE_MESSAGE |
  		PIPE_READMODE_MESSAGE | PIPE_WAIT,
  		PIPE_UNLIMITED_INSTANCES, // 최대 인스턴스 개수.
  		BUF_SIZE,                  // 출력버퍼 사이즈.
  		BUF_SIZE,                  // 입력버퍼 사이즈 
  		20000, // 클라이언트 타임-아웃  
  		NULL                    // 디폴트 보안 속성
  		);
  
  	if (hPipe == INVALID_HANDLE_VALUE) 
  	{
  		_tprintf( _T("CreatePipe failed")); 
  		return -1;
  	}
  
  	BOOL isSuccess; 
  	isSuccess = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 
  
  	if (isSuccess) 
  		CommToClient(hPipe);
  	else 
  		CloseHandle(hPipe); 
	}
	return 1; 
} 

int CommToClient(HANDLE hPipe)
{ 
	TCHAR fileName[MAX_PATH];
	TCHAR dataBuf[BUF_SIZE];

	BOOL isSuccess;
	DWORD fileNameSize;

	isSuccess = ReadFile ( 
		hPipe,        // 파이프 핸들 
		fileName,    // read 버퍼 지정.
		MAX_PATH * sizeof(TCHAR), // read 버퍼 사이즈 
		&fileNameSize,  // 수신한 데이터 크기
		NULL);       

	if (!isSuccess || fileNameSize == 0) 
	{
		_tprintf( _T("Pipe read message error! \n") );
		return -1; 
	}

	FILE * filePtr = _tfopen(fileName, _T("r") );

	if(filePtr == NULL)
	{
		_tprintf( _T("File open fault! \n") );
		return -1; 
	}

	DWORD bytesWritten = 0;
	DWORD bytesRead = 0;

	while( !feof(filePtr) )
	{
		bytesRead = fread(dataBuf, 1, BUF_SIZE, filePtr);

		WriteFile ( 
			hPipe,				// 파이프 핸들
			dataBuf,			// 전송할 데이터 버퍼  
			bytesRead,		// 전송할 데이터 크기 
			&bytesWritten,	// 전송된 데이터 크기 
			NULL);	

       if (bytesRead != bytesWritten)
		{
		    _tprintf( _T("Pipe write message error! \n") );
		    break; 
		}
	}

	FlushFileBuffers(hPipe); 
	DisconnectNamedPipe(hPipe); 
	CloseHandle(hPipe); 
	return 1;
}
