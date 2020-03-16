// software_tool_vnc.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_vnc.h"
#include "afxdialogex.h"


// software_tool_vnc 对话框

IMPLEMENT_DYNAMIC(software_tool_vnc, CDialogEx)

software_tool_vnc::software_tool_vnc(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_vnc::IDD, pParent)
{

}

software_tool_vnc::~software_tool_vnc()
{
}

void software_tool_vnc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SET_SN, m_EditSn);
	DDX_Control(pDX, IDC_LIST3, m_listEvt);
}


BEGIN_MESSAGE_MAP(software_tool_vnc, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON13, &software_tool_vnc::OnBnClickedButton13)
	ON_EN_CHANGE(IDC_SET_SN, &software_tool_vnc::OnEnChangeSetSn)
	ON_BN_CLICKED(IDC_BUTTON22, &software_tool_vnc::OnBnClickedButton22)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

void software_tool_vnc::AddEvent2List(CString sText)
{
	if(quit_thread)
		return ;
	CTime tNow = CTime::GetCurrentTime();
	CString sTemp;
	int count = m_listEvt.GetCount();
	sTemp.Format("%02d:%02d:%02d %s", tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond(), sText);
	m_listEvt.InsertString(count, sTemp);
	m_listEvt.SetCurSel( m_listEvt.GetCount() - 1);

	//if(m_hLogFile.m_hFile != INVALID_HANDLE_VALUE) {
	//	m_hLogFile.Write(sTemp, sTemp.GetLength());
	//	m_hLogFile.Write("\r\n", 2); m_hLogFile.Flush();
	//}
}

bool software_tool_vnc::OpenWLANService()
{
	dwResult = WlanOpenHandle(1    //客户端支持的WLAN API的最高版本
			, NULL, &dwCurVersion    //指定这次会话中将会使用的版本
			, &hClient    //指定客户端在这次会话中使用的句柄，这个句柄会贯穿整个会话被其他函数使用 。
			);
    if(ERROR_SUCCESS != dwResult){
        switch (dwResult){
            case ERROR_INVALID_PARAMETER:    //参数一、四为空或参数二不为空
                TRACE(L"Para is NULL\n"); 
                break;
            case ERROR_NOT_ENOUGH_MEMORY:    //没有足够的内存空间
                TRACE(L"Failed to allocate memory \n");
                break; 
            case ERROR_REMOTE_SESSION_LIMIT_EXCEEDED:    //超过服务器限定的句柄数量
                TRACE(L"Server is Busy\n");
                break;
            default:
                dwResult = GetLastError();
                TRACE(L"WlanOpenHandle Fail：%wd\n", dwResult);
                break;
        }
		return false;
    }
	return true;
}
bool software_tool_vnc::WLANNetwork()
{
	dwResult = WlanEnumInterfaces(hClient    //客户端会话句柄。由 WlanOpenHandle 得到。
		, NULL, &pInterfaceList    //指向包含无线网卡接口信息list的结构体 PWLAN_INTERFACE_INFO_LIST 的指针。
		);
    if(ERROR_SUCCESS != dwResult){
        switch (dwResult){
			case ERROR_INVALID_PARAMETER:    //参数一、三为空，或参数二不为空
				TRACE(L"Para is NULL\n");
				break;
			case ERROR_INVALID_HANDLE:    //无效的句柄
				TRACE(L"Failed to INVALID HANDLE \n");
				break; 
			case ERROR_NOT_ENOUGH_MEMORY:    //没有足够的内存空间
				TRACE(L"Failed to allocate memory \n");
				break; 
			default:
				dwResult = GetLastError();
				TRACE(L"WlanEnumInterfaces Fail： %wd\n", dwResult);
				break;
        }
		return false;
    }
	TRACE(L"WlanEnumInterfaces Number %wd\n", pInterfaceList->dwNumberOfItems);    //输出网卡数量
	return true;
}

LPCWSTR software_tool_vnc::StringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length()-1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

bool software_tool_vnc::SetProfile()
{ 
	string szProfileXML("");  //Profile XML流
	LPCWSTR wscProfileXML = NULL;
	/*组合参数XML码流*/ 
    string szTemp("");
	// char p[1024];
    /*头*/
    szProfileXML += string("<?xml version=\"1.0\"?><WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\"><name>");
    /*name，一般与SSID相同*/
	szTemp = string((char*)pNet->dot11Ssid.ucSSID);    //配置文件名
    szProfileXML += szTemp;
    /*SSIDConfig*/
    szProfileXML += string("</name><SSIDConfig><SSID><name>");
	szProfileXML += szTemp;    //已搜索到的网络对应的SSID
	szProfileXML += string("</name></SSID></SSIDConfig>");
	/*connectionType*/
	szProfileXML += string("<connectionType>");
	switch(pNet->dot11BssType){    //网络类型
        case dot11_BSS_type_infrastructure:
            szProfileXML += "ESS";
            break;
        case dot11_BSS_type_independent:
            szProfileXML += "IBSS";
            break;
        case dot11_BSS_type_any:
            szProfileXML += "Any";
            break;
        default:
            TRACE(L"Unknown BSS type");
            return false;
    }
	/*MSM*/
	szProfileXML += string("</connectionType><MSM><security><authEncryption><authentication>");
	switch( pNet->dot11DefaultAuthAlgorithm){    //网络加密方式
        case DOT11_AUTH_ALGO_80211_OPEN:
            szProfileXML += "open";
			TRACE(L"Open 802.11 authentication\n");
            break;
        case DOT11_AUTH_ALGO_80211_SHARED_KEY:
			szProfileXML += "shared";
            TRACE(L"Shared 802.11 authentication");
            break;
        case DOT11_AUTH_ALGO_WPA:
			szProfileXML += "WPA";
            TRACE(L"WPA-Enterprise 802.11 authentication\n");
            break;
        case DOT11_AUTH_ALGO_WPA_PSK:
			szProfileXML += "WPAPSK";
            TRACE(L"WPA-Personal 802.11 authentication\n");
            break;
        case DOT11_AUTH_ALGO_WPA_NONE:
			szProfileXML += "none";
            TRACE(L"WPA-NONE,not exist in MSDN\n");
            break;
        case DOT11_AUTH_ALGO_RSNA:
			szProfileXML += "WPA2";
            TRACE(L"WPA2-Enterprise 802.11 authentication\n");
            break;
        case DOT11_AUTH_ALGO_RSNA_PSK:
			szProfileXML += "WPA2PSK";
            TRACE(L"WPA2-Personal 802.11 authentication\n");
            break;
        default:
			TRACE(L"Unknown authentication");
   			return false;
    }
	szProfileXML += string("</authentication><encryption>");
	/*sprintf(p, "%d", pNet->dot11DefaultCipherAlgorithm);
	szProfileXML += string(p);*/
	switch(pNet->dot11DefaultCipherAlgorithm){
        case DOT11_CIPHER_ALGO_NONE:
            szProfileXML += "none";
            break;
        case DOT11_CIPHER_ALGO_WEP40:
            szProfileXML += "WEP";
            break;
        case DOT11_CIPHER_ALGO_TKIP:
            szProfileXML += "TKIP";
            break;
        case DOT11_CIPHER_ALGO_CCMP:
            szProfileXML += "AES";
            break;
        case DOT11_CIPHER_ALGO_WEP104:
            szProfileXML += "WEP";
            break;        
        case DOT11_CIPHER_ALGO_WEP:
            szProfileXML += "WEP";
            break;
		case DOT11_CIPHER_ALGO_WPA_USE_GROUP:
			TRACE(L"USE-GROUP not exist in MSDN");
        default:
            TRACE(L"Unknown encryption");
			return false;
    }
	//szProfileXML += string	("</encryption><useOneX>false</useOneX></authEncryption></security></MSM>");
    //如果加密方式为WEP，keyType必须改为networkKey
	szProfileXML += string	("</encryption></authEncryption><sharedKey><keyType>passPhrase</keyType><protected>false</protected><keyMaterial>");
	szProfileXML += targetKey;//;
	/*尾*/
	szProfileXML += string("</keyMaterial></sharedKey></security></MSM></WLANProfile>");
	/*XML码流转换成双字节*/
	wscProfileXML = StringToLPCWSTR(szProfileXML);
	if(NULL == wscProfileXML){
		TRACE(L"Change wscProfileXML fail\n");
		return false;
	}
 	/*设置网络参数*/
	DWORD dwReasonCode;
    dwResult = WlanSetProfile(hClient, &pInterface->InterfaceGuid, 
			0x00    //设置用户文件类型
		, wscProfileXML    //用户文件内容
		, NULL    //在XP SP1和SP2下必须为NULL
		, FALSE    //是否覆盖已存在的用户文件
		, NULL    //在XP SP1和SP2下必须为NULL
		, &dwReasonCode);
    if(ERROR_SUCCESS != dwResult){
        switch (dwResult){
			case ERROR_INVALID_PARAMETER:    //参数一、二、四、八为空或在XP SP1和SP2下参数三不为0
				TRACE(L"Para is NULL\n");
				break;
			case ERROR_NO_MATCH:    //网络接口不支持的加密类型
				TRACE(L"NIC NOT SUPPORT\n");
				break; 
			case ERROR_NOT_ENOUGH_MEMORY:    //没有足够的内存空间
				TRACE(L"Failed to allocate memory \n");
				break;
			case ERROR_BAD_PROFILE:    //用户文件格式错误
				TRACE(L"The profile specified by strProfileXml is not valid \n");
				break;
			case ERROR_ALREADY_EXISTS:    //设置的用户文件已存在
				TRACE(L"strProfileXml specifies a network that already exists \n");
				break;
			case ERROR_ACCESS_DENIED:    //用户没有权限设置用户文件
				TRACE(L"The caller does not set the profile. \n");
				break;
			default:
				dwResult = GetLastError();
				TRACE(L"WlanSetProfile Fail： %wd\n", dwResult); 
				break;
        }
		if (dwResult != 183){
			return false;
		}
    }
	return true;
}

bool software_tool_vnc::WLANConnect()
{
	pConnPara = (PWLAN_CONNECTION_PARAMETERS)calloc(1, sizeof(WLAN_CONNECTION_PARAMETERS));
    /*设置网络连接参数*/
    if(NULL == pConnPara){
        TRACE(L"pConnPara fail\n");
    }
	pConnPara->wlanConnectionMode = (WLAN_CONNECTION_MODE)0; //XP SP2,SP3 must be 0
	pConnPara->strProfile = StringToLPCWSTR(target);    //指定的用户文件
	pConnPara->pDot11Ssid = &pNet->dot11Ssid;    //指定的SSID
	pConnPara->pDesiredBssidList = NULL; //XP SP2,SP3 must be NULL
	pConnPara->dot11BssType = pNet->dot11BssType;    //网络类型
	pConnPara->dwFlags = 0x00000000; //XP SP2,SP3 must be 0
   
    dwResult = WlanConnect(hClient    //客户端句柄
		, &pInterface->InterfaceGuid    //连接使用的接口的GUID
		, pConnPara    //指向结构体 WLAN_CONNECTION_PARAMETERS ，其中指定了连接类型，模式，网络概况，SSID 等其他参数。
		, NULL);
    if(ERROR_SUCCESS != dwResult){
        switch (dwResult){
			case ERROR_INVALID_PARAMETER:
				TRACE(L"Para is NULL\n");
				break;
			case ERROR_INVALID_HANDLE:
				TRACE(L"Failed to INVALID HANDLE \n");
				break; 
			case ERROR_ACCESS_DENIED:
				TRACE(L"The caller does not have sufficient permissions. \n");
				break;
			default:
				dwResult = GetLastError();
				TRACE(L"WlanConnect Fail： %wd\n", dwResult);
				break;
        }
		return false;
    }
	return true;
}

bool software_tool_vnc::SearchNet()
{   
	int nInterfaceNumber;
	unsigned int nNetNumber;
	//CString sText="";

    if (!OpenWLANService()){
		AfxMessageBox("网卡打开失败，检查是否插上无线网卡");
        return false;
    }
    if (!WLANNetwork()){
		AfxMessageBox("网卡打开失败，检查是否插上无线网卡");
        return false;
    } 
	if ((int)pInterfaceList->dwNumberOfItems == 0){
		AfxMessageBox("未检测到可用无线网卡");
        return false;
    } 
    for(nInterfaceNumber = 0; nInterfaceNumber < (int)pInterfaceList->dwNumberOfItems; nInterfaceNumber++){
        /*获取网卡信息*/
        pInterface = (PWLAN_INTERFACE_INFO)&pInterfaceList->InterfaceInfo[nInterfaceNumber];
        /*获得网络信息列表*/
        dwResult = WlanGetAvailableNetworkList(hClient    //客户端的会话句柄
			, &pInterface->InterfaceGuid    //要搜索的接口的GUID
			, 0x00    //控制list中返回的网络的类型，XP SP2和SP3为零
			, NULL, &pNetList    //指向返回的可用网络的 WLAN_AVAILABLE_NETWORK_LIST 的指针
			);
        if(ERROR_SUCCESS != dwResult){
            switch (dwResult){
				case ERROR_INVALID_PARAMETER:    //参数四不为空，或其他参数为空
					TRACE(L"Para is NULL\n");
					break;
				case ERROR_INVALID_HANDLE:
					TRACE(L"Failed to INVALID HANDLE \n");
					break; 
				case ERROR_NOT_ENOUGH_MEMORY:
					TRACE(L"Failed to allocate memory \n");
					break;
				case ERROR_NDIS_DOT11_POWER_STATE_INVALID:    //广播关闭无法搜索
					TRACE(L"The radio for the interface is turned off \n");
					break;
				default:
					dwResult = GetLastError();
					TRACE(L"WlanGetAvailableNetworkList Fail： %wd\n", dwResult);
					break;
            }
			return false;
        }
        TRACE(L"WlanGetAvailableNetworkList Number %wd\n", pNetList->dwNumberOfItems);    //输出网络数量

        /*连接网络*/
		bool resultSearch = true;

        for(nNetNumber = 0; nNetNumber < pNetList->dwNumberOfItems; nNetNumber++){
            /*获取网络信息，去重复*/
			pNet = (PWLAN_AVAILABLE_NETWORK)&pNetList->Network[nNetNumber];     
			if (strcmp(target.c_str(),(char*)pNet->dot11Ssid.ucSSID) || !resultSearch){
				continue;
			}
			AddEvent2List("找到sn号名的热点");
			resultSearch = false;
			if (!SetProfile()){
				AfxMessageBox("网卡设置失败，检查下sn号是否正确");
				return false;
			}
			if (!WLANConnect()){
				AfxMessageBox("网卡连接失败，检查sn及密码是否正确");
				return false;
			}/* else {
				if (NULL != pNetList){
					WlanFreeMemory(pNetList);
					pNetList = NULL;
				}	
				if(NULL != pInterfaceList){
					WlanFreeMemory(pInterfaceList);
					pInterfaceList = NULL;
				}   
				TRACE("SUCESS\n");
				return true;
			}*/
		}
		if(resultSearch == true){
			AfxMessageBox("未检测到对应wifi网络！");
			return false;
		}
        /*释放网络信息列表*/
        if (NULL != pNetList){
            WlanFreeMemory(pNetList);
            pNetList = NULL;
        }
    }
    /*释放网卡信息列表*/
    if(NULL != pInterfaceList){
        WlanFreeMemory(pInterfaceList);
        pInterfaceList = NULL;
    }   
    TRACE("SUCESS\n");
    return true;
}

bool software_tool_vnc::get_wifi_status()
{
    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
    PWLAN_INTERFACE_INFO pIfInfo = NULL;
    dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList); 
    if (dwResult != ERROR_SUCCESS)  {
        TRACE(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
        // FormatMessage can be used to find out why the function failed
		return FALSE;
    }
    pIfInfo = (WLAN_INTERFACE_INFO *) &pIfList->InterfaceInfo[0];
    switch (pIfInfo->isState) {
            case wlan_interface_state_not_ready:
                TRACE(L"Not ready\n");
                break;
            case wlan_interface_state_connected:
                TRACE(L"Connected\n");
				return true;
                break;
            case wlan_interface_state_ad_hoc_network_formed:
                TRACE(L"First node in a ad hoc network\n");
                break;
            case wlan_interface_state_disconnecting:
                TRACE(L"Disconnecting\n");
                break;
            case wlan_interface_state_disconnected:
                TRACE(L"Not connected\n");
                break;
            case wlan_interface_state_associating:
                TRACE(L"Attempting to associate with a network\n");
                break;
            case wlan_interface_state_discovering:
                TRACE(L"Auto configuration is discovering settings for the network\n");
                break;
            case wlan_interface_state_authenticating:
                TRACE(L"In process of authenticating\n");
                break;
            default:
                TRACE(L"Unknown state %ld\n", pIfInfo->isState);
                break;
    }
	return false;
}
bool software_tool_vnc::get_connect_msg()
{
	WLAN_CONNECTION_ATTRIBUTES *p_connect_net;
	DWORD                  pdwDataSize;
	WlanQueryInterface(hClient,
		&pInterface->InterfaceGuid,
		wlan_intf_opcode_current_connection,
		0,
		&pdwDataSize,
		(PVOID*)&p_connect_net,
		NULL);
	if(pdwDataSize != sizeof(WLAN_CONNECTION_ATTRIBUTES)){
		return false;
	}
	if(memcmp(sSetSN, p_connect_net->wlanAssociationAttributes.dot11Ssid.ucSSID, 
		p_connect_net->wlanAssociationAttributes.dot11Ssid.uSSIDLength) == 0){
		return true;
	}
	return false;
}
bool software_tool_vnc::WLANDisconnect()
{ 
    dwResult = WlanDisconnect(hClient, &pInterface->InterfaceGuid, NULL);
    if(ERROR_SUCCESS != dwResult){
        switch (dwResult){
			case ERROR_INVALID_PARAMETER:    //参数一、二为空
				TRACE(L"Para is NULL\n");
				break;
			case ERROR_INVALID_HANDLE:
				TRACE(L"Failed to INVALID HANDLE \n");
				break; 
			case ERROR_NOT_ENOUGH_MEMORY:
				TRACE(L"Failed to allocate memory \n");
				break;
			case ERROR_ACCESS_DENIED:
				TRACE(L"The caller does not have sufficient permissions. \n");
				break;
			default:
				dwResult = GetLastError();
				TRACE(L"WlanConnect Fail： %wd\n", dwResult);
				break;
        }
		//if (pProfileXml != NULL){
		//	WlanFreeMemory(pProfileXml);	//所有API用到的参数，都要使用API提供的函数去释放
		//	pProfileXml = NULL;
		//}
        free(pConnPara);
		return false;
    }
	return true;
}

bool software_tool_vnc::CloseWLANService()
{
	if(NULL != hClient){
        dwResult = WlanCloseHandle(hClient    //要关闭的连接的客户端句柄
			, NULL);
        if(ERROR_SUCCESS != dwResult){
            switch (dwResult){
				case ERROR_INVALID_PARAMETER:    //参数一为空或参数二不为空
					TRACE(L"Para is NULL\n");
					break;
				case ERROR_INVALID_HANDLE:
					TRACE(L"Failed to INVALID HANDLE \n");
					break; 
				default:
					dwResult = GetLastError();
					TRACE(L"WlanCloseHandle Fail： %wd\n", dwResult);
					break;
            }
			return false;
        }
        hClient = NULL;
    }
 	return true;
}
// software_tool_vnc 消息处理程序

TCHAR* software_tool_vnc::StringToChar(CString& str)
{
	int len=str.GetLength();
	TCHAR* tr=str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr; 
}
CString software_tool_vnc::ExecuteCmd_sh(CString cmd_name, CString cmd_param)
{
	SECURITY_ATTRIBUTES sa;
    HANDLE hRead,hWrite;
 
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
	//sa.bInheritHandle = NULL;
    if (!CreatePipe(&hRead,&hWrite,&sa,0)){
        AfxMessageBox("Error on CreatePipe()!");
        return NULL;
    }
	//TRACE("%s################################################cmd:0x%x,0x%x\r\n",cmd_param,hRead,hWrite);
	
    
	//TRACE("#############################pi:0x%x\r\n",&pi);
	STARTUPINFO si={0};
    si.cb = sizeof( STARTUPINFO ); //结构体大小 
    GetStartupInfo( &si );
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* name=StringToChar(cmd_name);
	TCHAR* param=StringToChar(cmd_param);
	if (!CreateProcess(name,param,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&vnc_sh_pi)){
        AfxMessageBox("Error on CreateProcess()!");
        return NULL;
    }
	CloseHandle(hWrite);    
 
    char buffer[4096];
    memset(buffer,0,4096);
	CString output;
    DWORD byteRead;
    while(true){
		memset(buffer,0,4096);
        if (ReadFile(hRead,buffer,4095,&byteRead,NULL) == NULL){
            break;
        }
		TRACE("%s",buffer);
        output += buffer;
    }
	CloseHandle(hRead);
	return output;
}
CString software_tool_vnc::ExecuteCmd(CString cmd_name, CString cmd_param)
{
	SECURITY_ATTRIBUTES sa;
    HANDLE hRead,hWrite;
 
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
	//sa.bInheritHandle = NULL;
    if (!CreatePipe(&hRead,&hWrite,&sa,0)){
        AfxMessageBox("Error on CreatePipe()!");
        return NULL;
    }
	//TRACE("%s################################################cmd:0x%x,0x%x\r\n",cmd_param,hRead,hWrite);
	
    PROCESS_INFORMATION pi;
	//TRACE("#############################pi:0x%x\r\n",&pi);
	STARTUPINFO si={0};
    si.cb = sizeof( STARTUPINFO ); //结构体大小 
    GetStartupInfo( &si );
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* name=StringToChar(cmd_name);
	TCHAR* param=StringToChar(cmd_param);
	if (!CreateProcess(name,param,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)){
        AfxMessageBox("Error on CreateProcess()!");
        return NULL;
    }
	CloseHandle(hWrite);    
 
    char buffer[4096];
    memset(buffer,0,4096);
	CString output;
    DWORD byteRead;
    while(true){
		memset(buffer,0,4096);
        if (ReadFile(hRead,buffer,4095,&byteRead,NULL) == NULL){
            break;
        }
		TRACE("%s",buffer);
        output += buffer;
    }
	CloseHandle(hRead);
	return output;
}
int software_tool_vnc::process_response_connect(CString response)
{
	CString cut_str;
	int n=response.Find("connected to");
	if(n<0){
		return -1;
	}
	int length=response.Delete(0,n); //找出"List of devices attached"右边的字符串位数
	cut_str = response.Right(length - sizeof("connected to"));   
	if(cut_str.Find(":5555")<0){
		return -1;
	}
	return 0;
}
int software_tool_vnc::if_response_err(CString response)
{
	if(response.Find("error:") >= 0){
		AfxMessageBox("adb设备失去连接");
		return -1;
	}
	if(response.Find("error: device not found") < 0 &&
		response.Find("error: protocol fault") < 0){
		return 0;
	}else{
		init_err_flag = 1;
		AfxMessageBox("adb设备失去连接");
		return -1;
	}
}
int software_tool_vnc::create_app_trans(void *param)
{
	software_tool_vnc *p = (software_tool_vnc*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	cmd_param = " push firmware/vnc/x11vnc /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("x11vnc传输成功! ");
	}

	cmd_param = " push firmware/vnc/libX11.so.6 /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("libX11.so.6传输成功! ");
	}

	cmd_param = " push firmware/vnc/libXau.so.6 /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("libXau.so.6传输成功! ");
	}

	cmd_param = " push firmware/vnc/libxcb.so.1 /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("libxcb.so.1传输成功! ");
	}

	cmd_param = " push firmware/vnc/libXdmcp.so.6 /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("libXdmcp.so.6传输成功! ");
	}

	cmd_param = " push firmware/vnc/libXext.so.6 /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("libXext.so.6传输成功! ");
	}

	cmd_param = " push firmware/vnc/libXfixes.so.3 /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("libXfixes.so.3传输成功! ");
	}

	cmd_param = " push firmware/vnc/libXi.so.6 /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("libXi.so.6传输成功! ");
	}

	cmd_param = " push firmware/vnc/libXtst.so.6 /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("libXtst.so.6传输成功! ");
	}

	cmd_param = " push firmware/vnc/vnc.sh /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("vnc.sh传输成功! ");
	}

	cmd_param = " shell export LD_LIBRARY_PATH=/tmp:$LD_LIBRARY_PATH";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("环境变量设置成功! ");
	}

	cmd_param = " shell chmod +x /tmp/x11vnc";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("x11vnc修改权限成功! ");
	}

	cmd_param = " shell chmod +x /tmp/vnc.sh";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("vnc.sh修改权限成功! ");
	}

	cmd_param = " shell /tmp/vnc.sh";
	response = p->ExecuteCmd_sh(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->AddEvent2List("adb失去连接! ");
		return -1;
	}else{
		p->AddEvent2List("vnc.sh 已运行! ");
	}
	p->init_flag = 1;
	return 0;
}
enum {
	BY_VNC_USB_ADB_DEVICE = 0,
	BY_VNC_USB_PUSH,
	BY_VNC_OPEN_WIFI,
	BY_VNC_WAIT_WIFI,
	BY_VNC_DEVICES,
	BY_VNC_CREATE_TRANS,
	BY_VNC_WAIT_TRANS,
	BY_VNC_SCUCESS,
};
CString ping_test(CString ip);
int software_tool_vnc::WorkerThreadProc(void *param)
{
	CTime tNow = CTime::GetCurrentTime();
	CTime tsav = CTime::GetCurrentTime();
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	software_tool_vnc *p = (software_tool_vnc*)param;
	CString sText;
	CString netsh_cmd;
	//p->main_thread_working = TRUE;
	p->init_flag = 0;
	p->init_err_flag = 0;
	((CButton*)p->GetDlgItem(IDC_BUTTON13))->EnableWindow(FALSE);

	int status;
	if(p->total_machine_by_option_instance.g_connect_type == 1){
		status = BY_VNC_USB_ADB_DEVICE;
	}else{
		status = BY_VNC_OPEN_WIFI;
	}

		while(1){
			tNow = CTime::GetCurrentTime();
			//if(p->quit_thread){
			//	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_vnc::kill_adbd, param, 0, NULL);
			//}
			switch (status){
				case BY_VNC_USB_ADB_DEVICE:
					cmd_param = " devices";
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(tNow.GetTime() - tsav.GetTime() > p->total_machine_by_option_instance.g_wifihot_timeout){
						p->AddEvent2List("usb devices连接超时! ");
						goto out;
					}
					if(p->process_response_devices(response)){
						Sleep(1000);
					}else{
						status = BY_VNC_USB_PUSH;
					}
					break;
				case BY_VNC_USB_PUSH:
					cmd_param = " push firmware/vnc/ap.sh /tmp";
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->if_response_err(response)){
						p->AddEvent2List("adb失去连接! ");
						goto out;
					}else{
						p->AddEvent2List("ap.sh传输成功! ");
					}
					cmd_param = " shell chmod +x /tmp/ap.sh";
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->if_response_err(response)){
						p->AddEvent2List("adb失去连接! ");
						goto out;
					}else{
						p->AddEvent2List("ap.sh权限修改成功! ");
					}

					cmd_param = " push firmware/vnc/hostapd /tmp";
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->if_response_err(response)){
						p->AddEvent2List("adb失去连接! ");
						goto out;
					}else{
						p->AddEvent2List("hostapd传输成功! ");
					}
					cmd_param = " shell chmod +x /tmp/hostapd";
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->if_response_err(response)){
						p->AddEvent2List("adb失去连接! ");
						goto out;
					}else{
						p->AddEvent2List("hostapd权限修改成功! ");
					}

					cmd_param = " push firmware/vnc/hostapd.conf /tmp";
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->if_response_err(response)){
						p->AddEvent2List("adb失去连接! ");
						goto out;
					}else{
						p->AddEvent2List("hostapd.conf传输成功! ");
					}

					cmd_param = " shell /tmp/ap.sh";
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->if_response_err(response)){
						p->AddEvent2List("adb失去连接! ");
						goto out;
					}else{
						p->AddEvent2List("热点程序已运行，断开usb! ");
					}
					status = BY_VNC_OPEN_WIFI;
					Sleep(2000);
					break;
				case BY_VNC_OPEN_WIFI:
					sText.Format("%s",p->sSetSN);
					p->target = sText;
					p->targetKey = p->total_machine_by_option_instance.g_wifihot_passwd;
					if(p->SearchNet()!=true){
						p->AddEvent2List("网络连接失败! ");
						goto out;
					}
					netsh_cmd = "netsh interface ip set address ";
					netsh_cmd += p->total_machine_by_option_instance.g_local_net_name;
					netsh_cmd += " static ";
					netsh_cmd += p->total_machine_by_option_instance.g_local_ip;
					netsh_cmd += " 255.255.255.0";
					WinExec(netsh_cmd,SW_HIDE);
					p->AddEvent2List("网络连接成功! ");
					status = BY_VNC_WAIT_WIFI;
					break;
				case BY_VNC_WAIT_WIFI:
					if(p->get_wifi_status() && p->get_connect_msg()){
						int ping_right_times = 0;
						for(int i=0; i<4; i++){
							response = ping_test(p->total_machine_by_option_instance.g_wifihot_ip);
							if(response.Find("Reply from") >= 0 &&
								response.Find("bytes=") >= 0 &&
								response.Find("time=") >= 0 &&
								response.Find("TTL=") >= 0){
								ping_right_times ++;
							}
						}
						if(ping_right_times >= 4){
							tsav = tNow;
							p->AddEvent2List("ping成功! ");
							status = BY_VNC_DEVICES;				
						}
					}
					if(tNow.GetTime() - tsav.GetTime() > p->total_machine_by_option_instance.g_wifihot_timeout){
						p->AddEvent2List("wifi连接超时! ");
						goto out;
					}
					break;
				case BY_VNC_DEVICES:
					if(tNow.GetTime() - tsav.GetTime() > p->total_machine_by_option_instance.g_wifihot_timeout){
						p->AddEvent2List("adb连接超时! ");
						goto out;
					}
					cmd_param = " connect ";
					cmd_param += p->total_machine_by_option_instance.g_wifihot_ip;
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->process_response_connect(response)){
						Sleep(1000);
					}else{
						//cmd_param = " shell /etc/init.d/S50servermanager stop";
						//response = p->ExecuteCmd(cmd_str, cmd_param);
						//if(p->if_response_err(response)){
						//	p->AddEvent2List("adb失去连接! ");
						//	goto out;
						//}
						status = BY_VNC_CREATE_TRANS;
						p->AddEvent2List("adb连接成功! ");
						( (CButton*)p->GetDlgItem(IDC_BUTTON22) )->EnableWindow(TRUE);
					}
					break;
				case BY_VNC_CREATE_TRANS:
					p->trans_hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_vnc::create_app_trans, param, 0, NULL);
					status = BY_VNC_WAIT_TRANS;
					break;
				case BY_VNC_WAIT_TRANS:
					if(p->init_err_flag){
						p->AddEvent2List("失败! ");
						goto out;
					}
					if(p->init_flag){
						status = BY_VNC_SCUCESS;
					}
					break;
				case BY_VNC_SCUCESS:
					p->AddEvent2List("成功! ");
					goto out;
					break;
				default:

					break;
			}
			Sleep(1);
		}

out:
	if(p->quit_thread){
		return 0;
	}
	p->WLANDisconnect();
	p->CloseWLANService();	
	((CButton*)p->GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
	//p->main_thread_working = FALSE;
	return 0;
}
int software_tool_vnc::process_response_devices(CString response)
{
	CString cut_str;
	int n=response.Find("List of devices attached");
	if(n<0){
		return -1;
	}
	int length=response.Delete(0,n); //找出"List of devices attached"右边的字符串位数
	cut_str = response.Right(length - sizeof("List of devices attached"));   
	if(cut_str.Find("device")<0){
		return -1;
	}
	return 0;
}
void software_tool_vnc::SetAllItemRestart(void)
{
	CString sText;
	m_EditSn.SetWindowTextA("");
	m_listEvt.ResetContent();
	m_EditSn.SetFocus();

	((CButton*)GetDlgItem(IDC_EDIT20))->SetWindowTextA("192.168.12.1");
	((CButton*)GetDlgItem(IDC_EDIT38))->SetWindowTextA("123456789");
	((CButton*)GetDlgItem(IDC_EDIT39))->SetWindowTextA("15");
	((CButton*)GetDlgItem(IDC_EDIT41))->SetWindowTextA("无线网络连接");
	((CButton*)GetDlgItem(IDC_EDIT42))->SetWindowTextA("192.168.12.127");

	((CButton*)GetDlgItem(IDC_BUTTON22))->EnableWindow(FALSE);
	AddEvent2List("#等待用户输入sn号#");
}

void software_tool_vnc::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	SetAllItemRestart();
}
void software_tool_vnc::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	//total_machine_by_option_instance = g_p->total_machine_by_option_instance;
}

extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_tool_vnc::OnEnChangeSetSn()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString sText,sText1;
	UpdateData(TRUE);
	m_EditSn.GetWindowTextA(sText);
	if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		if( sText.Find("-") < 0 ){
			if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("当前产品SN号(v2)不正确");
			}else{
				GetDlgItem(IDC_EDIT39)->GetWindowTextA(sText1);
				StrToIntEx(sText1, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_wifihot_timeout);
				GetDlgItem(IDC_EDIT20)->GetWindowTextA(total_machine_by_option_instance.g_wifihot_ip);
				GetDlgItem(IDC_EDIT38)->GetWindowTextA(total_machine_by_option_instance.g_wifihot_passwd);
				GetDlgItem(IDC_EDIT41)->GetWindowTextA(total_machine_by_option_instance.g_local_net_name);
				GetDlgItem(IDC_EDIT42)->GetWindowTextA(total_machine_by_option_instance.g_local_ip);
				total_machine_by_option_instance.g_connect_type = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();//1 usb
				
				memset((void*)sSetSN, 0, sizeof(sSetSN));
				memcpy((void*)sSetSN, sText, 18);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_vnc::WorkerThreadProc, this, 0, NULL);
				AddEvent2List("sn号输入成功，准备连接");
			}
		}
	}
	if(sText.GetLength() >= 19){
		if(CheckSN(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
			AfxMessageBox("当前产品SN号(v1)不正确");
		}else{
				GetDlgItem(IDC_EDIT39)->GetWindowTextA(sText1);
				StrToIntEx(sText1, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_wifihot_timeout);
				GetDlgItem(IDC_EDIT20)->GetWindowTextA(total_machine_by_option_instance.g_wifihot_ip);
				GetDlgItem(IDC_EDIT38)->GetWindowTextA(total_machine_by_option_instance.g_wifihot_passwd);
				GetDlgItem(IDC_EDIT41)->GetWindowTextA(total_machine_by_option_instance.g_local_net_name);
				GetDlgItem(IDC_EDIT42)->GetWindowTextA(total_machine_by_option_instance.g_local_ip);
				total_machine_by_option_instance.g_connect_type = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();//1 usb

			memset((void*)sSetSN, 0, sizeof(sSetSN));
			memcpy((void*)sSetSN, sText, 19);
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_vnc::WorkerThreadProc, this, 0, NULL);
			AddEvent2List("sn号输入成功，准备连接");
		}

	}
	// TODO:  在此添加控件通知处理程序代码
}

HANDLE software_tool_vnc::StartProcess(LPCTSTR program, LPCTSTR args)
{
     //HANDLE hProcess = NULL;
     STARTUPINFO startupInfo;
	 PROCESS_INFORMATION vnc_processInfo;
     ::ZeroMemory(&startupInfo, sizeof(startupInfo));
     startupInfo.cb = sizeof(startupInfo);
     if(::CreateProcess(program, (LPTSTR)args, 
                        NULL,  // process security
                        NULL,  // thread security
                        FALSE, // no inheritance
                        0,     // no startup flags
                        NULL,  // no special environment
                        NULL,  // default startup directory
                        &startupInfo,
                        &vnc_processInfo))
						//TRACE("#################id = %d\r\n", vnc_processInfo.dwProcessId);
		return vnc_processInfo.hProcess;
	 else
		return NULL;
}

int  software_tool_vnc::WorkerThreadProcvnc(void *param)
{
	CWnd *pWnd = NULL;
	HWND apphWnd ,static_wnd;
	CRect rect;
	software_tool_vnc *glb_dlg = (software_tool_vnc *)param;
	static_wnd = glb_dlg->GetDlgItem(IDC_STATIC)->m_hWnd;
	glb_dlg->GetDlgItem(IDC_STATIC)->GetWindowRect(&rect);
	glb_dlg->ScreenToClient(rect);  
	CRect rectShow;
	rectShow.left = 0;
	rectShow.top = 0;
	rectShow.right = rect.right - rect.left;
	rectShow.bottom = rect.bottom - rect.top;
	apphWnd = glb_dlg->vnc_hWnd;
	int times = 0;
	HANDLE vnc_hWnd_arr[10];
	while(!glb_dlg->quit_thread){
		if (IsWindow(apphWnd)){
		}else{
			if ( !IsWindow(static_wnd) ){
				break;
			}
			vnc_hWnd_arr[times] = glb_dlg->StartProcess("thirdapp\\tightvnc-1.3.10_x86_viewer\\vncviewer.exe","");
			times++;
			do{
				apphWnd =::FindWindow(_T("#32770"), _T("New TightVNC Connection") );
			} while (apphWnd == NULL);
			glb_dlg->vnc_hWnd = apphWnd;
			::SetParent(apphWnd,glb_dlg->GetDlgItem(IDC_STATIC)->m_hWnd);
			LONG style=GetWindowLong(apphWnd,GWL_STYLE);// 14CF 0000  
			style &=~WS_CAPTION;  
			style &=~WS_THICKFRAME;  
			style |=WS_VISIBLE;

			SetWindowLong(apphWnd, GWL_STYLE, style);//eraze title of ms paint window.
			::MoveWindow(apphWnd, rectShow.left, rectShow.top,rectShow.right, rectShow.bottom, true);
		}
		Sleep(200);
	}
	for(int i=0; i<times; i++)
		//TerminateProcess(glb_dlg->vnc_processInfo.hProcess,0);
		TerminateProcess(vnc_hWnd_arr[i],0);
	return 0;
}

int software_tool_vnc::WorkerThreadProcdisplay(void *param)
{
	HWND apphWnd = NULL;
	CRect rect;
	software_tool_vnc *glb_dlg = (software_tool_vnc *)param;
	glb_dlg->GetDlgItem(IDC_STATIC2)->GetWindowRect(&rect);
	glb_dlg->ScreenToClient(rect);  
	CRect rectShow;
	rectShow.left = 0;
	rectShow.top = 0;
	rectShow.right = rect.right - rect.left;
	rectShow.bottom = rect.bottom - rect.top;
	while(!glb_dlg->quit_thread){
		do{
			if ( glb_dlg->quit_thread ){
				goto out;
			}
			apphWnd =::FindWindow("VNCviewer","x11vnc");
			Sleep(10);

		} while (apphWnd == NULL);
		::SetParent(apphWnd,glb_dlg->GetDlgItem(IDC_STATIC2)->m_hWnd);
		LONG style=GetWindowLong(apphWnd,GWL_STYLE);// 14CF 0000  
		style &=~WS_CAPTION;  
		style &=~WS_THICKFRAME;  
		style |=WS_VISIBLE;

		SetWindowLong(apphWnd, GWL_STYLE, style);//eraze title of ms paint window.
		::MoveWindow(apphWnd, rectShow.left, rectShow.top,rectShow.right, rectShow.bottom, true);
		apphWnd = NULL;
	}
out:
	return 0;
}
int software_tool_vnc::need_file_ifexit()
{
	if(PathFileExists("thirdapp\\adb\\adb.exe") != TRUE){
		AfxMessageBox("错误：程序路径下未找到adb.exe");
		return 1;
	}

	if(PathFileExists("firmware\\vnc\\ap.sh") != TRUE){
		AfxMessageBox("错误：程序路径下未找到ap.sh");
		return 1;
	}
	if(PathFileExists("firmware\\vnc\\hostapd") != TRUE){
		AfxMessageBox("错误：程序路径下未找到hostapd");
		return 1;
	}
	if(PathFileExists("firmware\\vnc\\hostapd.conf") != TRUE){
		AfxMessageBox("错误：程序路径下未找到hostapd.conf");
		return 1;
	}
	if(PathFileExists("firmware\\vnc\\libX11.so.6") != TRUE){
		AfxMessageBox("错误：程序路径下未找到libX11.so.6");
		return 1;
	}
	if(PathFileExists("firmware\\vnc\\libXau.so.6") != TRUE){
		AfxMessageBox("错误：程序路径下未找到libXau.so.6");
		return 1;
	}
	if(PathFileExists("firmware\\vnc\\libxcb.so.1") != TRUE){
		AfxMessageBox("错误：程序路径下未找到libxcb.so.1");
		return 1;
	}
	if(PathFileExists("firmware\\vnc\\libXdmcp.so.6") != TRUE){
		AfxMessageBox("错误：程序路径下未找到libXdmcp.so.6");
		return 1;
	}

	if(PathFileExists("firmware\\vnc\\libXext.so.6") != TRUE){
		AfxMessageBox("错误：程序路径下未找到libXext.so.6");
		return 1;
	}
	if(PathFileExists("firmware\\vnc\\libXfixes.so.3") != TRUE){
		AfxMessageBox("错误：程序路径下未找到libXfixes.so.3");
		return 1;
	}

	if(PathFileExists("firmware\\vnc\\libXi.so.6") != TRUE){
		AfxMessageBox("错误：程序路径下未找到libXi.so.6");
		return 1;
	}
	if(PathFileExists("firmware\\vnc\\libXtst.so.6") != TRUE){
		AfxMessageBox("错误：程序路径下未找到libXtst.so.6");
		return 1;
	}
	if(PathFileExists("firmware\\vnc\\vnc.sh") != TRUE){
		AfxMessageBox("错误：程序路径下未找到vnc.sh");
		return 1;
	}
	if(PathFileExists("firmware\\vnc\\x11vnc") != TRUE){
		AfxMessageBox("错误：程序路径下未找到x11vnc");
		return 1;
	}

	return 0;
}
BOOL software_tool_vnc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString soft_name;
	soft_name += "vnc测试工具";
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	//main_thread_working = false;
	quit_thread = false;

	SetAllItemRestart();
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_vnc::WorkerThreadProcvnc, this, 0, NULL);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_vnc::WorkerThreadProcdisplay, this, 0, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
}


BOOL software_tool_vnc::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

int software_tool_vnc::kill_adbd(void *param)
{
	software_tool_vnc *p = (software_tool_vnc*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	cmd_param = " shell killall adbd";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	return 0;
}

void software_tool_vnc::OnBnClickedButton22()
{
	// TODO: 在此添加控件通知处理程序代码
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_vnc::kill_adbd, this, 0, NULL);
	//TerminateProcess(vnc_sh_pi.hProcess,0);
}


void software_tool_vnc::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	quit_thread = true;
	//while(main_thread_working){
	//	;
	//}
	CDialogEx::OnClose();
}


//void software_tool_vnc::OnCancel()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	quit_thread = true;
//	CDialogEx::OnCancel();
//}
