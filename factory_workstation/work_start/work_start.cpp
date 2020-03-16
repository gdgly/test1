// work_start.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
//#include "afxwin.h"
#include "atlconv.h"
#include "shellapi.h"
#include <windows.h>

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
using namespace std;

class ChangePath
{
public:
	ChangePath()
	{
		size_t len = 0;
		char sz[2048*10] = {};
		getenv_s(&len, sz,2048*10,"PATH");
		m_szPath = sz;

		char exeFullPath[MAX_PATH]; // Full path   
		string strPath = "";   
		GetModuleFileName(NULL,exeFullPath,MAX_PATH);   
		strPath=(string)exeFullPath;    // Get full path of the file   
		int pos = strPath.find_last_of('\\', strPath.length());   
		strPath=strPath.substr(0, pos); 
		strPath+="\\bluesite_lib";

		string szAddPath = strPath;//"";//你的dll所在的绝对路径，使用“;”隔开
		string szNewPath = m_szPath + ";";
		szNewPath += szAddPath;
		_putenv_s("PATH",szNewPath.c_str());
	}
	~ChangePath()
	{
		_putenv_s("PATH",m_szPath.c_str());
	}
private:
	string m_szPath;
};

ChangePath changepath;//RAII修改Path环境变量

void LoadInstance(void *param)
{
	USES_CONVERSION;
	PROCESS_INFORMATION pi;
	STARTUPINFOA si = {sizeof(si)};
	std::string exename = "factory_workstation.exe";
	CreateProcessA(exename.c_str(),NULL,NULL,NULL, FALSE, 0, NULL, NULL, &si, &pi);
}

int _tmain(int argc, _TCHAR* argv[])
{
	printf("hello!\n");
	LoadInstance(NULL);
	return 1;
}

