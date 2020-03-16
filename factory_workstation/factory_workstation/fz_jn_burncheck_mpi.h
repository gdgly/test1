#pragma once
#include "afxcmn.h"

#define G_CLPROG ("cli_programmer.exe");

#define COM_MAX_RDSZIZE            (4096*2*10)
#define COM_BUF_SIZE            (2048)   // 串口缓冲区大小
enum {COM_No_BOARD=0, COM_No_CHECK, COM_No_COUNT};

// 每一位表示一 种成功的检测状态
enum {BIT_SYS_RUN96M=0,              // Systen Run96M OK
	BIT_CS47L35,                     // CS47L35 OK
	BIT_NAU88L24,
	BIT_SPEAKER,                     // TEA9896 OK
	BIT_KEY_TOUCH_INIT,              // TouchInit OK

	BIT_KEY_TOUCH_UP,                // TouchUp OK
	BIT_KEY_TOUCH_DOWN,              // TouchDown OK
	BIT_KEY_BOARD,                   // KeyUp OK   | KeyDown OK

	BIT_AUDIO_IN_OUT,                // User Select

	BIT_SN_OK,                       // 获取到蓝牙地址及SN
	BIT_ADDR_OK,
	BIT_FIRMVER_OK,                  // 固件版本


	BIT_CURRENT_mA,                  // CURRENT=xxxmA OK
	BIT_LANGUAGES_STR,			//languages 

	BIT_END,
};

#define BIT_START_INIT_FZ         ((1<<BIT_SYS_RUN96M) | (1<<BIT_CS47L35) | (1<<BIT_SPEAKER)\
	| (1<<BIT_KEY_TOUCH_INIT) | (1<<BIT_SN_OK) | (1<<BIT_ADDR_OK) | (1<<BIT_FIRMVER_OK))
#define BIT_START_INIT_JN         ((1<<BIT_SYS_RUN96M) | (1<<BIT_NAU88L24)\
	| (1<<BIT_KEY_TOUCH_INIT) | (1<<BIT_SN_OK) | (1<<BIT_ADDR_OK) | (1<<BIT_FIRMVER_OK))
#define BIT_START_DISPLY ((1<<BIT_SN_OK) | (1<<BIT_ADDR_OK) | (1<<BIT_FIRMVER_OK))
#define BIT_KEY_CHECKED_FZ        (BIT_START_INIT_FZ | (1<<BIT_KEY_TOUCH_UP) | (1<<BIT_KEY_TOUCH_DOWN) | (1<<BIT_KEY_BOARD))
#define BIT_KEY_CHECKED_JN        (BIT_START_INIT_JN | (1<<BIT_KEY_TOUCH_UP) | (1<<BIT_KEY_TOUCH_DOWN) | (1<<BIT_KEY_BOARD))

enum {KEY_COM ,CHECK_COM ,Play_COM ,CHARG_OPEN_COM, CHARG_SHUT_COM,STOP_PLAY_COM, 
	CURRENT_UART_CLOSE, CURRENT_UART_OPEN, SLEEP, USB_ON, 
	NOPATH_TEST_R, PATH_TEST_R, END_COM};//发送串口命令指令

enum {MSG_T_STRING=0, MSG_T_BTN_START, MSG_T_NO_CURRET,MSG_T_KEY_ERR,
	MSG_T_PROGRESS,
	MSG_T_BITCHECK,
	MSG_T_PLAYAUD,
	MSG_T_WRITEFIRM,
	MSG_T_ADDRSN,        // 显示ADDR & SN
	MSG_T_RESTART,
	MSG_T_AUDSELOK,MSG_T_AUDSELERR,
	MSG_T_CURRENT_ERR,
	MSG_T_JLINK_ERR,
	MSG_T_CHARGE_OPEN,
	MSG_T_CHARGE_CLOSE,
	MSG_T_CHARG_SUSS,
	MSG_T_SLEEP_CMD,
	MSG_T_SLEEP_CUR_SUSS,
	MSG_T_SLEEP_CUR_ERR,
	MSG_T_ALL_SUSS,
	MSG_T_SMES_SEND,
	MSG_T_EMES_SEND,
	MSG_T_EMES_ERR,
	MSG_T_MES_STRING,
	MSG_T_SHOW_CURRENT,
	MSG_T_NO_CHECK_COM_DATA,
	MSG_T_NO_BOARD_COM_DATA
};

enum {
	WPARAM_START,
	WPARAM_UART_ERR,
	WPARAM_START_TIMEOUT,
	WPARAM_STOP_TIMEOUT,
	WPARAM_AUDIO_LENGTH,
	WPARAM_MAC_SN,
	WPARAM_TFA9896_VALUE,
	WPARAM_GET_RIGHTAUD_FILE,
	WPARAM_GET_LEFTAUD_FILE,
	WPARAM_THREAD_EXITED,
	WPARAM_MES_STRING,
	WPARAM_MES_ERR,
	WPARAM_SN,

//	WPARAM_MES_TURE,
//	WPARAM_CYCLE_DIS
};


typedef struct tagMSGINFO {
	DWORD         dwMsgCh:8;      // 传递给界面的消息通道
	DWORD         dwMsgType:8;    // 传递给界面的消息类型
	DWORD         dwMsgRev:16;    // 传递给界面的
}MsgInfo;
#define SEND_USER_MSG(wnd,m,w,l)    ::SendMessage((wnd), (m), (w), (l));
#define SET_MSG_INFO(mpi, ch, type) do {(mpi)->lparam.u.dwMsgCh = (ch); (mpi)->lparam.u.dwMsgType=(type);}while(0)
#define SET_MSG_SEND(mpi, ch, type)                                                  \
	do {SET_MSG_INFO((mpi),(ch), (type));                                            \
		::PostMessage(mpi->hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)&mpi->lparam);   \
		Sleep(80);                                                                   \
	}while(0)

typedef union {
	DWORD         msg;
	MsgInfo       u;
}MsgLParam;;


