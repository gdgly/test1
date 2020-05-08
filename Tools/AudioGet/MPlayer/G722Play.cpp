#include "StdAfx.h"
#include "G722Play.h"
#include "acodec.h"


#include "MPlayer.h"
#include "MPlayerDlg.h"
#include "afxdialogex.h"


extern int pause;
int offset = 0;
PlayStatus gPlayInfo = {0};       //初始化

int G722Play::AVPlayProc(void *param)                //相当于于一个main
{
	//((G722Play*)param)->AudioPlaying(param);         //
	PlayStatusPtr playptr = &gPlayInfo;
	G722Play *g722play = (G722Play*)param;
	SEND_USER_MSG(g722play->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_GETFILE, 0);
	g722play->AudioPlaying(g722play);
	if(playptr->stop == 0)
		SEND_USER_MSG(g722play->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);
	TRACE("%s Exit success\n", __FUNCTION__);
	return 0;
}

G722Play::G722Play(void)
{
	m_playThread      = INVALID_HANDLE_VALUE;
	
}


G722Play::~G722Play(void)
{
}

int G722Play::AudioPlaying(void *dat)
{
	int rdLen, ret;
	unsigned short outSize;
	PlayStatusPtr playptr = &gPlayInfo;
	PlayTimePtr timeptr = &gPlayTimeInfo;
	CFile hFile;
	
	hFile.Open(m_sPathName, CFile::modeRead | CFile::typeBinary,  NULL, NULL);

	acodec_reset();
	SoundOutOpen();
	while(playptr->stop == 0) {
		if(playptr->stop == 1) 
			break;
		if(playptr->pause == 1) {
			Sleep(8);
			continue;
		}
			if(playptr->con_play == 1)                                                   //继续播放
			{
				hFile.Seek(playptr->offset,CFile::begin);
				rdLen = hFile.Read(m_g722ReadBuf, AUDIO_ENCODE_PACKET_SIZE);  
				playptr->offset = playptr->offset + AUDIO_ENCODE_PACKET_SIZE;
			}
			else{
				rdLen = hFile.Read(m_g722ReadBuf, AUDIO_ENCODE_PACKET_SIZE);             //正常从头开始读取
				TRACE("rdLen = %d\n",rdLen);
				playptr->offset = playptr->offset + AUDIO_ENCODE_PACKET_SIZE;
			}
			
			if(rdLen < AUDIO_ENCODE_PACKET_SIZE){                                         //判断是否播放结束
				//TRACE("ret = %d\n",ret);
				playptr->pause = 0;
				break;
			}
			
			outSize = 0;
			ret = acodec_decoder(m_g722ReadBuf, AUDIO_ENCODE_PACKET_SIZE, m_g722DecBuf, &outSize);      //解码

			while(true) {
				if(m_SoundOut.InputData((BYTE*)m_g722DecBuf, 0) == TRUE)                             //写入声卡
					break;
				Sleep(40);
			}
	}
	Sleep(200);
	//timeptr->current_time = 0;
	//playptr->stop = 0;
    //playptr->offset = 0;
	//TRACE("OFFSET=%d outsize=%d\n", (int)hFile.GetPosition(), outSize);
	hFile.Close();
	SoundOutClose();
	m_playThread   = INVALID_HANDLE_VALUE;
	TRACE("Play END====================================================\n");
	//playptr->play_end = 1;
	return 0;
}

int G722Play::SoundOutOpen()
{
	WAVEFORMATEX struWaveFormat;

	struWaveFormat.cbSize =			sizeof(struWaveFormat);
	struWaveFormat.nBlockAlign =	1 * BITS_PER_SAMPLE / 8;
	struWaveFormat.nChannels =		1;
	struWaveFormat.nSamplesPerSec =	SAMPLES_PER_SECOND;
	struWaveFormat.wBitsPerSample =	BITS_PER_SAMPLE;
	struWaveFormat.nAvgBytesPerSec =SAMPLES_PER_SECOND * struWaveFormat.nBlockAlign;
	struWaveFormat.wFormatTag =		WAVE_FORMAT_PCM;

	if (m_SoundOut.OpenSound(struWaveFormat, AUDIO_BUFFER_NUM, AUDIO_RAW_PACKET_SIZE, CALLBACK_FUNCTION, 0)) {
		if (m_SoundOut.PlaySound())
		{				
			m_SoundOut.SetVolume(0x7fff7fff);
			m_bAudioOutOpened = TRUE;
		}
	}
	else{
		TRACE("SoundOut Error\n");
		return -1;
	}	
	
	return 0;
}

int G722Play::SoundOutClose()
{
	if(m_bAudioOutOpened) {
		m_bAudioOutOpened = FALSE;
		m_SoundOut.CloseSound();
	}

	return 0;
}

int G722Play::PlayFile(CString sPath)
{
	DWORD dwID = 0;
	//INVALID_HANDLE_VALUE = m_playThread;
	//m_playThread = INVALID_HANDLE_VALUE;
	if(INVALID_HANDLE_VALUE != m_playThread) {
		//AfxMessageBox("Please Ended First");
		return 0;
	}
	
	m_sPathName = sPath;
	m_bExitPlay = FALSE;
	if(INVALID_HANDLE_VALUE == m_playThread) {
		m_playThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)G722Play::AVPlayProc,this, 0, &dwID);
	}
	return 1;
}