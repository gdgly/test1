#include "stdafx.h"
#include "CurlCtrl.h"
#include<string.h>
#include<stdio.h>
#pragma comment(lib,"curl/x32/libcurl.lib") 

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

void CCurlCtrl::clear_flag(void)
{
	Messtatus = 0;
	log = 0;
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

	TRACE("%s\n", (char*)buffer);
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
	//FILE *fp;
	//
	//if((fopen_s(&fp,"meslog.txt", "w")) != 0)  // 返回结果用文件存储
	//	return false;
	CString sText;
	sText.Format("http://%s:%d/sg/ajax?",g_mes_ip,g_mes_port);

	//char url[256] = "http://10.1.8.240:8090/sg/ajax?";
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
	//fclose(fp);
	return value;
}

//烧录
int CCurlCtrl::SendBurns(CString sSN)
{
	int value;
	char dat[256];

	sprintf_s(dat, "style=smtUtil&type=queryWriteFlagBySn&allow_type=allow_type&pn=%s"
		,sSN);
	value = CurlSend(dat);

	return value;
}

int CCurlCtrl::SendBurne(CString sSN)
{
	int value;
	char dat[256];
	sprintf_s(dat, "style=smtUtil&type=writeFlagBySn&allow_type=allow_type&pn=%s&status=0&error="
		,sSN);

	value = CurlSend(dat);
	return value;
}





///check
int CCurlCtrl::SendChecks(CString sSN)
{
	int value;
	char dat[256];
	sprintf_s(dat, "style=smtUtil&type=RouterCheck&allow_type=allow_type&station=TEST_CHECK&sn=%s",
		sSN);
	value = CurlSend(dat);

	return value;
}

int CCurlCtrl::SendChecke(CString sSN)
{
	int value;
	char dat[512];

	sprintf_s(dat, "style=smtUtil&type=StationPass&allow_type=allow_type&Station=TEST_CHECK&Sn=%s &Status=0&ToolNo=&UserName=UA&ErrMsg=", 
		sSN);

	value = CurlSend(dat);

	return value;
}

int CCurlCtrl::bluetooth_SendChecks(CString sSN)
{
	int value;
	char dat[256];
	sprintf_s(dat, "style=smtUtil&type=RouterCheck&allow_type=allow_type&station=ASSY_ BLUETOOTH_TEST&sn=%s",
	sSN);

	//sprintf_s(dat, "style=smtUtil&type=RouterCheck&allow_type=allow_type&station=ASSY_ BLUETOOTH_TEST&sn=ZYFYD00-04-02000517");

	value = CurlSend(dat);

	return value;
}

int CCurlCtrl::bluetooth_SendChecke(CString sSN)
{
	int value;
	char dat[512];

	sprintf_s(dat, "style=smtUtil&type=StationPass&allow_type=allow_type&Station=ASSY_ BLUETOOTH_TEST&Sn=%s &Status=0&ToolNo=&UserName=UA&ErrMsg=", 
		sSN);


	//sprintf_s(dat, "style=smtUtil&type=StationPass&allow_type=allow_type&Station=ASSY_ BLUETOOTH_TEST&Sn=ZYFYD00-04-02000517&Status=0&ToolNo=&UserName=UA&ErrMsg=");

	value = CurlSend(dat);

	return value;
}

int CCurlCtrl::function_SendChecks(CString sSN)
{
	int value;
	char dat[256];
	sprintf_s(dat, "style=smtUtil&type=RouterCheck&allow_type=allow_type&station=ASSY-FUNCTIONCHECK&sn=%s",
	sSN);

	//sprintf_s(dat, "style=smtUtil&type=RouterCheck&allow_type=allow_type&station=ASSY_ BLUETOOTH_TEST&sn=ZYFYD00-04-02000517");

	value = CurlSend(dat);

	return value;
}

int CCurlCtrl::function_SendChecke(CString sSN)
{
	int value;
	char dat[512];

	sprintf_s(dat, "style=smtUtil&type=StationPass&allow_type=allow_type&Station=ASSY-FUNCTIONCHECK&Sn=%s &Status=0&ToolNo=&UserName=UA&ErrMsg=", 
		sSN);

	//sprintf_s(dat, "style=smtUtil&type=StationPass&allow_type=allow_type&Station=ASSY_ BLUETOOTH_TEST&Sn=ZYFYD00-04-02000517&Status=0&ToolNo=&UserName=UA&ErrMsg=");

	value = CurlSend(dat);

	return value;
}


///电池电量测试上传MES
int CCurlCtrl::power_SendChecks(CString sSN)
{
	int value;
	char dat[256];
	sprintf_s(dat, "style=smtUtil&type=RouterCheck&allow_type=allow_type&station=PACK_POWER_CHECK&sn=%s",
	sSN);
	value = CurlSend(dat);
	return value;
}

int CCurlCtrl::power_SendDatae(CString sSN)
{
	int value;
	char dat[768];
	sprintf_s(dat,"style=smtUtil&type=StationPass&allow_type=allow_type&Station=PACK_POWER_CHECK&Sn=%s&Status=0&ToolNo=&UserName=UA&ErrMsg=失败信息&LogFile=",
		sSN);
	value = CurlSend(dat);
	return value;
}

//上传电量值
int CCurlCtrl::power_SendChecke(CString sSN,CString data,CString ifirver, CString isouver)
{
	int value;
	char dat[512];
	CString sTime;

	CTime tNow = CTime::GetCurrentTime();
	sTime.Format("%04d%02d%02d-%02d:%02d", tNow.GetYear(), tNow.GetMonth(),tNow.GetDay(), tNow.GetHour(), tNow.GetMinute());
	sprintf_s(dat, "style=smtUtil&type=JDdata&allow_type=allow_type&sn=%s&Position=PACK_POWER_CHECK&data1=%s&data2=%s&data3=%s&data4=%s", 
		sSN,data,ifirver,isouver,sTime);
	value = CurlSend(dat);
	return value;
}

//上传电量值
int CCurlCtrl::by_power_SendChecke(CString sSN,CString data,CString ifirver, CString isouver, CString normal_cur, CString char_cur)
{
	int value;
	char dat[512];
	CString sTime;

	CTime tNow = CTime::GetCurrentTime();
	sTime.Format("%04d%02d%02d-%02d:%02d", tNow.GetYear(), tNow.GetMonth(),tNow.GetDay(), tNow.GetHour(), tNow.GetMinute());
	sprintf_s(dat, "style=smtUtil&type=JDdata&allow_type=allow_type&sn=%s&Position=PACK_POWER_CHECK&data1=%s&data2=%s&data3=%s&data4=%s&data5=%s&data6=%s", 
		sSN,data,ifirver,isouver,sTime,normal_cur,char_cur);
	value = CurlSend(dat);
	return value;
}

int CCurlCtrl::changh_mes_int(CString sSN,struct ch_mes_data data_p)
{
	CFile m_hLogFile;
	CString sText,sText1;
	if(data_p.data_flag == 1){
		sText = "MESUP\\burn\\";
	}else if(data_p.data_flag == 2){
		sText = "MESUP\\check\\";
	}else if(data_p.data_flag == 3){
		sText = "MESUP\\bluetooth\\";
	}else if(data_p.data_flag == 4){
		sText = "MESUP\\batpower\\";
	}
	CreateDirectory("MESUP", 0);
	CreateDirectory(sText, 0);
	sText += sSN;
	sText += ".txt";
	if(m_hLogFile.Open(sText, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) == TRUE) {
		m_hLogFile.SeekToBegin();
		sText.Format("PASS\r\n");
		if(data_p.data_flag == 4){
			sText1.Format("%d\r\n",data_p.iBatVal);
			sText += sText1;
			sText1.Format("%s\r\n",data_p.firversion);
			sText += sText1;
			sText1.Format("%s\r\n",data_p.souversion);
			sText += sText1;
		}
		m_hLogFile.Write(sText, sText.GetLength());
		m_hLogFile.Close();
	}
	return 0;
}

int CCurlCtrl::changh_mes_int(BYTE *sSN,struct ch_mes_data data_p)
{
	CFile m_hLogFile;
	CString sText,sText1;
	if(data_p.data_flag == 1){
		sText = "MESUP\\burn\\";
	}else if(data_p.data_flag == 2){
		sText = "MESUP\\check\\";
	}else if(data_p.data_flag == 3){
		sText = "MESUP\\bluetooth\\";
	}else if(data_p.data_flag == 4){
		sText = "MESUP\\batpower\\";
	}
	CreateDirectory("MESUP", 0);
	CreateDirectory(sText, 0);
	sText1.Format("%s",sSN);
	sText += sText1;
	sText += ".txt";
	if(m_hLogFile.Open(sText, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) == TRUE) {
		m_hLogFile.SeekToBegin();
		sText.Format("PASS\r\n");
		if(data_p.data_flag == 4){
			sText1.Format("%d\r\n",data_p.iBatVal);
			sText += sText1;
			sText1.Format("%s\r\n",data_p.firversion);
			sText += sText1;
			sText1.Format("%s\r\n",data_p.souversion);
			sText += sText1;
		}
		m_hLogFile.Write(sText, sText.GetLength());
		m_hLogFile.Close();
	}
	return 0;
}

size_t writeToString(void *ptr, size_t size, size_t count, void *stream)
{
	CString sText;
	sText.Format("%s", ptr);
	*((CString*)stream) += sText;
    return size* count;
}

//#include <string>
//using namespace std;
//wstring AsciiToUnicode(const string& str) 
//{
//    // 预算-缓冲区中宽字节的长度  
//    int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
//    // 给指向缓冲区的指针变量分配内存  
//    wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
//    // 开始向缓冲区转换字节  
//    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);
//    wstring ret_str = pUnicode;
//    free(pUnicode);
//    return ret_str;
//}
//
//string UnicodeToUtf8(const wstring& wstr) 
//{
//    // 预算-缓冲区中多字节的长度  
//    int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
//    // 给指向缓冲区的指针变量分配内存  
//    char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
//    // 开始向缓冲区转换字节  
//    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
//    string ret_str = pAssii;
//    free(pAssii);
//    return ret_str;
//}
//
//string AsciiToUtf8(const string& str) 
//{
//    return UnicodeToUtf8(AsciiToUnicode(str));
//}

CString CCurlCtrl::sz_CurlSend(CString url_param, bool post_flag, CString post_data)
{
	struct curl_slist* headers = NULL;
	CURL *curl;CURLcode res;
	CString url = "http://www.baidu.com";
	CString sText;	CString data;
	sText.Format("http://%s:%d/",g_mes_ip,g_mes_port);
	url = sText;
	url += url_param;

	curl =  curl_easy_init();  
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);      //延续5秒
		if(post_flag){
			//headers=curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
			headers=curl_slist_append(headers, "Content-Type:application/json");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

			curl_easy_setopt(curl, CURLOPT_POST, 1L);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
			//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_str.c_str());
		}
 
		// write curl response to string variable..
		res = curl_easy_perform(curl);
		if(post_flag){
			curl_slist_free_all(headers);
		}
		/* Check for errors */
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
 
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	return data;
}

CString CCurlCtrl::sz_get_btaddr(CString sSN,CString station,int rl_flag,CString &response_msg)
{
	CString mac;CString url_param;
	CString sText;CString response;int length;
	url_param.Format("api/KS/serial/%s/station/%s", sSN, station);
	response = sz_CurlSend(url_param, 0, NULL);
	//response = "\"mac\":\"11:22:33:44:55:66\"";
	//response = "\"mac\":\"70b3d54d3064\"";
	if( (length = response.Find("\\\"mac\\\"")) >= 0){
		//sText = response.Mid(length+sizeof("\"mac\":"), 17);
		sText = response.Mid(length+sizeof("\\\"mac\\\":\\"), 12);
		mac = sText;
	}else{
		//mac = "00:00:00:00:00:00";
		mac = "000000000000";
		response_msg = response;
	}
	return mac;
}

//CString CCurlCtrl::sz_post_passfail(CString sSN,CString station,int rl_flag,bool pass_fail_flag)
//{
//	CTime tNow = CTime::GetCurrentTime();
//	CString ret;CString url_param;CString post_data;
//	CString sText;CString response;int length;
//	url_param.Format("api/KS/Result/serial/%s/station/%s/results", sSN, station);
//	post_data = "{";
//	sText.Format("\"serial\": \"%s\",", sSN);
//	post_data += sText;
//
//	sText.Format("\"test_station\": \"%s\",", station);
//	post_data += sText;
//
//	sText.Format("\"start_time\": \"%04d-%02d-%02d %02d:%02d:%02d\",", 
//		tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), 
//		tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond() );
//	post_data += sText;
//
//	sText.Format("\"finish_time\": \"%04d-%02d-%02d %02d:%02d:%02d\",", 
//		tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), 
//		tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond() );
//	post_data += sText;
//
//	if(pass_fail_flag){
//		sText.Format("\"status\": \"passed\",");
//	}else{
//		sText.Format("\"status\": \"failed\",");
//	}
//	post_data += sText;
//
//	sText.Format("\"error_code\": \"\",");
//	post_data += sText;
//	post_data += "}";
//
//	response = sz_CurlSend(url_param, 1, post_data);
//
//	if( (length = response.Find("OK")) >= 0){
//		ret = "OK";
//	}else{
//		ret = response;
//	}
//	return ret;
//}

CString CCurlCtrl::sz_post_passfail(CString sSN,CString station,int rl_flag,bool pass_fail_flag)
{
	CTime tNow = CTime::GetCurrentTime();
	CString ret;CString url_param;CString post_data;
	CString sText;CString response;int length;
	url_param.Format("api/KS/Result/serial/%s/station/%s/results", sSN, station);
	post_data = "{";
	sText.Format("\"serial\": \"%s\",", sSN);
	post_data += sText;

	sText.Format("\"test_station\": \"%s\",", station);
	post_data += sText;

	sText.Format("\"start_time\": \"%04d-%02d-%02d %02d:%02d:%02d\",", 
		tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), 
		tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond() );
	post_data += sText;

	sText.Format("\"finish_time\": \"%04d-%02d-%02d %02d:%02d:%02d\",", 
		tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), 
		tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond() );
	post_data += sText;

	if(pass_fail_flag){
		sText.Format("\"status\": \"passed\",");
	}else{
		sText.Format("\"status\": \"failed\",");
	}
	post_data += sText;

	sText.Format("\"error_code\": \"\",");
	post_data += sText;
	post_data += "}";

	response = sz_CurlSend(url_param, 1, post_data);

	if( (length = response.Find("OK")) >= 0){
		ret = "OK";
	}else{
		ret = response;
	}
	return ret;
}

