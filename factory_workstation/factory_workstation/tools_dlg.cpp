// tools_dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "tools_dlg.h"
#include "afxdialogex.h"
#include "factory_workstationDlg.h"

// tools_dlg �Ի���

IMPLEMENT_DYNAMIC(tools_dlg, CPropertyPage)

tools_dlg::tools_dlg()
	: CPropertyPage(tools_dlg::IDD)
{

}

tools_dlg::~tools_dlg()
{
}

void tools_dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(tools_dlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON3, &tools_dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &tools_dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &tools_dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &tools_dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &tools_dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON19, &tools_dlg::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON24, &tools_dlg::OnBnClickedButton24)
	ON_BN_CLICKED(IDC_BUTTON25, &tools_dlg::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_BUTTON26, &tools_dlg::OnBnClickedButton26)
	ON_BN_CLICKED(IDC_BUTTON27, &tools_dlg::OnBnClickedButton27)
	ON_BN_CLICKED(IDC_BTN_SENSOR_CALC, &tools_dlg::OnBnClickedBtnSensorCalc)
	ON_BN_CLICKED(IDC_BTN_TAP_CALC, &tools_dlg::OnBnClickedBtnTapCalc)
END_MESSAGE_MAP()

void tools_dlg::set_param(void *p)
{
	glob_param_p = p;
}
// tools_dlg ��Ϣ�������


void tools_dlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	software_tool_version  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}



void tools_dlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	software_tool_barcode  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	software_tool_box_audio  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	software_tool_current_board  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	software_tool_gener_barcode  Dlg; 
	Dlg.DoModal();
}


DWORD tools_dlg::MicCallBack(HWAVEIN hWaveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    // ���е���Щ¼��������������¼�������Զ������ģ������ⲻ��Ҫ�Լ�����
	tools_dlg *pwnd = (tools_dlg*)dwInstance;     // ��ʾ¼���Ĵ���
    PWAVEHDR whd = (PWAVEHDR)dwParam1; // ¼����ͷ�ṹ�����
    switch(uMsg){
		case WIM_OPEN: // ��¼���豸�����ﲻ������
			break;
		case WIM_DATA: // ��ʾ���������������ǽ���Ϣд��һ��pcm�ļ�
			if (pwnd->isGetSound){
				FILE *pf;
				fopen_s(&pf, "lalalalala.pcm", "ab+");// һ��Ҫ�Զ���������д�룬����¼������Ƶ���������
				fwrite(whd->lpData, 1, whd->dwBufferLength, pf);
				waveInAddBuffer(hWaveIn, whd, sizeof(WAVEHDR));
				fclose(pf);
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


void tools_dlg::OnBnClickedButton19()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//HWAVEIN hWaveIn;
	//WAVEFORMATEX waveFormat;
	//waveFormat.nSamplesPerSec = 44100;
	//waveFormat.wBitsPerSample = 16;
	//waveFormat.nChannels = 2;

	//waveFormat.nSamplesPerSec = 16000;
	//waveFormat.wBitsPerSample = 16;
	//waveFormat.nChannels = 1;
	//waveFormat.cbSize = 0;
	//waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	//waveFormat.nBlockAlign = (waveFormat.wBitsPerSample * waveFormat.nChannels)/8;
	//waveFormat.nAvgBytesPerSec = waveFormat.nBlockAlign * waveFormat.nSamplesPerSec;

	//MMRESULT mRet=waveInOpen(&hWaveIn, WAVE_MAPPER, (LPCWAVEFORMATEX)&waveFormat,(DWORD)MicCallBack, (DWORD)this, CALLBACK_FUNCTION);
	//if (mRet != MMSYSERR_NOERROR)
	//{
	//	return;
	//}

	//INT bufSize = waveFormat.nSamplesPerSec * waveFormat.nChannels * waveFormat.wBitsPerSample /8;

	//BYTE *pBuffer1 = new BYTE[bufSize];
	//if (pBuffer1 == NULL) return;
	//memset(pBuffer1, 0, bufSize);
	//WAVEHDR wHdr1;
	//wHdr1.lpData = (LPSTR)pBuffer1;
	//wHdr1.dwBufferLength = bufSize;
	//wHdr1.dwBytesRecorded = 0;
	//wHdr1.dwUser = 0;
	//wHdr1.dwFlags = 0;
	//wHdr1.dwLoops = 1;
 //
	//BYTE *pBuffer2 = new BYTE[bufSize];
	//if (pBuffer2 == NULL) return;
	//memset(pBuffer2,0, bufSize);
	//WAVEHDR wHdr2;
	//wHdr2.lpData = (LPSTR)pBuffer2;
	//wHdr2.dwBufferLength = bufSize;
	//wHdr2.dwBytesRecorded = 0;
	//wHdr2.dwUser = 0;
	//wHdr2.dwFlags = 0;
	//wHdr2.dwLoops = 1;

	//waveInPrepareHeader(hWaveIn, (LPWAVEHDR)&wHdr1, sizeof(WAVEHDR)); // ׼����һ���������ݿ�����¼��
	//waveInPrepareHeader(hWaveIn, (LPWAVEHDR)&wHdr2, sizeof(WAVEHDR)); // ׼���ڶ������ݿ�����¼��

	//waveInAddBuffer(hWaveIn, (LPWAVEHDR)&wHdr1, sizeof(WAVEHDR)); // ָ���������ݿ�Ϊ¼�����뻺��
	//waveInAddBuffer(hWaveIn, (LPWAVEHDR)&wHdr2, sizeof(WAVEHDR)); // ָ���������ݿ�Ϊ¼������

	//isGetSound = 1;
	//waveInStart(hWaveIn); // ��ʼ¼��

	//Sleep(3000);
 //  
 //   isGetSound = 0;
	//waveInReset(hWaveIn);
	//waveInClose(hWaveIn); // ֹͣ¼��
	//delete []pBuffer1;
	//delete []pBuffer2;   
	software_tool_vnc  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton24()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	software_tool_by_getfile  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton25()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	software_tool_sz_light  Dlg; 
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton26()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	software_tool_sz_freq  Dlg; 
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton27()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	software_tool_sz_btaddr  Dlg; 
	Dlg.DoModal();
}

#include "software_tool_psensor_calc.h"
void tools_dlg::OnBnClickedBtnSensorCalc()
{
	software_tool_psensor_calc  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}

#include "software_tool_tap_calc.h"
void tools_dlg::OnBnClickedBtnTapCalc()
{
	software_tool_tap_calc  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}
