#include "StdAfx.h"
#include "G722Play.h"
#include "acodec.h"

int G722Play::AVPlayProc(void *param)
{
	((G722Play*)param)->AudioPlaying(param);

//	((G722Play*)param)->m_VideoPlay = INVALID_HANDLE_VALUE;

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
	CFile hFile;
	
	hFile.Open(m_sPathName, CFile::modeRead | CFile::typeBinary,  NULL, NULL);

	acodec_reset();
	SoundOutOpen();
	while(FALSE == m_bExitPlay) {
		outSize = 0;

		switch(m_audType) {
		case AUD_T_G722:
			rdLen = hFile.Read(m_g722ReadBuf, AUDIO_ENCODE_PACKET_SIZE);
			if(rdLen < AUDIO_ENCODE_PACKET_SIZE)
				break;

			ret = acodec_decoder(m_g722ReadBuf, AUDIO_ENCODE_PACKET_SIZE, m_g722DecBuf, &outSize);
			break;

		case AUD_T_SRC_MON:
		case AUD_T_SRC_STERO:
			rdLen = hFile.Read(m_g722DecBuf, AUDIO_RAW_PACKET_SIZE);
			if(rdLen < AUDIO_RAW_PACKET_SIZE)
				break;
			outSize = rdLen;
			break;
		}

		while(outSize > 0) {
			if(m_SoundOut.InputData((BYTE*)m_g722DecBuf, 0) == TRUE)
				break;
			Sleep(40);
		}

		TRACE("OFFSET=%d outsize=%d\n", (int)hFile.GetPosition(), outSize);
		if(0 == outSize) break;    // ²¥·Å½áÊø
	}
	Sleep(1000);

	hFile.Close();
	SoundOutClose();
	m_playThread      = INVALID_HANDLE_VALUE;
	TRACE("Play END====================================================\n");

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

int G722Play::PlayFile(CString sPath, int audType)
{
	DWORD dwID;

	if(INVALID_HANDLE_VALUE != m_playThread) {
		AfxMessageBox("Please Ended First");
		return 0;
	}
	
	m_audType = audType;
	m_sPathName = sPath;
	m_bExitPlay = FALSE;

	if(m_audType > AUD_T_SRC_STERO) {
		AfxMessageBox("PlayFile Unknow AudType");
		m_audType = AUD_T_G722;
	}

	if(INVALID_HANDLE_VALUE == m_playThread) {
		m_playThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)G722Play::AVPlayProc,
			this, 0, &dwID);
	}

	return 0;
}