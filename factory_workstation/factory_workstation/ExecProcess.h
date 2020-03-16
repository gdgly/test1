#pragma once
class CExecProcess
{
public:
    enum ExecType { ASYNC, SYNC };  
	CExecProcess(void);
    CExecProcess(ExecType execType);  
	~CExecProcess(void);

public:  
    // 管道类型  
    enum PipeType { pIn, pOut, pErr };  
    // 读写方式  
    enum Direction { Read, Write };  
    // 程序执行的方式  

    // 创建一个指定执行方式的进程类  
    BOOL Create(ExecType execType);  
    // 执行脚本  
    BOOL Execute(LPCTSTR Command);  
    // 进程类是否正常  
    BOOL IsOk() { return m_hPipes[pIn][Write] != INVALID_HANDLE_VALUE;}  
    // 获取进程id  
    DWORD GetPid() { return m_dwProcessId; }  
    // 从管道读取一行  
    size_t ReadLine(LPVOID lpBuffer,size_t len,char seq = '\n');  
    // 向管道写入  
    size_t WriteSome(LPVOID lpBuffer, size_t len);  

protected:  
    BOOL CreatePipes();  
    void ClosePipes();  
private:  
    DWORD m_dwProcessId;  
    HANDLE m_hPipes[3][2];  
    ExecType m_execType; 
};

