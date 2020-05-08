#pragma once

class Mp3
{
public:
	Mp3(void);
	~Mp3(void);
	HWND hWnd;
	DWORD Device;
    MCI_OPEN_PARMS mciopenparms;
    void Open(HWND hwnd,CString Strfilepath);
	DWORD info(DWORD item);
	void Play();
	void Pause();
	void resume();
	void Stop();
	void SetVolume(DWORD vol);
	void SeekTo(DWORD to);
};

