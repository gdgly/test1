#pragma once

#include "WaveSound.h"

#define AUDIO_BUFFER_NUM                (16)       // Ϊÿ����Ƶ����ṩ��໺������
#define SAMPLES_PER_SECOND              (16000)
#define BITS_PER_SAMPLE                 (16)
#define BYTES_PER_SAMPLE                (BITS_PER_SAMPLE/8)
#define AUDIO_RAW_PACKET_SIZE           (640)       // ����ǰ
#define AUDIO_ENCODE_PACKET_SIZE        (AUDIO_RAW_PACKET_SIZE/16)        // �����

enum {AUD_T_G722=0, AUD_T_SRC_MON, AUD_T_SRC_STERO};      // ��Ҫ���ŵ���Ƶ����

class G722Play
{
public:
	G722Play(void);
	~G722Play(void);

public:
	int              m_bExitPlay;
	int PlayFile(CString sPath, int audType = AUD_T_G722);
	
public:	
	int AudioPlaying(void *dat);

private:
	CString          m_sPathName;
	HANDLE           m_playThread;    // �����߳�,��Ҫ������Ƶ����Ƶ
	UINT             m_audType;

private:
// ��Ƶ���
	BYTE            m_g722ReadBuf[AUDIO_BUFFER_NUM*AUDIO_ENCODE_PACKET_SIZE];
	BYTE            m_g722DecBuf[AUDIO_RAW_PACKET_SIZE];
	CWaveSound      m_SoundOut;
	BOOL            m_bAudioOutOpened;
	int SoundOutOpen();
	int SoundOutClose();

private:
	static int AVPlayProc(void *param);

};

