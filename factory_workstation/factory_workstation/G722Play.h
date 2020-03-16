#pragma once

#include "WaveSound.h"

#define AUDIO_BUFFER_NUM                (16)       // 为每个音频句柄提供最多缓冲区数
#define SAMPLES_PER_SECOND              (16000)
#define BITS_PER_SAMPLE                 (16)
#define BYTES_PER_SAMPLE                (BITS_PER_SAMPLE/8)
#define AUDIO_RAW_PACKET_SIZE           (640)       // 编码前
#define AUDIO_ENCODE_PACKET_SIZE        (AUDIO_RAW_PACKET_SIZE/16)        // 编码后

enum {AUD_T_G722=0, AUD_T_SRC_MON, AUD_T_SRC_STERO};      // 需要播放的音频类型

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
	HANDLE           m_playThread;    // 播放线程,需要播放音频及视频
	UINT             m_audType;

private:
// 音频输出
	BYTE            m_g722ReadBuf[AUDIO_BUFFER_NUM*AUDIO_ENCODE_PACKET_SIZE];
	BYTE            m_g722DecBuf[AUDIO_RAW_PACKET_SIZE];
	CWaveSound      m_SoundOut;
	BOOL            m_bAudioOutOpened;
	int SoundOutOpen();
	int SoundOutClose();

private:
	static int AVPlayProc(void *param);

};

