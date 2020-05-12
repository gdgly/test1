#pragma once

#include "WaveSound.h"

#define AUDIO_BUFFER_NUM                (16)       // 为每个音频句柄提供最多缓冲区数
#define SAMPLES_PER_SECOND              (16000)
#define BITS_PER_SAMPLE                 (16)
#define BYTES_PER_SAMPLE                (BITS_PER_SAMPLE/8)
#define AUDIO_RAW_PACKET_SIZE           (640)       // 编码前
#define AUDIO_ENCODE_PACKET_SIZE        (AUDIO_RAW_PACKET_SIZE/16)        // 编码后


#define SEND_USER_MSG(wnd,m,w,l)    ::SendMessage((wnd), (m), (w), (l));


#define WM_USERMSG_MASSPRODUCT         (WM_USER+33)
#define    WPARAM_GETFILE                 (0x10)
#define    WPARAM_THREAD_EXITED           (0x11)

class G722Play
{
public:
	G722Play(void);
	~G722Play(void);

public:
	HWND m_hWnd;
	int              m_bExitPlay;
	int PlayFile(CString sPath);
	void SetWnd(HWND hWnd) { m_hWnd = hWnd; }

public:	
	int AudioPlaying(void *dat);

private:
	CString          m_sPathName;
	HANDLE           m_playThread;    // 播放线程,需要播放音频及视频

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


typedef struct tagPLAYSTATUSINFO{
	char pause;                    //3个状态   0 没有按过暂停键  1 按过暂停键，处于暂停状态  2 按过暂停键，处于继续播放状态
	char con_play;
	char stop;
	char play_end;
	int offset;
}PlayStatus, *PlayStatusPtr;

extern PlayStatus gPlayInfo;


typedef struct tagPLAYTIMEINFO{
	int toal_time;
	int current_time;
	int old_current_time;
	int file_size;
	float start_offset;
	float end_offset;
	CString path;
}PlayTime, *PlayTimePtr;

extern PlayTime gPlayTimeInfo;



