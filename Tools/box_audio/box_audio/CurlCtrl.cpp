#include "stdafx.h"
#include "CurlCtrl.h"
#include<string.h>
#include<stdio.h>
#pragma comment(lib,"curl/x32/libcurl.lib") 
extern CString g_mes_ip;                  //MES端口的设置
extern UINT g_mes_port;
CCurlCtrl::CCurlCtrl(void)
{
	m_svrAddr.Empty();
	m_sTation.Empty();
	m_sType.Empty();
	m_svrPort = 8090;
	m_svrAddr = "202.104.96.30";
	m_pCurl = NULL;
	Messtatus = 0;
	log = 0;
	
	curl_global_init(CURL_GLOBAL_ALL);


}


CCurlCtrl::~CCurlCtrl(void)
{
}

void CCurlCtrl::SetServerInfo(CString sAddr, int port, CString sType, CString sTation)
{
	m_svrAddr.Format("%s", sAddr);
	m_sTation.Format("%s", sTation);
	m_sType.Format("%s", sType);

	m_svrPort = port;
}


int CCurlCtrl::Handdata(char *data)
{
	if(strstr(data,"code") >= 0){
		if(strstr(data,"\"code\":0") != NULL){
			Messtatus |= 1<<log;
		}
		log ++;
	}

	return 0;
}
int CCurlCtrl::CallbackRecv(void *buffer, int size, int nmemb, void *user)
{
	CCurlCtrl *thisCurl = (CCurlCtrl*)user;
	BYTE *ptr = (BYTE*)buffer;

	//TRACE("Size=%d %d\n", size, nmemb);

//	TRACE("%s\n", (char*)buffer);
	char *sect = (char*)buffer;
	memset(thisCurl->line,0,sizeof(thisCurl->line));

	memcpy(thisCurl->line,sect,strlen(sect));

	thisCurl->line[strlen(sect)] = '\0';
	TRACE("line:%s\n",thisCurl->line);
	thisCurl->Handdata(thisCurl->line);

	return size * nmemb;
}

INT CCurlCtrl::CurlSend(char *postdat)
{
	CURL *curl;
	
	CString sText;
	sText.Format("http://%s:%d/sg/ajax?",g_mes_ip,g_mes_port);

	char url[256];

	strncpy_s(url,(char*)(LPCTSTR)sText,sText.GetLength());

	curl =  curl_easy_init();                     
	
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);      //延续3秒
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdat);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,CCurlCtrl::CallbackRecv);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

	CURLcode value = curl_easy_perform(curl);

	if(value != 0){ 
		TRACE("MESERR:%d\n",value);
	}
	curl_easy_cleanup(curl);

	return value;
}


///check
int CCurlCtrl::SendChecks(CString sSN)
{
	int value;
	char dat[256];
	sprintf_s(dat, "style=smtUtil&type=RouterCheck&allow_type=allow_type&station=ASSY_ BLUETOOTH_TEST&sn=%s",
	sSN);

	//sprintf_s(dat, "style=smtUtil&type=RouterCheck&allow_type=allow_type&station=ASSY_ BLUETOOTH_TEST&sn=ZYFYD00-04-02000517");

	value = CurlSend(dat);

	return value;
}

int CCurlCtrl::SendChecke(CString sSN)
{
	int value;
	char dat[512];

	sprintf_s(dat, "style=smtUtil&type=StationPass&allow_type=allow_type&Station=ASSY_ BLUETOOTH_TEST&Sn=%s &Status=0&ToolNo=&UserName=UA&ErrMsg=", 
		sSN);


	//sprintf_s(dat, "style=smtUtil&type=StationPass&allow_type=allow_type&Station=ASSY_ BLUETOOTH_TEST&Sn=ZYFYD00-04-02000517&Status=0&ToolNo=&UserName=UA&ErrMsg=");

	value = CurlSend(dat);

	return value;
}