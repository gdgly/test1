#pragma once
class CExecProcess
{
public:
    enum ExecType { ASYNC, SYNC };  
	CExecProcess(void);
    CExecProcess(ExecType execType);  
	~CExecProcess(void);

public:  
    // �ܵ�����  
    enum PipeType { pIn, pOut, pErr };  
    // ��д��ʽ  
    enum Direction { Read, Write };  
    // ����ִ�еķ�ʽ  

    // ����һ��ָ��ִ�з�ʽ�Ľ�����  
    BOOL Create(ExecType execType);  
    // ִ�нű�  
    BOOL Execute(LPCTSTR Command);  
    // �������Ƿ�����  
    BOOL IsOk() { return m_hPipes[pIn][Write] != INVALID_HANDLE_VALUE;}  
    // ��ȡ����id  
    DWORD GetPid() { return m_dwProcessId; }  
    // �ӹܵ���ȡһ��  
    size_t ReadLine(LPVOID lpBuffer,size_t len,char seq = '\n');  
    // ��ܵ�д��  
    size_t WriteSome(LPVOID lpBuffer, size_t len);  

protected:  
    BOOL CreatePipes();  
    void ClosePipes();  
private:  
    DWORD m_dwProcessId;  
    HANDLE m_hPipes[3][2];  
    ExecType m_execType; 
};

