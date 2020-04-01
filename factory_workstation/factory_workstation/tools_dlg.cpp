// tools_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "tools_dlg.h"
#include "afxdialogex.h"
#include "factory_workstationDlg.h"

// tools_dlg 对话框

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
// tools_dlg 消息处理程序


void tools_dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	software_tool_version  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}



void tools_dlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	software_tool_barcode  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	software_tool_box_audio  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	software_tool_current_board  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	software_tool_gener_barcode  Dlg; 
	Dlg.DoModal();
}


DWORD tools_dlg::MicCallBack(HWAVEIN hWaveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    // 所有的这些录音缓冲区都是由录音函数自动触发的，开发这不需要自己触发
	tools_dlg *pwnd = (tools_dlg*)dwInstance;     // 表示录音的窗体
    PWAVEHDR whd = (PWAVEHDR)dwParam1; // 录音的头结构体对象
    switch(uMsg){
		case WIM_OPEN: // 打开录音设备，这里不做处理
			break;
		case WIM_DATA: // 表示缓冲区已满，我们将信息写入一个pcm文件
			if (pwnd->isGetSound){
				FILE *pf;
				fopen_s(&pf, "lalalalala.pcm", "ab+");// 一定要以二进制数据写入，否则录音的音频会出现杂音
				fwrite(whd->lpData, 1, whd->dwBufferLength, pf);
				waveInAddBuffer(hWaveIn, whd, sizeof(WAVEHDR));
				fclose(pf);
            }
			break;
		case WIM_CLOSE: // 停止录音
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
	// TODO: 在此添加控件通知处理程序代码
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

	//waveInPrepareHeader(hWaveIn, (LPWAVEHDR)&wHdr1, sizeof(WAVEHDR)); // 准备第一个波形数据块用于录音
	//waveInPrepareHeader(hWaveIn, (LPWAVEHDR)&wHdr2, sizeof(WAVEHDR)); // 准备第二个数据块用于录音

	//waveInAddBuffer(hWaveIn, (LPWAVEHDR)&wHdr1, sizeof(WAVEHDR)); // 指定波形数据块为录音输入缓存
	//waveInAddBuffer(hWaveIn, (LPWAVEHDR)&wHdr2, sizeof(WAVEHDR)); // 指定波形数据块为录音缓存

	//isGetSound = 1;
	//waveInStart(hWaveIn); // 开始录音

	//Sleep(3000);
 //  
 //   isGetSound = 0;
	//waveInReset(hWaveIn);
	//waveInClose(hWaveIn); // 停止录音
	//delete []pBuffer1;
	//delete []pBuffer2;   
	software_tool_vnc  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton24()
{
	// TODO: 在此添加控件通知处理程序代码
	software_tool_by_getfile  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton25()
{
	// TODO: 在此添加控件通知处理程序代码
	software_tool_sz_light  Dlg; 
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton26()
{
	// TODO: 在此添加控件通知处理程序代码
	software_tool_sz_freq  Dlg; 
	Dlg.DoModal();
}


void tools_dlg::OnBnClickedButton27()
{
	// TODO: 在此添加控件通知处理程序代码
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
