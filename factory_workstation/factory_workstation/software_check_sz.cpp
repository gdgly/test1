// software_check_sz.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_check_sz.h"
#include "afxdialogex.h"


// software_check_sz �Ի���

IMPLEMENT_DYNAMIC(software_check_sz, CDialogEx)

software_check_sz::software_check_sz(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_check_sz::IDD, pParent)
{

}

software_check_sz::~software_check_sz()
{
}

void software_check_sz::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listEvt);
	DDX_Control(pDX, IDC_EDIT2, m_EditSn);
	DDX_Control(pDX, IDC_EDIT_FIRMVER2, m_fw_version);
	DDX_Control(pDX, IDC_EDIT_FIRMVER3, m_btaddr);
	DDX_Control(pDX, IDC_STC_BOOT, m_btnBoot);
	DDX_Control(pDX, IDC_STC_BOOT2, m_light);
	DDX_Control(pDX, IDC_STC_BOOT3, m_tap);
	DDX_Control(pDX, IDC_STC_BOOT4, m_plc);
	DDX_Control(pDX, IDC_STC_BOOT5, m_apollo);
	DDX_Control(pDX, IDC_STC_BOOT12, m_bonemic);
	DDX_Control(pDX, IDC_STC_BOOT6, m_get_ps);
	DDX_Control(pDX, IDC_STC_BOOT7, m_set_ps);
	DDX_Control(pDX, IDC_STC_BOOT8, m_wait_light);
	DDX_Control(pDX, IDC_STC_BOOT9, m_wait_tap);
	DDX_Control(pDX, IDC_STC_BOOT10, m_wait_plc);
	DDX_Control(pDX, IDC_STC_BOOT11, m_wait_apollo);
	DDX_Control(pDX, IDC_STC_BOOT25, m_wait_openusb);
	DDX_Control(pDX, IDC_STC_BOOT26, m_wait_closeusb);
	DDX_Control(pDX, IDC_AUDIO_SEC3, m_play_db);
	DDX_Control(pDX, IDC_AUDIO_DB_RIGHT, m_rec_right_db);
	DDX_Control(pDX, IDC_AUDIO_DB_LEFT, m_rec_left_db);
	DDX_Control(pDX, IDC_RECPLAY_SECOND, m_recplay_sec);
	DDX_Control(pDX, IDC_PLAYER_SECOND, m_player_sec);
	DDX_Control(pDX, IDC_STATIC9, m_Epass);
}


BEGIN_MESSAGE_MAP(software_check_sz, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &software_check_sz::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &software_check_sz::OnEnChangeEdit2)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_DEV_ERROR, &software_check_sz::OnDevCtrlError)
	ON_MESSAGE(WM_DEV_REPORT, &software_check_sz::OnDevCtrlReport)
END_MESSAGE_MAP()

void software_check_sz::AddEvent2List(CString sText)
{
	CTime tNow = CTime::GetCurrentTime();
	CString sTemp;
	int count = m_listEvt.GetCount();
	sTemp.Format("%02d:%02d:%02d %s", tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond(), sText);
	m_listEvt.InsertString(count, sTemp);
	m_listEvt.SetCurSel( m_listEvt.GetCount() - 1);

	if(m_hLogFile.m_hFile != INVALID_HANDLE_VALUE) {
		m_hLogFile.Write(sTemp, sTemp.GetLength());
		m_hLogFile.Write("\r\n", 2); m_hLogFile.Flush();
	}
}

// software_check_sz ��Ϣ�������
void software_check_sz::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	check_sz_option_instance = g_p->check_sz_option_instance;
}

void software_check_sz::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sText;
	err_flag = 0;
	check_init_num = 0;
	pc_record_flag = 0;
	em20168_init_ok = 0;
	m_EditSn.SetWindowTextA("");
	pass_green_color = 0;
	m_Epass.SetWindowTextA("");
	m_fw_version.SetWindowTextA("");
	m_btaddr.SetWindowTextA("");

	m_btnBoot.SetColor(STATIC_WND_DISABLE);
	m_light.SetColor(STATIC_WND_DISABLE);
	m_tap.SetColor(STATIC_WND_DISABLE);
	m_plc.SetColor(STATIC_WND_DISABLE);
	m_apollo.SetColor(STATIC_WND_DISABLE);
	m_bonemic.SetColor(STATIC_WND_DISABLE);
	m_get_ps.SetColor(STATIC_WND_DISABLE);
	m_get_ps.SetWindowTextA("");
	m_set_ps.SetColor(STATIC_WND_DISABLE);
	m_wait_light.SetColor(STATIC_WND_DISABLE);
	m_wait_tap.SetColor(STATIC_WND_DISABLE);
	m_wait_plc.SetColor(STATIC_WND_DISABLE);
	m_wait_apollo.SetColor(STATIC_WND_DISABLE);
	m_wait_openusb.SetColor(STATIC_WND_DISABLE);
	m_wait_closeusb.SetColor(STATIC_WND_DISABLE);

	m_play_db.SetWindowTextA("");
	m_rec_right_db.SetWindowTextA("");
	m_rec_left_db.SetWindowTextA("");
	m_recplay_sec.SetWindowTextA("");
	m_player_sec.SetWindowTextA("");

	m_EditSn.SetFocus();
	m_listEvt.ResetContent();
	AddEvent2List("#####�ȴ��û�����sn��#####");

	//UINT32 m_devHandle = openTestEngine(USBDBG, "1", 0, 5000, 1000);
	//TRACE("handle = %d", m_devHandle);

	//UINT32 m_devHandle1 = openTestEngine(USBDBG, "1", 0, 5000, 1000);
	//TRACE("handle = %d", m_devHandle1);

	//closeTestEngine(1);
	//closeTestEngine(2);
	//closeTestEngine(3);
	//closeTestEngine(4);
}

LRESULT software_check_sz::OnDevCtrlError(WPARAM wParam, LPARAM lParam)
{
	CString sText,stmp;
	switch(wParam){
		case ERROR_GETPORT:
			sText = "ERROR_GETPORT";
			break;
		case ERROR_GETPORT_NOPORT:
			sText = "ERROR_GETPORT_NOPORT";
			break;
		case ERROR_OPENTRANS:
			sText = "ERROR_OPENTRANS";
			break;
		case ERROR_OPEN_FLASHFILE:
			sText = "ERROR_OPEN_FLASHFILE";
			break;
		case ERROR_ERASE_START:
			sText = "ERROR_ERASE_START";
			break;
		case ERROR_ERASE_SETBANK:
			sText = "ERROR_ERASE_SETBANK";
			break;
		case ERROR_PROGRAM_START:
			sText = "ERROR_PROGRAM_START";
			break;
		case ERROR_PROGRAM_FAIL:
			sText = "ERROR_PROGRAM_FAIL";
			break;
		case ERROR_FLASH_TIMEOUT:
			sText = "ERROR_FLASH_TIMEOUT";
			break;
		case ERROR_OPEN_ENGINE:
			sText = "ERROR_OPEN_ENGINE";
			break;
		case ERROR_CACHE_INIT:
			sText = "ERROR_CACHE_INIT";
			break;
		case ERROR_CACHE_READ:
			sText = "ERROR_CACHE_READ";
			break;
		case ERROR_READ_DEVNAME:
			sText = "ERROR_READ_DEVNAME";
			break;
		case ERROR_READ_BDADDR:
			sText = "ERROR_READ_BDADDR";
			break;
		case ERROR_WRITE_BTADDR:
			sText = "ERROR_WRITE_BTADDR";
			break;
		case ERROR_WRITE_DEVNAME:
			sText = "ERROR_WRITE_DEVNAME";
			break;
		case ERROR_WRITE_CACHE:
			sText = "ERROR_WRITE_CACHE";
			break;
		case ERROR_WRITE_FIXPARAM:
			sText = "ERROR_WRITE_FIXPARAM";
			break;
		case ERROR_APOLLO:
			sText = "ERROR_APOLLO";
			break;
		case ERROR_READ_RECORD:
			sText = "ERROR_READ_RECORD";
			break;
		case ERROR_PLAY_TONE:
			sText = "ERROR_PLAY_TONE";
			stmp.Format("%s", (char*)lParam);
			sText += " ";
			sText += stmp;
			break;
		case ERROR_READ_SENSOR:
			sText = "ERROR_READ_SENSOR";
			AddEvent2List("��ȡpsֵ����");
			break;
		case ERROR_WRITE_SENSOR:
			sText = "ERROR_WRITE_SENSOR";
			AddEvent2List("����psֵ����");
			break;
		case ERROR_WAKEUP:
			sText = "ERROR_WAKEUP";
			stmp.Format("%s", (char*)lParam);
			if(stmp.Find("TimeOut check") >= 0){
				AddEvent2List("�����޼�ⳬʱ��");
				m_wait_apollo.SetColor(STATIC_WND_WARNING);
			}
			break;
		case ERROR_SENSOR:
			sText = "ERROR_SENSOR ";
			stmp.Format("%s", (char*)lParam);
			if(stmp.Find("TimeOut check") >= 0){
				AddEvent2List("�ӽ����ⳬʱ��");
				m_wait_light.SetColor(STATIC_WND_WARNING);
			}
			break;
		case ERROR_PLC:
			sText = "ERROR_PLC";
			stmp.Format("%s", (char*)lParam);
			if(stmp.Find("TimeOut check") >= 0){
				AddEvent2List("plc��ⳬʱ��");
				m_wait_plc.SetColor(STATIC_WND_WARNING);
			}
			break;
		case ERROR_TAP:
			sText = "ERROR_TAP";
			stmp.Format("%s", (char*)lParam);
			if(stmp.Find("TimeOut check") >= 0){
				AddEvent2List("�û���ⳬʱ��");
				m_wait_tap.SetColor(STATIC_WND_WARNING);
			}
			break;
		case ERROR_COMMU_FAIL:
			sText = "ERROR_COMMU_FAIL";
			break; 
	
	}
	sText = "ERROR: err msg = " + sText;
	AddEvent2List(sText);
	m_devCtrl.Stop();
	err_flag = 1;
	return 0;
}

void software_check_sz::set_init_check_status(LPARAM lParam)
{
	CString sText;
	int length;
	CString value_str;
	sText.Format("%s", (char*)lParam);
	if( (length = sText.Find("check RIGHTVERSION")) >= 0){
		value_str = " ";
		value_str += sText.Mid( length+sizeof("check RIGHTVERSION") );
		m_fw_version.SetWindowTextA(value_str);
	}
	if(check_sz_option_instance.g_if_check_apollo){
		if( sText.Find("check APOLLO ") >= 0){
			if( sText.Find("PASS") >= 0){
				m_apollo.SetColor(STATIC_WND_SUCCESS);
			}else{
				m_apollo.SetColor(STATIC_WND_WARNING);
				err_flag = 1;
			}
			check_init_num++;
		}
	}
	if(check_sz_option_instance.g_if_check_light){
		if( sText.Find("check EM20168") >= 0 ){
			if( sText.Find("PASS") >= 0){
				AddEvent2List("��⵽20168��ʼ���ɹ���");
				em20168_init_ok = 1;
			}else{
				AddEvent2List("��⵽20168��ʼ��ʧ�ܣ�");
				em20168_init_ok = 0;
			}
		}
	}
	if(check_sz_option_instance.g_if_check_light){
		if( sText.Find("check UCS146E0") >= 0 ){
			if( sText.Find("PASS") >= 0){
				m_light.SetColor(STATIC_WND_SUCCESS);
			}else{
				if(em20168_init_ok == 1){
					m_light.SetColor(STATIC_WND_SUCCESS);
				}else{
					m_light.SetColor(STATIC_WND_WARNING);
					err_flag = 1;
				}
			}
			check_init_num++;
		}
	}
	if(check_sz_option_instance.g_if_check_plc){
		if( sText.Find("check MAX20340") >= 0){
			if( sText.Find("PASS") >= 0){
				m_plc.SetColor(STATIC_WND_SUCCESS);
			}else{
				m_plc.SetColor(STATIC_WND_WARNING);
				err_flag = 1;
			}
			check_init_num++;
		}
	}
	if(check_sz_option_instance.g_if_check_tap){
		if( sText.Find("check LIS2DW12") >= 0){
			if( sText.Find("PASS") >= 0){
				m_tap.SetColor(STATIC_WND_SUCCESS);
			}else{
				m_tap.SetColor(STATIC_WND_WARNING);
				err_flag = 1;
			}
			check_init_num++;
		}
	}
	if(check_sz_option_instance.g_if_check_bonemic){
		if( sText.Find("check LIS25") >= 0){
			if( sText.Find("PASS") >= 0){
				m_bonemic.SetColor(STATIC_WND_SUCCESS);
			}else{
				m_bonemic.SetColor(STATIC_WND_WARNING);
				err_flag = 1;
			}
			check_init_num++;
		}
	}
	if( (length = sText.Find("check BTADDR")) >= 0){
		value_str = " ";
		value_str += sText.Mid( length+sizeof("check BTADDR") );
		m_btaddr.SetWindowTextA(value_str);
	}
	if( (length = sText.Find("check APOLLOVERSION")) >= 0){
		value_str = " ";
		value_str += sText.Mid( length+sizeof("check APOLLOVERSION") );
		m_fw_version.GetWindowTextA(sText);
		m_fw_version.SetWindowTextA(sText + "   apollo:" + value_str);
		if(check_sz_option_instance.g_glob_if_check_light)
			AddEvent2List("��ʼ���ýӽ�������������ڵ��ӽ���оƬ");
	}
	if(check_init_num == (check_sz_option_instance.g_if_check_bonemic +
			check_sz_option_instance.g_if_check_light +
			check_sz_option_instance.g_if_check_tap +
			check_sz_option_instance.g_if_check_plc +
			check_sz_option_instance.g_if_check_apollo) ){
		if(err_flag == 1){
			m_devCtrl.Stop();
		}	
	}
	//AddEvent2List(sText);
}

int software_check_sz::PlaysoundThreadProc(void *param)
{
	software_check_sz *p = (software_check_sz*)param;
	UINT fNo;
	while(p->PlaysoundThreadProc_exit) {
		::waveOutSetVolume(0, 0xFFFFFFFF);
		if(p->Playsound_value == 1){//1 СޱСޱ
			//fNo = IDR_WAVE14;
			//fNo = IDR_WAVE13;//С��С��
			fNo = IDR_WAVE15;//ʷС��
		}else{//2 1khz��Ƶ
			//fNo = IDR_WAVE12;
			fNo = IDR_WAVE11;
		}
		PlaySound((LPCTSTR)fNo, NULL, SND_NODEFAULT|SND_SYNC|SND_RESOURCE);
	}
	return 0;
}

int software_check_sz::pc_thread_record(void *param)
{
	software_check_sz *p = (software_check_sz*)param;
	p->pc_record();
	return 0;
}

extern int getAvgPcmDB(INT16  *pcmdata,  long nmemb);
int software_check_sz::get_file_dbvalue(int *db_value, CString file_path)
{
	BYTE *audio_decode_buf = new BYTE[1<<20];//����buf
	unsigned short outSize;
	
	CFile m_recFile;
	//CFile m_testFile;
	int g_size;
	unsigned char* g_buf;
	int audio_decode_len = 0;
	
	//file_path = m_devCtrl.GetRecordFile();
	if(m_recFile.Open(file_path, CFile::modeRead) == TRUE) {
		m_recFile.SeekToBegin();
		g_size = (int)m_recFile.GetLength();
		g_buf = (unsigned char*)malloc(g_size);
		m_recFile.Read(g_buf,g_size);

		audio_decode_len = 0;
		while(audio_decode_len != g_size*16){
				if( acodec_decoder((g_buf+(audio_decode_len/16)), 40,(audio_decode_buf+audio_decode_len), &outSize) != 0 ){
					AddEvent2List("¼���ļ�����ʧ�ܣ�");
					return -1;
				}
				audio_decode_len += outSize;
		}

		*db_value = getAvgPcmDB((INT16*)audio_decode_buf, audio_decode_len / 2);

		//if(m_testFile.Open("audio\\check_sz\\record\\1.pcm", CFile::modeCreate | CFile::typeBinary | CFile::modeReadWrite) == TRUE) {
		//	m_testFile.Write(audio_decode_buf, audio_decode_len);
		//	m_testFile.Close();
		//}

		m_recFile.Close();
		free(g_buf);
				
	}else{
		AddEvent2List("¼���ļ���ʧ�ܣ�");
		return -1;
	}

	delete audio_decode_buf;

	return 0;
}

DWORD software_check_sz::MicCallBack(HWAVEIN hWaveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    // ���е���Щ¼��������������¼�������Զ������ģ������ⲻ��Ҫ�Լ�����
	software_check_sz *pwnd = (software_check_sz*)dwInstance;     // ��ʾ¼���Ĵ���
    PWAVEHDR whd = (PWAVEHDR)dwParam1; // ¼����ͷ�ṹ�����
    switch(uMsg){
		case WIM_OPEN: // ��¼���豸�����ﲻ������
			break;
		case WIM_DATA: // ��ʾ���������������ǽ���Ϣд��һ��pcm�ļ�
			if (pwnd->isGetSound){
				fwrite(whd->lpData, 1, whd->dwBufferLength, pwnd->pf);
				waveInAddBuffer(hWaveIn, whd, sizeof(WAVEHDR));
            }
			break;
		case WIM_CLOSE: // ֹͣ¼��
			waveInStop(hWaveIn);
			waveInReset(hWaveIn);    
			waveInClose(hWaveIn);
			break;
		default:
			break;
   }
   return 0;
}
int software_check_sz::pc_record()
{
	pc_record_flag = 1;
	waveFormat.nSamplesPerSec = 16000;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.cbSize = 0;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample * waveFormat.nChannels)/8;
	waveFormat.nAvgBytesPerSec = waveFormat.nBlockAlign * waveFormat.nSamplesPerSec;

	MMRESULT mRet=waveInOpen(&hWaveIn, WAVE_MAPPER, (LPCWAVEFORMATEX)&waveFormat,(DWORD)MicCallBack, (DWORD)this, CALLBACK_FUNCTION);
	if (mRet != MMSYSERR_NOERROR){
		AfxMessageBox("��˷��ʧ�ܣ�����Ƿ���");
		return -1;
	}

	INT bufSize = waveFormat.nSamplesPerSec * waveFormat.nChannels * waveFormat.wBitsPerSample /8;

	BYTE *pBuffer1 = new BYTE[bufSize];
	if (pBuffer1 == NULL) return -1;
	memset(pBuffer1, 0, bufSize);
	WAVEHDR wHdr1;
	wHdr1.lpData = (LPSTR)pBuffer1;
	wHdr1.dwBufferLength = bufSize;
	wHdr1.dwBytesRecorded = 0;
	wHdr1.dwUser = 0;
	wHdr1.dwFlags = 0;
	wHdr1.dwLoops = 1;
 
	BYTE *pBuffer2 = new BYTE[bufSize];
	if (pBuffer2 == NULL) return -1;
	memset(pBuffer2,0, bufSize);
	WAVEHDR wHdr2;
	wHdr2.lpData = (LPSTR)pBuffer2;
	wHdr2.dwBufferLength = bufSize;
	wHdr2.dwBytesRecorded = 0;
	wHdr2.dwUser = 0;
	wHdr2.dwFlags = 0;
	wHdr2.dwLoops = 1;

	waveInPrepareHeader(hWaveIn, (LPWAVEHDR)&wHdr1, sizeof(WAVEHDR)); // ׼����һ���������ݿ�����¼��
	waveInPrepareHeader(hWaveIn, (LPWAVEHDR)&wHdr2, sizeof(WAVEHDR)); // ׼���ڶ������ݿ�����¼��

	waveInAddBuffer(hWaveIn, (LPWAVEHDR)&wHdr1, sizeof(WAVEHDR)); // ָ���������ݿ�Ϊ¼�����뻺��
	waveInAddBuffer(hWaveIn, (LPWAVEHDR)&wHdr2, sizeof(WAVEHDR)); // ָ���������ݿ�Ϊ¼������
	
	CString sText;
	CTime tNow = CTime::GetCurrentTime();
	CreateDirectory("audio", NULL);
	CreateDirectory("audio\\check_sz", NULL);
	CreateDirectory("audio\\check_sz\\play", NULL);
	sText.Format("audio\\check_sz\\play\\Aud_play_%04d%02d%02d_%02d%02d%02d.pcm",
					tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(),
					tNow.GetHour(),tNow.GetMinute(), tNow.GetSecond());
	rec_file_path = sText;
	fopen_s(&pf, sText, "ab+");// һ��Ҫ�Զ���������д�룬����¼������Ƶ���������

	isGetSound = 1;
	DWORD dwStart, dwStop, dwStart_sav; 
	dwStart = GetTickCount(); 
	dwStart_sav = dwStart;
	waveInStart(hWaveIn); // ��ʼ¼��
	float time_ms;
	while(1){
		dwStop = GetTickCount(); 
		if(dwStop - dwStart > 150){
			time_ms = (float)(dwStop - dwStart_sav);
			time_ms = time_ms/1000;
			sText.Format("%f",time_ms);
			sText = sText.Left(3);
			sText += "��";
			m_player_sec.SetWindowTextA(sText);
			dwStart = dwStop;
		}
		if(dwStop - dwStart_sav > check_sz_option_instance.g_play_time_sec*1000){
			time_ms = (float)(dwStop - dwStart_sav);
			time_ms = time_ms/1000;
			sText.Format("%f",time_ms);
			sText = sText.Left(3);
			sText += "��";
			m_player_sec.SetWindowTextA(sText);
			break;
		}
		Sleep(1);
	}
    isGetSound = 0;
	waveInReset(hWaveIn);
	waveInClose(hWaveIn); // ֹͣ¼��
	delete []pBuffer1;
	delete []pBuffer2;
	fclose(pf);

	int g_size;
	char* g_buf;
	if(rec_file.Open(rec_file_path, CFile::modeRead) == TRUE) {
		rec_file.SeekToBegin();
		g_size = (int)rec_file.GetLength();
		g_buf = (char*)malloc(g_size);
		rec_file.Read(g_buf,g_size);
		rec_file.Close();
		aud_db = getAvgPcmDB((INT16*)g_buf, g_size / 2);
		free(g_buf);
		sText.Format("%d",aud_db);
		m_play_db.SetWindowTextA(sText);
		if((int)aud_db < (int)check_sz_option_instance.g_play_passdb){
			AddEvent2List("�����ļ�dbֵ��С�����ɹ�վ��");
			err_flag = 1;
		}else{
			AddEvent2List("�����ļ�dbֵ������ϣ��ɹ�վ��");
		}
	}else{
		err_flag = 1;
		AddEvent2List("¼���ļ���ʧ�ܣ�");
	}
	pc_record_flag = 0;
	return 0;
}

int software_check_sz::check_reboot(void *param)
{
	CString sText;bool check_success;
	UINT32 m_devHandle_reboot[100], i;
	software_check_sz *p = (software_check_sz*)param;

	check_success = FALSE;

	sText.Format("%d�����usb�ɷ����ӣ�",p->check_sz_option_instance.g_wait_reboot_time_sec);
	p->AddEvent2List(sText);
	Sleep(p->check_sz_option_instance.g_wait_reboot_time_sec * 1000);

	UINT32 m_devHandle = openTestEngine(USBDBG, "1", 0, 5000, 1000);
	TRACE("handle = %d", m_devHandle);

	if(m_devHandle <= 0){
		p->m_wait_openusb.SetColor(STATIC_WND_WARNING);
		p->AddEvent2List("��usb�豸ʧ�ܣ�");
		check_success = FALSE;
	}else{
		p->m_wait_openusb.SetColor( STATIC_WND_SUCCESS);
		p->AddEvent2List("��usb�豸�ɹ���");

		sText.Format("%d���ڼ��usb�Ƿ�Ͽ�,�밴��������",p->check_sz_option_instance.g_wait_key_time_sec);
		p->AddEvent2List(sText);
		
		for(i=0; i<p->check_sz_option_instance.g_wait_key_time_sec * 10; i++){
			m_devHandle_reboot[i] = openTestEngine(USBDBG, "1", 0, 5000, 1000);
			TRACE("handle = %d", m_devHandle_reboot[i]);
			if(m_devHandle_reboot[i] <= 0){
				p->AddEvent2List("��⵽usb�Ͽ����������ɹ���");
				p->m_wait_closeusb.SetColor( STATIC_WND_SUCCESS);
				check_success = TRUE;
				break;
			}
			Sleep(100);
		}

		if(i == p->check_sz_option_instance.g_wait_key_time_sec * 10){
			p->AddEvent2List("��ⳬʱ������ʱ���ڰ�����������");
			p->m_wait_closeusb.SetColor( STATIC_WND_WARNING);
			check_success = FALSE;
		}

		for(i=0; i<p->check_sz_option_instance.g_wait_key_time_sec * 10; i++){
			closeTestEngine(m_devHandle_reboot[i]);
		}
		closeTestEngine(m_devHandle);
	}

	if(check_success){
		if( p->general_option_instance.g_check_mes && 
			(p->general_option_instance.g_check_mes_type == 3) ){//mes����
			if( (sText = p->mesInfo.sz_post_passfail(p->sn_no, 
						p->general_option_instance.g_station_name,
						1, 1) ) == "OK"){
				p->AddEvent2List("mes ͨ�ųɹ���");
				p->pass_green_color = 1;
				p->m_Epass.SetWindowTextA("PASS");
				p->Invalidate();
			}else{
				p->AddEvent2List("mes ͨ��ʧ�ܣ�������Ϣ��");
				p->AddEvent2List(sText);
				p->pass_green_color = 0;
				p->m_Epass.SetWindowTextA("FAIL");
				p->Invalidate();
			}
		}else{
			p->pass_green_color = 1;
			p->m_Epass.SetWindowTextA("PASS");
			p->Invalidate();
		}			
	}else{
		p->pass_green_color = 0;
		p->m_Epass.SetWindowTextA("FAIL");
		p->Invalidate();
	}

	return 0;
}

int software_check_sz::mes_sannuo_write_logfile(CString SN, CString MAC)
{
	CString sFname;
	CreateDirectory(SANNUO_MES_LOG_PATH, 0);
	sFname = SN + "_PASS.txt";
	sFname = SANNUO_MES_LOG_PATH + sFname;
	if(m_mes_file.Open(sFname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) == TRUE) {
		sFname.Format("********************************\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("Function   :  smt���\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("Device SN:     ");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname = SN + "\r\n";
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("Device MAC:     ");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname = MAC + "\r\n";
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("Test Result:     PASS\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("Cycle Time:      0 s\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("ErrorCode:\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("********************************\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		CTime tNow = CTime::GetCurrentTime();
		sFname.Format("%04d%02d%02d_%02d%02d%02d\r\n", 
			tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), 
			tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond());
		sFname = "Start Time :     " + sFname;
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("********************************\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		m_mes_file.Flush();
		m_mes_file.Close();
	}
	return 0;
}

LRESULT software_check_sz::OnDevCtrlReport(WPARAM wParam, LPARAM lParam)
{
	CString sText;
	int length;
	CString value_str;

	switch ((int)wParam) {
	case REPORT_THREAD_START:
		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
		PlaysoundThreadProc_exit = 1;
		Playsound_value = 1;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_sz::PlaysoundThreadProc, this, 0, NULL);
		m_btnBoot.SetColor(STATIC_WND_SUCCESS);
		break;
	case REPORT_CHIPID:
		break;
	case REPORT_ERASE_START:
		break;
	case REPORT_ERASE_PROGRESS:
		break;
	case REPORT_PROGRAM_START:
		break;
	case REPORT_PROGRAM_PERCENT:
		break;
	case REPORT_PROGRAM_END:
		break;
	case REPORT_OPEN_ENGINE:
		break;
	//case REPORT_RDBD_ADDR:
	//	break;
	//case REPORT_WRBD_ADDR:
	//	break;
	//case REPORT_RDBD_NAME:
	//	break;
	//case REPORT_WRBD_NAME:
	//	break;
	case REPORT_WRITE_FIXPARAM:
		break;
	case REPORT_CLOSE_ENGINE:
		break;
	case REPORT_WAIT_RESTART:
		break;
	case REPORT_COMMU_START:
		break;
	//case REPORT_COMMU_SUCC:
	//	break;
	//case REPORT_COMMU_READ:
	//	break;
	case REPORT_COMMU_OPEN:
		sText.Format("REPORT_COMMU_OPEN %s", (char*)lParam);
		AddEvent2List(sText);
		break;
	case RERROT_COMMU_TIMEOUT:
		break;
	//case REPORT_APOLLO:
	//	break;
	//case REPORT_READ_RECORD:
	//	break;
	case REPORT_USER_EXIT:
		break;
	case REPORT_END_ALL:
		while(pc_record_flag == 1){;}//��¼������
		if(err_flag != 1){
			if( (int)lParam==0 ){
				if(check_sz_option_instance.g_glob_if_check_reboot){
					CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_sz::check_reboot, this, 0, NULL);
				}else{
					if( general_option_instance.g_check_mes ){ 
						if( (general_option_instance.g_check_mes_type == 3) ){//mes����
							if( (sText = mesInfo.sz_post_passfail(sn_no, 
										general_option_instance.g_station_name,
										1, 1) ) == "OK"){
								AddEvent2List("mes ͨ�ųɹ���");
								pass_green_color = 1;
								m_Epass.SetWindowTextA("PASS");
								Invalidate();
							}else{
								AddEvent2List("mes ͨ��ʧ�ܣ�������Ϣ��");
								AddEvent2List(sText);
								pass_green_color = 0;
								m_Epass.SetWindowTextA("FAIL");
								Invalidate();
							}
						}else if( (general_option_instance.g_check_mes_type == 4) ){
							m_btaddr.GetWindowText(sText);
							mes_sannuo_write_logfile(sn_no,sText);
							AddEvent2List("mes log�ļ����ɳɹ���");
							pass_green_color = 1;
							m_Epass.SetWindowTextA("PASS");
							Invalidate();						
						}else if( (general_option_instance.g_check_mes_type == 5) ){
							mes_zhuoyi_dll mes_zhuoyi_dll_ins;
							if( mes_zhuoyi_dll_ins.Check_Station(sn_no)==0 ){
								AddEvent2List("mes Check_Station ͨ�ųɹ���");
							}else{
								AddEvent2List("mes Check_Station У��ʧ�ܣ�sn���ڴ˹�վ��");
								pass_green_color = 0;
								m_Epass.SetWindowTextA("FAIL");
								Invalidate();
							}
							if( mes_zhuoyi_dll_ins.Pass_Station(1, sn_no)==0 ){
								AddEvent2List("mes Pass_Station ͨ�ųɹ���");
								pass_green_color = 1;
								m_Epass.SetWindowTextA("PASS");
								Invalidate();
							}else{
								AddEvent2List("mes Pass_Station ͨ��ʧ�ܣ�");
								pass_green_color = 0;
								m_Epass.SetWindowTextA("FAIL");
								Invalidate();
							}

						}else{
							AddEvent2List("��֧�ֵ�mes���ͣ����������ã�");
							pass_green_color = 0;
							m_Epass.SetWindowTextA("FAIL");
							Invalidate();								
						}
					}else{
						pass_green_color = 1;
						m_Epass.SetWindowTextA("PASS");
						Invalidate();
					}
				}
			}else{
				pass_green_color = 0;
				m_Epass.SetWindowTextA("FAIL");
				Invalidate();
			}
		}else{
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();		
		}
		PlaysoundThreadProc_exit = 0;
		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
		break;
	case REPORT_LAST:
		break;

	case REPORT_APOLLO:
		break;
	case REPORT_RDBD_NAME:
	case REPORT_WRBD_NAME:
	case REPORT_RDBD_ADDR:
	case REPORT_WRBD_ADDR:
	case REPORT_COMMU_SUCC:
		sText.Format("%s", (char*)lParam);
		//AddEvent2List(sText);
		if(sText.Find("check DEVICEINFO") >= 0){
			AddEvent2List("��ʼ����ʼ��������");
		}
		if(sText.Find("checkresp SENSOR") >= 0){
			AddEvent2List("�ȴ��ڵ��ӽ���оƬ��");
		}
		if(sText.Find("checkresp WAKEUP") >= 0){
			Playsound_value = 1;
			AddEvent2List("�ȴ������޲��ԣ�");
		}
		if(sText.Find("checkresp TAP") >= 0){
			Playsound_value = 2;
			AddEvent2List("�ȴ��û����ԣ�");
		}
		if(sText.Find("checkresp PLC") >= 0){
			Playsound_value = 2;
			AddEvent2List("�ȴ�plc���ԣ�");
		}
		if(sText.Find("check STARTRECORD0") >= 0){
			Playsound_value = 2;
			right_file_path = m_devCtrl.GetRecordFile();
			AddEvent2List("��ʼ��������");
		}
		if(sText.Find("check STARTRECORD1") >= 0){
			Playsound_value = 2;
			left_file_path = m_devCtrl.GetRecordFile();
			AddEvent2List("��ʼ���Ը���");
		}
		if(sText.Find("check STOPRECORD0") >= 0){
			AddEvent2List("����¼��������");
			if( get_file_dbvalue(&dbvalue_right, right_file_path) == 0){
				sText.Format("����¼��������ϣ�db=%d��",dbvalue_right);

				AddEvent2List(sText);
				sText.Format("%d",dbvalue_right);
				m_rec_right_db.SetWindowTextA(sText);
				if(dbvalue_right < (int)check_sz_option_instance.g_record_passdb){
					AddEvent2List("����¼��dbֵ��С��");
					m_devCtrl.Stop();
					err_flag = 1;
				}
			}else{
				AddEvent2List("����¼��ʧ��");
				m_devCtrl.Stop();
				err_flag = 1;
			}
		}
		if(sText.Find("check STOPRECORD1") >= 0){
			AddEvent2List("����¼��������");
			PlaysoundThreadProc_exit = 0;
			if( get_file_dbvalue(&dbvalue_left, left_file_path) == 0){
				sText.Format("����¼��������ϣ�db=%d��",dbvalue_left);

				AddEvent2List(sText);
				sText.Format("%d",dbvalue_left);
				m_rec_left_db.SetWindowTextA(sText);
				if(dbvalue_left < (int)check_sz_option_instance.g_record_passdb){
					AddEvent2List("����¼��dbֵ��С��");
					m_devCtrl.Stop();
					err_flag = 1;
				}
			}else{
				AddEvent2List("����¼��ʧ��");
				m_devCtrl.Stop();
				err_flag = 1;
			}
			if( abs(dbvalue_left-dbvalue_right) > (int)check_sz_option_instance.g_record_subdb ){
				AddEvent2List("������¼���ļ���ֵ����");
				m_devCtrl.Stop();
				err_flag = 1;
			}
		}
		break;
	case REPORT_COMMU_READ:
		set_init_check_status(lParam);
		break;
	case REPORT_READ_SENSOR:
		sText.Format("%s", (char*)lParam);
		if( (length = sText.Find("checkresp RDSENSOR=")) >= 0){
			value_str = " ";
			value_str += sText.Mid( length+sizeof("checkresp RDSENSOR") );
			m_get_ps.SetWindowTextA(value_str);
			m_get_ps.SetColor(STATIC_WND_SUCCESS);
			sText = "�ӽ����ȡpsֵΪ��" + value_str;
			AddEvent2List(sText);
		}
		break;
	case REPORT_WRITE_SENSOR:
		m_set_ps.SetColor(STATIC_WND_SUCCESS);
		sText.Format("����psֵ+%d�ɹ�", check_sz_option_instance.g_check_light_addps_value);
		AddEvent2List(sText);
		break;
	case REPORT_WAKEUP:
		sText.Format("%s", (char*)lParam);
		if(sText.Find("SUSS") >= 0){
			AddEvent2List("�����޼��ɹ���");
			m_wait_apollo.SetColor(STATIC_WND_SUCCESS);
		}
		break;
	case REPORT_SENSOR:
		sText.Format("%s", (char*)lParam);
		if(sText.Find("SUSS") >= 0){
			AddEvent2List("�ӽ�����ɹ���");
			m_wait_light.SetColor(STATIC_WND_SUCCESS);
		}
		break;
	case REPORT_PLC:
		sText.Format("%s", (char*)lParam);
		if(sText.Find("SUSS") >= 0){
			AddEvent2List("plc���ɹ���");
			m_wait_plc.SetColor(STATIC_WND_SUCCESS);
		}
		break;
	case REPORT_TAP:
		sText.Format("%s", (char*)lParam);
		if(sText.Find("SUSS") >= 0){
			AddEvent2List("�û����ɹ���");
			m_wait_tap.SetColor(STATIC_WND_SUCCESS);
		}
		break;
	case REPORT_PLAY_TONE:
		sText.Format("%s", (char*)lParam);
		if(sText.Find("checkresp SPEAKER") >= 0){
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_sz::pc_thread_record, this, 0, NULL);
			AddEvent2List("������ʼ��");
		}
		if(sText.Find("check SPEAKER END") >= 0){
			AddEvent2List("����������");
		}
		//AddEvent2List(sText);
		break;
	case REPORT_READ_RECORD:
		float sec_time = (float)lParam;
		sec_time = sec_time/2000;
		sText.Format("%f", sec_time);
		sText = sText.Left(3);
		sText += "��";
		m_recplay_sec.SetWindowTextA(sText);
		break;
	}


	return 0;
}

void software_check_sz::StartDevContrl()
{
	CString sText;UINT type;
	UpdateData(TRUE);

	m_devCtrl.SetTimeout(check_sz_option_instance.g_check_timeout_s);
	m_devCtrl.SetRecordTime(check_sz_option_instance.g_record_time_sec);
	m_devCtrl.SetPlayTime(check_sz_option_instance.g_play_time_sec);

	type = 0;
	if(check_sz_option_instance.g_glob_if_check_init){ type |= THREAD_CHECK; }
	if(check_sz_option_instance.g_glob_if_check_apollo){ type |= THREAD_WAKEUP; }
	if(check_sz_option_instance.g_glob_if_check_light){ type |= THREAD_SENSOR; }
	if(check_sz_option_instance.g_glob_if_check_plc){ type |= THREAD_PLC; }
	if(check_sz_option_instance.g_glob_if_check_tap){ type |= THREAD_TAP; }
	if(check_sz_option_instance.g_glob_if_check_record){
		type |= THREAD_RECORD_0;
		type |= THREAD_RECORD_1;
	}
	if(check_sz_option_instance.g_glob_if_check_play){ type |= THREAD_PLAY; }
	m_devCtrl.SetThreadFunc(type);
	m_devCtrl.Start(this->m_hWnd);
}

extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_check_sz::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString sText;
	CString stmp;
	m_EditSn.GetWindowTextA(sText);
	if(sText.GetLength() == 14){//v3
		sn_no = sText;
		StartDevContrl();
		stmp = "sn������ɹ����ȴ��ϵ�" + sText;
		AddEvent2List(stmp);
	}
	if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		if( sText.Find("-") < 0 ){
			if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("��ǰ��ƷSN��(v2)����ȷ");
			}else{
				sn_no = sText;
				StartDevContrl();
				AddEvent2List("sn������ɹ����ȴ��ϵ�");
			}
		}
	}
	if(sText.GetLength() >= 19){
		if(CheckSN(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
			AfxMessageBox("��ǰ��ƷSN��(v1)����ȷ");
		}else{
			sn_no = sText;
			StartDevContrl();
			AddEvent2List("sn������ɹ����ȴ��ϵ�");
		}
	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


BOOL software_check_sz::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);//������ͼ��
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString dir_path;
	dir_path = LOG_PATH_CHECK_sz;

	CString soft_name;
	soft_name += last_sel_soft;
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	CreateDirectory("log", 0);
	CreateDirectory(dir_path, 0);
	CString sFname;
	CTime tNow = CTime::GetCurrentTime();
	sFname.Format("LOG-CH%d-%04d%02d%02d-%dh.txt", 0,
		tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), tNow.GetHour());
	sFname = dir_path + sFname;
	if(m_hLogFile.Open(sFname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) == TRUE) {
		m_hLogFile.SeekToEnd();
		sFname.Format("===============================================\r\n");
		m_hLogFile.Write(sFname, sFname.GetLength());
	}

	m_editFont.CreatePointFont(700, "����");
	m_Epass.SetFont(&m_editFont);

	mesInfo.g_mes_ip = general_option_instance.g_mes_ip;
	mesInfo.g_mes_port = general_option_instance.g_mes_port;

	if(check_sz_option_instance.g_glob_if_check_init){
		AddEvent2List("�����˲��Գ�ʼ��������");
	}else{
		AddEvent2List("û�����ò��Գ�ʼ��������");
	}
	if(check_sz_option_instance.g_glob_if_check_apollo){
		AddEvent2List("�����˲��԰����ޣ�");
	}else{
		AddEvent2List("û�����ò��԰����ޣ�");
	}
	if(check_sz_option_instance.g_glob_if_check_light){
		AddEvent2List("�����˲��Խӽ��⣡");
	}else{
		AddEvent2List("û�����ò��Խӽ��⣡");
	}
	if(check_sz_option_instance.g_glob_if_check_plc){
		AddEvent2List("�����˲���plc��");
	}else{
		AddEvent2List("û�����ò���plc��");
	}
	if(check_sz_option_instance.g_glob_if_check_tap){
		AddEvent2List("�����˲����û���");
	}else{
		AddEvent2List("û�����ò����û���");
	}
	if(check_sz_option_instance.g_glob_if_check_record){
		AddEvent2List("�����˲���¼����");
	}else{
		AddEvent2List("û�����ò���¼����");
	}
	if(check_sz_option_instance.g_glob_if_check_play){
		AddEvent2List("�����˲��Է�����");
	}else{
		AddEvent2List("û�����ò��Է�����");
	}
	if(check_sz_option_instance.g_wait_key_time_sec > 10){
		AddEvent2List("���󣺰���ʱ�䲻�ɳ���10s��");
	}
	pc_record_flag = 0;
	check_init_num = 0;
	AddEvent2List("#####�ȴ��û�����sn��#####");

	return TRUE;  // return TRUE unless you set the focus to a control
}


BOOL software_check_sz::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH software_check_sz::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID()==IDC_STATIC9)//����Ǿ�̬�༭��
    {
		if(pass_green_color){
			pDC->SetTextColor(RGB(0,255,0));//�޸��������ɫ
			pDC->SetBkMode(TRANSPARENT);//������ı������͸����
		}else{
		    pDC->SetTextColor(RGB(255,0,0));//�޸��������ɫ
			pDC->SetBkMode(TRANSPARENT);//������ı������͸����
		}
    }

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
