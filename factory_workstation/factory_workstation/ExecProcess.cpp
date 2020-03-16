#include "stdafx.h"
#include "ExecProcess.h"


CExecProcess::CExecProcess(void)
{
	m_dwProcessId = 0;  
	m_execType = SYNC;  

	// 初始化管道句柄为空句柄  
	for (int i = 0; i < _countof(m_hPipes); i++)  { 
		for (int j = 0; j < _countof(m_hPipes[i]); j++)  {
			TRACE("PIPES %d-%d %d %d\n", i, j, _countof(m_hPipes), _countof(m_hPipes[i]));
			m_hPipes[i][j] = INVALID_HANDLE_VALUE; 
		}
	}
}

CExecProcess::CExecProcess(ExecType execType)
{  
    if (!Create(execType)) ClosePipes();  
}  

CExecProcess::~CExecProcess(void)
{
	ClosePipes();
}

BOOL CExecProcess::Create(ExecType execType)  
{  
	BOOL bRet = TRUE;  

	ClosePipes();
	bRet |= CreatePipes();  
	m_execType = execType;  
	return bRet;  
}  

BOOL CExecProcess::Execute(LPCTSTR Command)  
{  
	BOOL bRet = TRUE;  

	STARTUPINFO si;  
	ZeroMemory(&si,sizeof(si));  
	si.cb = sizeof(si);  
	si.hStdInput    = m_hPipes[pIn][Read];  
	si.hStdOutput   = m_hPipes[pOut][Write];  
	si.hStdError    = m_hPipes[pErr][Write];  
	si.hStdError    = m_hPipes[pOut][Write];   // WHG 将ERR输出到OUT中去
	si.wShowWindow = SW_HIDE;  
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;  

	PROCESS_INFORMATION pi;  

	TCHAR cmd[MAX_PATH+1000];  
	_tcscpy_s(cmd,_countof(cmd), Command);  

	bRet |= ::CreateProcess(NULL, cmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);  
	if (!bRet) {  
		ClosePipes();  
		return FALSE;  
	}

	m_dwProcessId = pi.dwProcessId;  
	// 关闭不要的读写句柄  
	::CloseHandle(m_hPipes[pIn][Read]);     m_hPipes[pIn][Read]     = INVALID_HANDLE_VALUE;  
	::CloseHandle(m_hPipes[pOut][Write]);   m_hPipes[pOut][Write]   = INVALID_HANDLE_VALUE;  
	::CloseHandle(m_hPipes[pErr][Write]);   m_hPipes[pErr][Write]   = INVALID_HANDLE_VALUE;  

	if(m_execType == SYNC) WaitForSingleObject(pi.hProcess, INFINITE);  
	::CloseHandle(pi.hProcess);  
	::CloseHandle(pi.hThread);  

	return bRet;  
}  

size_t CExecProcess::ReadLine(LPVOID lpBuffer, size_t len, char seq)  
{  
	if (!IsOk()) return 0;  

	DWORD totalRead = 0;  
	DWORD bytesRead;  
	char * buffer = new char[len + 1];  
	ZeroMemory(buffer,len + 1);  
	char ch;  
	do {  
		if (!::ReadFile(m_hPipes[pOut][Read], &ch, 1, &bytesRead, NULL)) break;  
		buffer[totalRead] = ch;  
		++totalRead;  
	} while (bytesRead == 1 && ch != seq && totalRead < len + 1);  

	if (totalRead > 0) strcpy_s((char*)lpBuffer, len, buffer);  
	delete[] buffer;  

	return totalRead;  
}  

size_t CExecProcess::WriteSome(LPVOID lpBuffer, size_t len)  
{  
	if (!IsOk()) return 0;  

	DWORD totalWritten = 0;  
	while (len > 0)  
	{  
		DWORD chunkWritten;  
		if (!::WriteFile(m_hPipes[pIn][Write], lpBuffer, len, &chunkWritten, NULL)) break;  
		if (!chunkWritten) break;  

		lpBuffer = (char *)lpBuffer + chunkWritten;  
		totalWritten += chunkWritten;  
		len -= chunkWritten;  
	}  

	return totalWritten;  
}  

BOOL CExecProcess::CreatePipes()  
{
	BOOL bRet = TRUE;  
	for (int i = 0; i < pOut + 1; ++i)  
	{  
		SECURITY_ATTRIBUTES security;  
		security.nLength = sizeof(security);  
		security.lpSecurityDescriptor = NULL;  
		security.bInheritHandle = TRUE;  
		bRet |= ::CreatePipe(&m_hPipes[i][Read], &m_hPipes[i][Write], &security, 0);  
	}  
	bRet |= ::SetHandleInformation(m_hPipes[pIn][Write], HANDLE_FLAG_INHERIT, 0);  
	bRet |= ::SetHandleInformation(m_hPipes[pOut][Read], HANDLE_FLAG_INHERIT, 0);  
	bRet |= ::SetHandleInformation(m_hPipes[pErr][Read], HANDLE_FLAG_INHERIT, 0);  
	return bRet;  
}  

void CExecProcess::ClosePipes()  
{  
	for (int i = 0; i < _countof(m_hPipes); ++i) {  
		for (int j = 0; j < _countof(m_hPipes[i]); ++j) {  
			if (m_hPipes[i][j] != INVALID_HANDLE_VALUE) {  
				::CloseHandle(m_hPipes[i][j]);  
				m_hPipes[i][j] = INVALID_HANDLE_VALUE;  
			}  
		}  
	}  
}  