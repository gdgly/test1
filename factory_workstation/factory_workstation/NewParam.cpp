

#include "afxcmn.h"
#include "stdafx.h"
//#include "MProduction.h"
#include "software_burn.h"
/**************************************************************************************
二、为BLE定制到一个特征字中，将长度减少到20个字节以内
	SN=ZYFYD00-A1-0100000a，长度为19个字节
规则：ZY：　　　启始2个字节ZY不变
　　　FYD：　　  产品型号，不同的型号可能会代表不同的方案
　　　00：　　　产品子型号（16进制）
　　　0A：　　   年月(YM) (16进制）(18年10月，年份从2018年开始)
　　　01：　　　批号　（16进制)，修改了硬件后就会改
　　　00000a：　 序号    （16进制）使用蓝牙自带的白名单机制[可到达100万]
**************************************************************************************/


extern UINT    g_iYearMon, g_iBatchNo;            // 配置日期及批号
#define OFFSET_BTADDRESS            (0)
#define OFFSET_SERIALNUMBER         (529)

CString get_firmwarefile_version(CString file_name)
{
	CString f_ver = "ok";
	FILE  *pFile;
	int OffSet = 0x5a00c;
	char buf[8];
	memset(buf,0,8);
	//pFile =fopen(file_name.GetBuffer(),"rb+");   //读操作
	fopen_s(&pFile, file_name.GetBuffer(),"rb+");
	if(pFile==NULL){
		AfxMessageBox("固件文件打开失败，检查固件文件是否被占用");
		TRACE("open firmware file failed !\n");
		f_ver = "failed";
		return f_ver;
	}
	fseek(pFile, OffSet,SEEK_SET);
	fread(buf,7,1, pFile);
	fclose(pFile);
	f_ver.Format("%s",buf);
	return f_ver;
}
#define SNv2_VER_CHECK_VALUE (0*16*16*16*16 + 0*16*16*16 + 3*16*16 + 2*16 + 5)
bool check_firmware_canuse_v2sn(CString g_wrFirm)
{
	CString sText = get_firmwarefile_version(g_wrFirm);//0.03.25版本后可用v2 sn
	unsigned char a[5];
	int i;
	unsigned int check_value;
	if(sText == "failed"){
		return FALSE;
	}
	unsigned char*p = (unsigned char*)sText.GetBuffer();
	a[0] = (*(p));
	a[1] = (*(p+2));
	a[2] = (*(p+3));
	a[3] = (*(p+5));
	a[4] = (*(p+6));
	for(i=0; i<5; i++){
		if( a[i]>='0' && a[i]<='9' ){
			a[i] = a[i] - 48;
		}else if( a[i]>='a' && a[i]<='f' ){
			a[i] = a[i] - 97;
		}else if( a[i]>='A' && a[i]<='F' ){
			a[i] = a[i] - 65;
		}
	}
	check_value = a[0]*16*16*16*16 + a[1]*16*16*16 + a[2]*16*16 + a[3]*16 + a[4];
	if(check_value >= SNv2_VER_CHECK_VALUE){
		return TRUE;
	}else{
		AfxMessageBox("固件文件版本不支持此sn号");
		return FALSE;
	}
}


BYTE hexstr2int(BYTE *data)
{
	BYTE i,tmp=0,result=0;

	for(i = 0;i<2;i++){
		if(*data>='A'&& *data<= 'Z')
			tmp = *data-'A'+10;
        else if(*data>='a'&& *data<= 'z')
            tmp = *data-'a'+10;
        else
            tmp = *data-'0';

        result*=16;
        result+=tmp;
        data++;
    }
    return result;
}

INT BtAddrFormat(MPIPtr mpi, BYTE *sBtAddr)
{
	sBtAddr[5] = 0xCA;
	sBtAddr[4] = hexstr2int(&mpi->sSetSN[8]);

	sBtAddr[3] = hexstr2int(&mpi->sSetSN[11]);

	sBtAddr[2] = hexstr2int(&mpi->sSetSN[13]);
	sBtAddr[1] = hexstr2int(&mpi->sSetSN[15]);
	sBtAddr[0] = hexstr2int(&mpi->sSetSN[17]);
	return 0;
}
static int UnicodeToUtf8(const char* unicode, int size, char* utf8)
{
	wchar_t* wstr = new wchar_t[size/2+1];
	memcpy(wstr, unicode, size);
	int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return len;
}

static int GB2312ToUtf8(const char* gb2312, char* utf8)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return len;
}

CString ConvertGB2312ToUtf8(CString strGBK) 
{
    CString resText; 
    int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0);
    unsigned short * wszUtf8 = new unsigned short[len+1];
    memset(wszUtf8, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len);
	
    len = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)wszUtf8, -1, NULL, 0, NULL, NULL); 
    char *szUtf8=new char[len + 1];
    memset(szUtf8, 0, len + 1);
    WideCharToMultiByte (CP_UTF8, 0, (LPCWCH)wszUtf8, -1, szUtf8, len, NULL,NULL);
    resText= szUtf8;
    delete[] szUtf8;
    delete[] wszUtf8;
    return resText;
}

INT CreateParamFile(CString src, CString dst, BYTE* btAddress, BYTE* sn)
{
	CString sText;
	BYTE param[4096];
	INT prmLen;
	INT ret = -2; 
	CFile hSrc, hDst;
	
//	const char *bufv1="小豹AI翻译棒录音版";
//	const char *bufv2="小豹AI翻译棒lite版";
	const char *bufv1="小豹翻译棒录音版";
	const char *bufv2="小豹翻译棒青春版";
	char szU8[100]={0};
	int len;

	hSrc.m_hFile = hDst.m_hFile = INVALID_HANDLE_VALUE;

	if(hSrc.Open(src, CFile::typeBinary | CFile::modeRead) != TRUE) {
		TRACE("Error Open:%s\n", src);
		AfxMessageBox("nvparam 文件打开失败，检查文件是否存在 ");
		goto out;
	}
	prmLen = hSrc.Read(param, sizeof(param));
	if(prmLen <= 20) {
		TRACE("Error PrmSize:%d\n", prmLen);
		goto out;
	}

	if(hDst.Open(dst, CFile::typeBinary | CFile::modeCreate | CFile::modeReadWrite) != TRUE) {
		TRACE("Error Open:%s\n", dst);
		goto out;
	}

	memcpy((void*)&param[OFFSET_BTADDRESS], btAddress, 6);       ////6个字节
	memcpy((void*)&param[OFFSET_SERIALNUMBER], sn, 19);
	
	sText.Format("%s",sn);
	if( sText.Find("-") < 0 ){
		if( *sn == 'B'		&&
			*(sn+1) == '0'	&&
			*(sn+2) == '1'	&&
			*(sn+3) == 'A'
			){
			len = GB2312ToUtf8((char *)bufv1,szU8+2);
			szU8[0] = len;
			szU8[1] = 0x00;
			memcpy((void*)&param[256], szU8, len+2);
		}
		if( *sn == 'B'		&&
			*(sn+1) == '0'	&&
			*(sn+2) == '2'	&&
			*(sn+3) == 'A'
			){
			len = GB2312ToUtf8((char *)bufv2,szU8+2);
			szU8[0] = len;
			szU8[1] = 0x00;
			memcpy((void*)&param[256], szU8, len+2);
		}
		prmLen = OFFSET_SERIALNUMBER + 18;
	}else{
		prmLen = OFFSET_SERIALNUMBER + 19;
	}
	hDst.Write(param, prmLen);
	ret = 0;
	TRACE("NewParamFile SUCC:%s\n", dst);

out:
	if(INVALID_HANDLE_VALUE != hSrc.m_hFile) hSrc.Close();
	if(INVALID_HANDLE_VALUE != hDst.m_hFile) hDst.Close();
	return ret;
}

INT CreateParamFile(CString src, CString dst,  MPIPtr mpi)
{
	BYTE sBtAddr[8];

	memset(mpi->sNoSN, 0, sizeof(mpi->sNoSN));
	memset(mpi->sBtAddr, 0, sizeof(mpi->sBtAddr));

	memset(sBtAddr, 0, sizeof(sBtAddr));

//	sprintf_s((char*)mpi->sNoSN, sizeof(mpi->sNoSN), "ZYFYD00-%02X-%02X%06x", g_iYearMon, g_iBatchNo, mpi->iNoSn);
	BtAddrFormat(mpi, sBtAddr);

	return CreateParamFile(src, dst, sBtAddr, mpi->sSetSN);
}

INT CreateParamFile(CString src, MPIPtr mpi)
{
	CString dest;

	dest.Format("%s.%d", src, mpi->ch);

	return CreateParamFile(src, dest, mpi);
}

BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count)
{
	UINT iNo = 0;
	if(sn.IsEmpty())
		return FALSE;
	if(sn.GetLength() != 18)
		return FALSE;

	if(sn.Find(sPrompt) != 0)
		return FALSE;

	CString sNo = sn.Right(6);
	if(FALSE == StrToIntEx(sNo, STIF_SUPPORT_HEX, (int*)&iNo))
		return FALSE;

	if(iNo >= iStart && iNo <= (iStart+count))
		return TRUE;

	return FALSE;
}

BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count)
{
	UINT iNo = 0;
	if(sn.IsEmpty())
		return FALSE;
	int b = sn.GetLength();
	if(sn.GetLength() != 19)
		return FALSE;

	if(sn.Find(sPrompt) != 0)
		return FALSE;

	CString sNo = sn.Right(6);
	if(FALSE == StrToIntEx(sNo, STIF_SUPPORT_HEX, (int*)&iNo))
		return FALSE;

	if(iNo >= iStart && iNo <= (iStart+count))
		return TRUE;

	return FALSE;
}





