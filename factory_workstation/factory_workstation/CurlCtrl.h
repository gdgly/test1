#pragma once

#define CONFIG_SUPPORT_MES

#include "curl\include\curl\curl.h"

struct ch_mes_data{
	unsigned int data_flag;
	UINT       iBatVal;             // 获取电压
	BYTE       firversion[8];
	BYTE	   souversion[30];	
};
class CCurlCtrl
{
public:
	CCurlCtrl(void);
	~CCurlCtrl(void);

public:
	CString CCurlCtrl::sz_CurlSend(CString url_param, bool post_flag, CString post_data);
	CString CCurlCtrl::sz_get_btaddr(CString sSN,CString station,int rl_flag,CString &response_msg);
	CString CCurlCtrl::sz_post_passfail(CString sSN,CString station,int rl_flag,bool pass_fail_flag);
	int changh_mes_int(BYTE *sSN,struct ch_mes_data data_p);
	int changh_mes_int(CString sSN,struct ch_mes_data data_p);
	void SetServerInfo(CString sAddr, int port, CString sType, CString sTation);
	INT  CurlSend(char *postdat);
	int SendBurns(CString sSN);
	int SendBurne(CString sSN);
	int SendChecks(CString sSN);
	int SendChecke(CString sSN);
	int function_SendChecks(CString sSN);
	int function_SendChecke(CString sSN);
	int bluetooth_SendChecks(CString sSN);
	int bluetooth_SendChecke(CString sSN);
	int power_SendChecks(CString sSN);
	int power_SendChecke(CString sSN,CString data, CString ifirver, CString isouver);
	int power_SendDatae(CString sSN);
	int by_power_SendChecke(CString sSN,CString data,CString ifirver, CString isouver, CString normal_cur, CString char_cur);
	int Handdata(char *data);
	void GetStatus(UINT hlog,UINT hMesstatus);
	void clear_flag(void);
	int Messtatus;
	int log;
	char line[256];
	CString g_mes_ip;                  //MES端口的设置
	UINT g_mes_port;

private:
	static int CallbackRecv(void *buffer, int size, int nmemb, void *user);
	CString m_svrAddr, m_sTation, m_sType;
	UINT   m_svrPort;


	CURL  *m_pCurl;
};


#if 0
工位接口
版本 V1.0

版本号	版本内容	编写人	编写时间
1.0	星空物连项目接口说明文档整合	曾永兴	2018-03-08
			
1.	版本更新说明

2.	接口调用流程图：
	接口访问条件：
	如果MES服务未发布必须在同一个内网中;
	在MES中添加本机IP的权限，步骤：进入MES基础维护生产资料管理 IP配置，在此页面中先根据本机IP查询，如没有结果就点击新增，将本机IP添加到MES中

3.	通用测试工位接口
...3.1 适用工位:
	测试check：站位编:TEST_CHECK；测试工位
	蓝牙传输测试；站位编码:ASSY_ BLUETOOTH_TEST；组装工位
	组装CHECK：站位编码:ASSY_CHECK；组装工位	
...3.2 接口详细信息说明
…..3.2.1 流程检查接口
	接口
http://172.16.1.32:8589/sg/ajax?style=smtUtil&type=RouterCheck&allow_type=allow_type&station=站位编码&sn=PCB SN 
参数说明
	服务器IP：例如172.16.1.32，需工具界面可配置。
	服务器端口：例如：8589，需工具界面可配置。
	station：MES系统站位编码，请按3.1适应工位对应的工站编码传入（蓝色背景字体）。
	sn：PCB SN。
	返回值说明：返回值格式为JSON
	正常时返回如下：{“code”:0} 
	失败时返回如下：{“code”:-1, “desc”:错误信息}

…..3.2.2数据上报接口
	接口
http://172.16.1.32:8589/sg/ajax?style=smtUtil&type=StationPass&allow_type=allow_type&Station=站位编码&Sn=PCB SN &Status=0&ToolNo=夹具编码&UserName=员工编号&ErrMsg=失败信息

参数说明
	服务器IP：例如172.16.1.32，需工具界面可配置。
	服务器端口：例如：8589，需工具界面可配置。
	Station：MES系统站位编码，请按3.1适应工位对应的工站编码传入（蓝色背景字体）。。
	Sn：PCB SN。
	ToolNo：夹具编码，没有可置空。
	UserName：员工编号，如无则为固定值“UA”，不可为空。
	Status:测试状态, 0表示测试通过(pass)；1表示测试不通过(fail)
	ErrMsg：失败信息。

	返回值说明：返回值格式为JSON
	正常时返回如下：{“code”:0}
	失败时返回如下：{“code”:-1, “desc”:错误信息}
4.	测试写SN接口(烧录功能站点)
…4.1 流程检查接口
http://172.16.1.32:8589/sg/ajax?style=smtUtil&type=queryWriteFlagBySn&allow_type=allow_type
&pn=SN号

	参数说明:
	172.16.1.32:服务器IP地址，需工具界面可配置。
	F8589:MES服务的端口，需工具界面可配置。
	Pn:写入手机里面的主板SN

返回说明:
	过站信息正常时返回如下: 
{“code”:0, “bt”: A23D3048A001 }
	如果过站信息或者数据有异常时返回如下:
{“code”:-1,”desc”:错误信息}

…4.2 数据上报接口
http://172.16.1.32:8589/sg/ajax?style=smtUtil&type=writeFlagBySn&allow_type=allow_type
&pn=主板SN号&status=0&error=

	参数说明:
	172.16.1.32:服务器IP地址，需工具界面可配置。
	8589:MES服务的端口，需工具界面可配置。
	Pn:写入手机里面的主板SN
	status:测试状态, 0表示测试通过；1表示测试不通过
	error:测试手机失败的错误信息

返回说明:
	过站信息正常时返回如下:
{“code”:0}
	如果过站信息或者数据有异常时返回如下:
{“code”:-1,”desc”:错误信息}

#endif
