
#include "stdafx.h"
#pragma  comment (lib, "ws2_32.lib")
 
#define ICMP_ECHOREPLY	0  // ICMP回复应答
#define ICMP_ECHOREQ	8  // ICMP回应请求
#define REQ_DATASIZE 32		// 请求数据报大小
 
// 定义IP首部格式
typedef struct _IPHeader
{
	u_char  VIHL;			// 版本和首部长度
	u_char	ToS;			// 服务类型
	u_short	TotalLen;		// 总长度
	u_short	ID;			// 标识号
	u_short	Frag_Flags;		// 段偏移量
	u_char	TTL;			// 生存时间
	u_char	Protocol;		// 协议
	u_short	Checksum;		// 首部校验和
	struct	in_addr SrcIP;	// 源IP地址
	struct	in_addr DestIP;	// 目的地址
}IPHDR, *PIPHDR;
 
 
// 定义ICMP首部格式
typedef struct _ICMPHeader
{
	u_char	Type;			// 类型
	u_char	Code;			// 代码
	u_short	Checksum;		// 首部校验和
	u_short	ID;				// 标识
	u_short	Seq;			// 序列号
	char	Data;			// 数据
}ICMPHDR, *PICMPHDR;
 
// 定义ICMP回应请求
typedef struct _ECHOREQUEST
{
	ICMPHDR icmpHdr;
	DWORD	dwTime;
	char	cData[REQ_DATASIZE];
}ECHOREQUEST, *PECHOREQUEST;
 
// 定义ICMP回应答复 
typedef struct _ECHOREPLY
{
	IPHDR	ipHdr;
	ECHOREQUEST	echoRequest;
	char    cFiller[256];
}ECHOREPLY, *PECHOREPLY;
 
// 计算校验和
u_short checksum(u_short *buffer, int len)
{
	register int nleft = len;
	register u_short *w = buffer;
	register u_short answer;
	register int sum = 0;
    // 使用32bit的累加器，进行16bit的反馈计算
	while( nleft > 1 )  {
		sum += *w++;
		nleft -= 2;
	}
	// 补全奇数位
	if( nleft == 1 ) {
		u_short	u = 0;
 
		*(u_char *)(&u) = *(u_char *)w ;
		sum += u;
	}
   // 将反馈的16bit从高位移至地位
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}
 
 
 
// 发送回应请求函数
int SendEchoRequest(SOCKET s,struct sockaddr_in *lpstToAddr) 
{
	static ECHOREQUEST echoReq;
	static u_short nId = 1;
	static u_short nSeq = 1;
	int nRet;
 
	// 填充回应请求消息
	echoReq.icmpHdr.Type		= ICMP_ECHOREQ;
	echoReq.icmpHdr.Code		= 0;
	echoReq.icmpHdr.Checksum	= 0;
	echoReq.icmpHdr.ID			= nId++;
	echoReq.icmpHdr.Seq			= nSeq++;
 
	// 填充要发送的数据（随便填写）
	for (nRet = 0; nRet < REQ_DATASIZE; nRet++)
		echoReq.cData[nRet] = ' '+nRet;
 
	// 储存发送的时间
	echoReq.dwTime = GetTickCount();
 
	// 计算回应请求的校验和
	echoReq.icmpHdr.Checksum = checksum((u_short *)&echoReq, sizeof(ECHOREQUEST));
 
	// 发送回应请求  								  
	nRet = sendto(s,						// 建立起的套接字
				 (LPSTR)&echoReq,			// 发送的缓冲区内容
				 sizeof(ECHOREQUEST),
				 0,							// 标志位
				 (struct sockaddr*)lpstToAddr, // 发送的目标地址
				 sizeof(SOCKADDR_IN));   // 地址结构长度
 
	if (nRet == SOCKET_ERROR)
	{
		TRACE("sendto() error:%d\n",WSAGetLastError());
	}
	return (nRet);
}
 
// 接收应答回复并进行解析
DWORD RecvEchoReply(SOCKET s, LPSOCKADDR_IN lpsaFrom, u_char *pTTL) 
{
	ECHOREPLY echoReply;
	int nRet;
	int nAddrLen = sizeof(struct sockaddr_in);
 
	//接收应答回复	
	nRet = recvfrom(s,					// 接收的套接字
					(LPSTR)&echoReply,	// 接收的缓冲区
					sizeof(ECHOREPLY),	// 缓冲区长度
					0,					// 标识
					(LPSOCKADDR)lpsaFrom,	// 接收的地址
					&nAddrLen);			// 地址结构长度
 
	// 检验接收结果
	if (nRet == SOCKET_ERROR) 
	{
		TRACE("recvfrom() error:%d\n",WSAGetLastError());
	}
    // 记录返回的TTL
	*pTTL = echoReply.ipHdr.TTL;
	//返回应答时间
	return(echoReply.echoRequest.dwTime);  		
}
 
// 等待回应答复，使用select机制
int WaitForEchoReply(SOCKET s)
{
	struct timeval timeout;
	fd_set readfds;
 
	readfds.fd_count = 1;
	readfds.fd_array[0] = s;
	timeout.tv_sec = 5;
    timeout.tv_usec = 0;
 
	return(select(1, &readfds, NULL, NULL, &timeout));
}
 
// Ping功能实现
CString Ping(const char *pstrHost)
{
	CString	sText;
	CString response = "";
	SOCKET	  rawSocket;
	LPHOSTENT lpHost;
	struct    sockaddr_in destIP;
	struct    sockaddr_in srcIP;
	DWORD	  dwTimeSent;
	DWORD	  dwElapsed;
	u_char    cTTL;
	int       nLoop;
	int       nRet;
 
	// 创建原始套接字，ICMP类型
	rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rawSocket == SOCKET_ERROR) 
	{
		sText.Format("socket() error:%d\n",WSAGetLastError());
		response += sText;
		TRACE(sText);
		return response;
	}
	
	// 检测目标主机
	lpHost = gethostbyname(pstrHost);
	if (lpHost == NULL)
	{
		sText.Format("Host not found: %s\n", pstrHost);
		response += sText;
		TRACE("Host not found: %s\n", pstrHost);
		return response;
	}
	
	// 设置目标机地址
	destIP.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
	destIP.sin_family = AF_INET;
	destIP.sin_port = 0;
 
	// 提示开始进行Ping
	sText.Format("\nPinging %s with %d bytes of data:\n",
				inet_ntoa(destIP.sin_addr),
				REQ_DATASIZE);
	response += sText;
	TRACE("\nPinging %s with %d bytes of data:\n",
				inet_ntoa(destIP.sin_addr),
				REQ_DATASIZE);
 
 
	// 发起多次Ping测试
	for (nLoop = 0; nLoop < 1; nLoop++)
	{
		//发送ICMP回应请求
		SendEchoRequest(rawSocket, &destIP);
 
		// 使用select()等待回复的数据
		nRet = WaitForEchoReply(rawSocket);
		if (nRet == SOCKET_ERROR)
		{
			sText.Format("select() error:%d\n",WSAGetLastError());
			response += sText;
			TRACE("select() error:%d\n",WSAGetLastError());
			break;
		}
		if (!nRet)
		{
			sText.Format("\nRequest time out");
			response += sText;
			TRACE("\nRequest time out");
			break;
		}
 
		//接收回复
		dwTimeSent = RecvEchoReply(rawSocket, &srcIP, &cTTL);
 
		// 计算花费的时间
		dwElapsed = GetTickCount() - dwTimeSent;
		sText.Format("\nReply from %s: bytes=%d time=%ldms TTL=%d", 
               inet_ntoa(srcIP.sin_addr), 
			   REQ_DATASIZE,
               dwElapsed,
               cTTL);
		response += sText;
		TRACE("\nReply from %s: bytes=%d time=%ldms TTL=%d", 
               inet_ntoa(srcIP.sin_addr), 
			   REQ_DATASIZE,
               dwElapsed,
               cTTL);
	}
 
	sText.Format("\n");
	response += sText;
	TRACE("\n");
	nRet = closesocket(rawSocket); // 关闭套接字，释放资源
	if (nRet == SOCKET_ERROR)
	{
		sText.Format("closesocket() error:%d\n",WSAGetLastError());
		response += sText;
		TRACE("closesocket() error:%d\n",WSAGetLastError());
	}
	return response;
}
 
CString ping_test(CString ip)
{
	CString response;
    WSADATA wsd;
	if (WSAStartup(MAKEWORD(1,1), &wsd) != 0)
    {
        TRACE("加载Winsock失败!\n");
    }
 
	//开始Ping
	response = Ping(ip);
 
	// 释放Winsock资源
    WSACleanup();
	return response;
}