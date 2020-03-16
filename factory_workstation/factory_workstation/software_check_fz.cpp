// software_check_fz.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_check_fz.h"
#include "afxdialogex.h"

#pragma comment(lib, "winmm")
// software_check_fz 对话框

IMPLEMENT_DYNAMIC(software_check_fz, CDialogEx)

software_check_fz::software_check_fz(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_check_fz::IDD, pParent)
{

}

software_check_fz::~software_check_fz()
{
}

void software_check_fz::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC10, m_Epass);
	DDX_Control(pDX, IDC_CHECK1, current_checkBox);
	DDX_Control(pDX, IDC_LIST2, m_listEvt);
	DDX_Control(pDX, IDC_EDIT2, m_EditSn);
	DDX_Control(pDX, IDC_STATIC2, m_EditBtAddr);
	DDX_Control(pDX, IDC_EDIT_CUR_CURRENT, m_EditCurrect);
	DDX_Control(pDX, IDC_STC_BOOT, m_btnBoot);
	DDX_Control(pDX, IDC_STC_L35, m_btnL35);
	DDX_Control(pDX, IDC_STC_PA, m_btnPA);
	DDX_Control(pDX, IDC_STC_TOUCH, m_btnTouch);
	DDX_Control(pDX, IDC_STC_TKEYUP, m_btnKeyU);
	DDX_Control(pDX, IDC_STC_TKEYDOWN, m_btnKeyD);
	DDX_Control(pDX, IDC_STC_HWKEY, m_btnKeyHW);
	DDX_Control(pDX, IDC_STC_AUDIO, m_btnAud);
	DDX_Control(pDX, IDC_STC_SN_READ, m_snRead);
	DDX_Control(pDX, IDC_EDIT_NORMAL_CURRENT2, m_normalCurrect);
	DDX_Control(pDX, IDC_EDIT_SLEEP_CURRENT3, m_sleepCurrect);
	DDX_Control(pDX, IDC_CHECK3, sleep_current_checkBox);
	DDX_Control(pDX, IDC_CHECK5, sleep_current_delay_checkBox);
	DDX_Control(pDX, IDC_STATIC8, m_readSn);
}


BEGIN_MESSAGE_MAP(software_check_fz, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_check_fz::OnMSGMassProduct)
	ON_EN_CHANGE(IDC_EDIT2, &software_check_fz::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON13, &software_check_fz::OnBnClickedButton13)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_MUSIC1, &software_check_fz::OnBnClickedRadioMusic1)
	ON_BN_CLICKED(IDC_RADIO_MUSIC2, &software_check_fz::OnBnClickedRadioMusic2)
	ON_BN_CLICKED(IDC_RADIO_MUSIC3, &software_check_fz::OnBnClickedRadioMusic3)
	ON_BN_CLICKED(IDC_RADIO_MUSIC4, &software_check_fz::OnBnClickedRadioMusic4)
	ON_BN_CLICKED(IDC_RADIO_MUSIC5, &software_check_fz::OnBnClickedRadioMusic5)
	ON_BN_CLICKED(IDC_RADIO_MUSIC6, &software_check_fz::OnBnClickedRadioMusic6)
END_MESSAGE_MAP()

void software_check_fz::AddEvent2List(CString sText)
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

void software_check_fz::CheckNewStatus(MPIPtr mpi, MsgInfo *pmsg)
{
	BOOL snOK = FALSE;
	CString sText;

	if(!mpi || !pmsg)
		return;
	
	switch(mpi->bitStatusMsg) {
	case BIT_SYS_RUN96M:                 // Systen Run96M OK
		m_btnBoot.SetColor(STATIC_WND_SUCCESS);
		break;
	case BIT_KEY_TOUCH_UP:               // TouchUp OK
		m_btnKeyU.SetColor(STATIC_WND_SUCCESS);
		break;
	case BIT_KEY_TOUCH_DOWN:   
		m_btnKeyD.SetColor(STATIC_WND_SUCCESS);
		break;
	case BIT_CS47L35:          // CS47L35 OK
		m_btnL35.SetColor(STATIC_WND_SUCCESS);
		break;
	case BIT_SPEAKER:                    // TEA9896 OK
		m_btnPA.SetColor(STATIC_WND_SUCCESS);
		break;
	case BIT_KEY_TOUCH_INIT:             // TouchInit OK  
		m_btnTouch.SetColor(STATIC_WND_SUCCESS);
		break;
	case BIT_AUDIO_IN_OUT:
		m_btnAud.SetColor(STATIC_WND_SUCCESS);
		TRACE("音频收到\n");
		break;
	case BIT_KEY_BOARD:
		m_btnKeyHW.SetColor(STATIC_WND_SUCCESS);
		break;

	case BIT_SN_OK:
		m_readSn.SetWindowTextA((char*)mpi->sNoSN);
		sText.Format("%s", (char*)mpi->sSetSN);
		if( sText.Find("-") < 0 ){
			snOK = (memcmp(mpi->sNoSN, mpi->sSetSN, 18) == 0) ? TRUE : FALSE;
		}else{
			snOK = (memcmp(mpi->sNoSN, mpi->sSetSN, 19) == 0) ? TRUE : FALSE;
		}
		if(snOK) {
			m_snRead.SetColor(STATIC_WND_SUCCESS);
			m_snRead.SetWindowTextA("OK");
		}else {
			m_snRead.SetColor(STATIC_WND_WARNING);
			m_snRead.SetWindowTextA("ERR");
		}
		break;

	case BIT_ADDR_OK:
		m_EditBtAddr.SetWindowTextA((char*)mpi->sBtAddr);
		break;

	case BIT_FIRMVER_OK:
		sText.Format("%s", (char*)mpi->sFirmVer);
		GetDlgItem(IDC_EDIT_FIRMVER)->SetWindowTextA(sText);
		break;
	}
}

LRESULT software_check_fz::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	UINT a;
	CString sText;
	MsgInfo *pmsg = (MsgInfo*)lParam;
	MPIPtr mpi =  (MPIPtr)&m_MPInfo;
	if(mpi->burncheck_option_instance.g_current_test){
		sText.Format("%d", mpi->current);
		m_normalCurrect.SetWindowTextA(sText);
		sText.Format("%d", mpi->chagrcurrent);
		m_EditCurrect.SetWindowTextA(sText);
		if(mpi->burncheck_option_instance.g_sleep_current_test){//如果勾选休眠电流检测获取休眠电流
			sText.Format("%d", mpi->sleepcurrent);
			m_sleepCurrect.SetWindowTextA(sText);
		}
	}
	switch(pmsg->dwMsgType) {
		case MSG_T_STRING:
			AddEvent2List(CString((char*)mpi->sWndMsg));
			break;
		case MSG_T_CURRENT_ERR:
			sText.Format("系统没有加电");
			AddEvent2List(sText);
			AfxMessageBox(sText, MB_ICONERROR);
			break;
		case MSG_T_BTN_START:
			AddEvent2List("检测到板卡，等待测试");
			sText.Format("%d", mpi->current);
			m_normalCurrect.SetWindowTextA(sText);
			m_normalCurrect.SetColor(STATIC_WND_SUCCESS);
			break;
		case MSG_T_NO_CURRET:           //检测到电流，有板子
			AddEvent2List("没有检测到板卡加载");
			break;
		case MSG_T_ADDRSN:         //显示序列号和蓝牙
			sText.Format("%02x:%02x:%02x:%02x:%02x:%02x",
				mpi->sBtAddr[0],mpi->sBtAddr[1],mpi->sBtAddr[2],
				mpi->sBtAddr[3],mpi->sBtAddr[4],mpi->sBtAddr[5]);
			m_EditBtAddr.SetWindowTextA(sText);
			m_readSn.SetWindowTextA((char*)mpi->sNoSN);
			AddEvent2List(sText);
			AddEvent2List((char*)mpi->sNoSN);
			break;
		case MSG_T_BITCHECK:
			CheckNewStatus(mpi, pmsg);
			break;
		case MSG_T_KEY_ERR:
			for(UINT uID = IDC_RADIO_MUSIC1; uID <= IDC_RADIO_MUSIC6; uID++)
				GetDlgItem(uID)->EnableWindow(((mpi->bitStatus >= 15) ? TRUE : FALSE));
			break;
		case MSG_T_PLAYAUD:
			for(int id=IDC_RADIO_MUSIC1; id <= IDC_RADIO_MUSIC4; id++) {
				((CButton*)GetDlgItem(id))->SetCheck(0);
				((CButton*)GetDlgItem(id))->EnableWindow(TRUE);
			}
			AddEvent2List("已经播放音频");
			break;
		case MSG_T_AUDSELERR:
			sText.Format("音频选择错了");
			AddEvent2List(sText);
			mpi->audSelect = MAX_AUDIO_PLAY;
			break;
		case MSG_T_CHARGE_OPEN:
			sText.Format("打开充电");
			AddEvent2List(sText);
			break;
		case MSG_T_CHARGE_CLOSE:
			sText.Format("关闭充电");
			AddEvent2List(sText);
			break;		
		case MSG_T_CHARG_SUSS:
			m_EditCurrect.SetColor(STATIC_WND_SUCCESS);
			sText.Format("电流检测成功！");
			AddEvent2List(sText);
			break;
		case MSG_T_SLEEP_CMD:
			sText.Format("开始检测休眠电流！");
			AddEvent2List(sText);
			break;
		case MSG_T_SLEEP_CUR_SUSS:
			sText.Format("%d", mpi->sleepcurrent);
			m_sleepCurrect.SetWindowTextA(sText);
			m_sleepCurrect.SetColor(STATIC_WND_SUCCESS);
			sText.Format("休眠电流检测成功！");
			AddEvent2List(sText);
			break;
		case MSG_T_SLEEP_CUR_ERR:
			sText.Format("%d", mpi->sleepcurrent);
			m_sleepCurrect.SetWindowTextA(sText);
			m_sleepCurrect.SetColor(STATIC_WND_WARNING);
			sText.Format("休眠电流检测失败！值为%d", mpi->sleepcurrent);
			AddEvent2List(sText);
			break;
		case MSG_T_ALL_SUSS:
			sText.Format("------------------板卡检测完成\r\n");
			AddEvent2List(sText);
			m_Epass.SetWindowTextA("PASS!");
			pass_green_color = 1;
			Invalidate();	
			//UpdateWindow();
			break;
		case MSG_T_SHOW_CURRENT:
			break;
		case MSG_T_MES_STRING:
			sText.Format(_T("mes返回数据:%s"),mpi->Mesresring);
			a = sText.Find("}") + 1;
			if(a > 0)
				sText = sText.Left(a);
			else
				sText = sText;
			AddEvent2List(sText);
			break;
		case MSG_T_SMES_SEND:         //mes烧写开始
			sText.Format("SN号:%s  mes判断结果:%d", mpi->sSetSN,mpi->Mesvalue);
			AddEvent2List(sText);
			break;
		case MSG_T_EMES_ERR:
			sText.Format("mes连不上服务器！");
			AfxMessageBox("mes连不上服务器! ");
			AddEvent2List(sText);
			break;
		default:
			break;
	}
	return 0;
}

int software_check_fz::WorkerThreadProc(void *param)
{
	int current_timedelay;
	UINT bitStat;
	CTime tNow = CTime::GetCurrentTime();
	CTime t_sleepcmd_time;
	MPIPtr  mpi = (MPIPtr)param;
	CContrlComm *pComBoard =  &mpi->comInfo[COM_No_BOARD], *pComCheck = &mpi->comInfo[COM_No_CHECK];
	CCurlCtrl *mesptr = &mpi->mesInfo;
	Sleep(500);
	if(pComBoard->GetPort() < 0)
		sprintf_s((char*)mpi->sWndMsg, 256, "未配置");
	else
		sprintf_s((char*)mpi->sWndMsg, 256, "端口 %d %d", pComBoard->portCom,  pComCheck->portCom);
	SET_MSG_INFO(mpi, mpi->ch, MSG_T_STRING);
	::SendMessage(mpi->hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)&mpi->lparam);
	Sleep(20);

	if(pComBoard->GetPort() < 0)
		goto out;

	// 打开串口设备
	if(FALSE == pComBoard->InitComm(pComBoard->portCom, DEIGHT, SONE, PNONE, 115200)) {
		sprintf_s((char*)mpi->sWndMsg, WND_MSG_STR_SIZE, "打开board串口失败");
		SET_MSG_SEND(mpi, mpi->ch, MSG_T_STRING);
	}

	if(FALSE == pComCheck->InitComm(pComCheck->portCom, DEIGHT, SONE, PNONE, 115200)) {
		sprintf_s((char*)mpi->sWndMsg, WND_MSG_STR_SIZE, "打开check串口失败");
		SET_MSG_SEND(mpi, mpi->ch, MSG_T_STRING);
	}else {
		pComCheck->SendComand(CHARG_SHUT_COM);      // 关闭充电
		pComCheck->SendComand(CURRENT_UART_OPEN); //打开串口
	}

	if(mpi->burncheck_option_instance.g_sleep_current_delay)
		current_timedelay = 32;
	else
		current_timedelay = 2;
	memset(mpi->sWndMsg, 0, WND_MSG_STR_SIZE);
	mpi->tStatus = CTime::GetCurrentTime();
	srand((unsigned int)::GetTickCount());
	while(0 == mpi->doExit) {
		tNow = CTime::GetCurrentTime();
		pComBoard->Process();
		pComCheck->Process();
		if(mpi->burncheck_option_instance.g_current_test){
			mpi->chagrcurrent = pComCheck->GetChargCurrent();
			mpi->current = pComCheck->GetCurrent();                   //获取电流
			if(mpi->burncheck_option_instance.g_sleep_current_test){//如果勾选休眠电流检测获取休眠电流
				mpi->sleepcurrent = pComCheck->GetSleepCurrent();
			}
			if(mpi->current > mpi->burncheck_option_instance.g_addlarge) {       // 系统出错
				if(mpi->runStatus < RUNST_START_WRITE1 ) {
					ChangeNewStatuc(mpi, RUNST_CURRENT_ERR, __LINE__);
					SET_MSG_SEND(mpi, mpi->ch, MSG_T_CURRENT_ERR);
				}
			}else if( (mpi->current > mpi->burncheck_option_instance.g_addCurrent) && 
						(mpi->current <= mpi->burncheck_option_instance.g_addlarge)) {         // 检测到板卡电流
				if(mpi->runStatus < RUNST_GET_BOARD) {
					ChangeNewStatuc(mpi, RUNST_CHECK_INIT, __LINE__);
					SET_MSG_SEND(mpi, mpi->ch, MSG_T_BTN_START);
				}
			}else {                               // 没有检测到板卡
				if(mpi->runStatus < RUNST_GET_BOARD) {  // 充电时会为0，所以只能在开始的时候检查
					if(RUNST_WAIT_BOARD != mpi->runStatus) {
						ChangeNewStatuc(mpi, RUNST_WAIT_BOARD, __LINE__);
						TRACE("Set Wait Board when no current\r\n");
						SET_MSG_SEND(mpi, mpi->ch, MSG_T_NO_CURRET);
					}
				}
			}
		}else{
			if(mpi->runStatus < RUNST_GET_BOARD) {
				ChangeNewStatuc(mpi, RUNST_CHECK_INIT, __LINE__);
				//SET_MSG_SEND(mpi, mpi->ch, MSG_T_BTN_START);
			}
		}

		switch(mpi->runStatus) {
			case RUNST_CHECK_INIT:                   //启动校验板子的状态，由板子打印的数据获得   7
				bitStat = pComBoard->GetStatus();

				if(bitStat != mpi->bitStatus) {
					for(int i = 0; i < BIT_END; i++) {
						if((bitStat & (1<<i)) && (!(mpi->bitStatus & (1<<i)))) {
							mpi->bitStatusMsg = i;   //0到7
							mpi->bitStatus |= (1<<i);
							TRACE("mpi->bitStatus:%d\n",mpi->bitStatus);
							TRACE("=====================MSG BIT:%d\n", i);
							if(BIT_SN_OK == i) {
								memset(mpi->sNoSN, 0, 20);
								if( find___str(pComBoard->devSN) )
									memcpy(mpi->sNoSN, pComBoard->devSN, 19);
								else
									memcpy(mpi->sNoSN, pComBoard->devSN, 18);
								CString sText;
								BOOL snOK = FALSE;
								sText.Format("%s", (char*)mpi->sSetSN);
								if( sText.Find("-") < 0 ){
									snOK = (memcmp(mpi->sNoSN, mpi->sSetSN, 18) == 0) ? TRUE : FALSE;
								}else{
									snOK = (memcmp(mpi->sNoSN, mpi->sSetSN, 19) == 0) ? TRUE : FALSE;
								}
								if(!snOK) {
									AfxMessageBox("sn号不一致，检查是否烧写不一致！");  
									ChangeNewStatuc(mpi, 100, __LINE__);
								}
							}
							if(BIT_ADDR_OK == i) {
								memcpy(mpi->sBtAddr, pComBoard->devAddr, 20);
							}
							if(BIT_FIRMVER_OK == i) {
								memcpy(mpi->sFirmVer, pComBoard->firmVer, 16);
							}
							SET_MSG_SEND(mpi, mpi->ch, MSG_T_BITCHECK);
							if(mpi->general_option_instance.g_check_mes  == 1){
								if(BIT_SN_OK == i){
									if(mpi->general_option_instance.g_check_mes_type == 1){
										if(mesptr->SendChecks((LPCSTR)mpi->sNoSN) == 0)   {                       //上传MES流程检查接口    check模式第一个MES
											mpi->Mesvalue = mesptr->Messtatus;
											strncpy_s(mpi->Mesresring,mesptr->line,strlen(mesptr->line));
											SET_MSG_SEND(mpi,mpi->ch,MSG_T_MES_STRING);                     //显示mes返回字符
											SET_MSG_SEND(mpi,mpi->ch,MSG_T_SMES_SEND);
										}else
											SET_MSG_SEND(mpi,mpi->ch,MSG_T_EMES_ERR);

										if(mpi->Mesvalue == 1){                                     //mes check判断

										}else{
											ChangeNewStatuc(mpi, 100, __LINE__);
											AfxMessageBox("mes返回数据错误，不可以继续check！ ");
										}
									}
								}
							}
							break;
						}
					}
				}

				if(mpi->bitStatus == BIT_START_INIT_FZ) {         // 检测到启动后，每三秒发一次key_dbg命令，防止命令没有正确接收
					if((tNow.GetTime() - mpi->tStatus.GetTime()) >= 3) {
						mpi->tStatus = CTime::GetCurrentTime();
						pComBoard->SendComand(KEY_COM);           // 打开按键调试					
					}
				}
			
				if((mpi->bitStatus & BIT_KEY_CHECKED_FZ) == BIT_KEY_CHECKED_FZ) {               /////开始音频文件的播放   127     mpi->bitStatus  8个显示颜色的标志位
					SET_MSG_SEND(mpi, mpi->ch, MSG_T_KEY_ERR);             //发消息控制按钮的状态
					ChangeNewStatuc(mpi, RUNST_CHECK_AUDIO, __LINE__);
				
					pComBoard->bitStatus = 0;                   // 进入测试固件前，进一步的去检测启动信息
					pComBoard->SendComand(CHECK_COM);           // checkfirm on
					TRACE("Enter CheckFirm\r\n");
				}
				break;

			case RUNST_CHECK_AUDIO:
				bitStat = pComBoard->GetStatus();
				if((mpi->bitStatus&BIT_START_INIT_FZ) == BIT_START_INIT_FZ) {   // 启动完成
					pComBoard->SendComand(Play_COM);              // ad2out on
					Sleep(50);
					mpi->audNo = rand() % MAX_AUDIO_PLAY;                  //rand()产生随机数
					mpi->iAudSetloops += 1;
					mpi->playTimes = 1;
					mpi->tLastPlay = CTime::GetCurrentTime();
					SET_MSG_SEND(mpi, mpi->ch, MSG_T_PLAYAUD);
					ChangeNewStatuc(mpi, RUNST_AUDIO_WAITUSER, __LINE__);
				}
				break;

			case RUNST_AUDIO_WAITUSER: // Wait User Select current play                  //选择按钮
				if(mpi->audSelect != mpi->audNo) {
					if((tNow.GetTime() - mpi->tLastPlay.GetTime()) >= 4) {
						if((mpi->playTimes % 2) == 0) {
							pComBoard->SendComand(Play_COM);              // 再次发送 ad2out on
							Sleep(100);
						}
						mpi->audNo = rand() % MAX_AUDIO_PLAY;                  //rand()产生随机数
						mpi->iAudSetloops += 1;
						mpi->playTimes += 1;
						mpi->tLastPlay = CTime::GetCurrentTime();
					}
				}
				if(mpi->audNo == mpi->audSelect) {       // 每两个音频文件对应一个用户选择
					if(0 == mpi->burncheck_option_instance.g_current_test)        // 没有电流检测,
						ChangeNewStatuc(mpi, RUNST_ALL_SUESS, __LINE__);
					else
						ChangeNewStatuc(mpi, RUNST_CHECK_CHARG_CURRET, __LINE__);
					mpi->bitStatusMsg = BIT_AUDIO_IN_OUT;
					SET_MSG_SEND(mpi, mpi->ch, MSG_T_BITCHECK);    // 显示音频绿色
					pComBoard->SendComand(STOP_PLAY_COM);       // 关闭音频
				}else if(MAX_AUDIO_PLAY != mpi->audSelect) {   // 用户选择出错
					SET_MSG_SEND(mpi, mpi->ch, MSG_T_AUDSELERR);
				}
				break;

			case RUNST_CHECK_CHARG_CURRET:                  //充电电流检测
				if(pComCheck->SendComand(CHARG_OPEN_COM) == TRUE) {           // 打开充电命令
					TRACE("mpi->chagrcurrent:%d\n",mpi->chagrcurrent);				
					SET_MSG_SEND(mpi, mpi->ch, MSG_T_CHARGE_OPEN);
					ChangeNewStatuc(mpi, RUNST_CHECK_WAIT_CHARGE, __LINE__);
				}
				break;

			case RUNST_CHECK_WAIT_CHARGE:                    //检测电流
				if(mpi->chagrcurrent > mpi->burncheck_option_instance.g_chargCurrent) {    // 200mA
					SET_MSG_SEND(mpi, mpi->ch, MSG_T_CHARG_SUSS);
					TRACE("mpi->chagrcurrent success:%d\n",mpi->chagrcurrent);
					if(mpi->burncheck_option_instance.g_sleep_current_test)
						ChangeNewStatuc(mpi, RUNST_CHECK_SLEEP_CURRENT, __LINE__);
					else
						ChangeNewStatuc(mpi, RUNST_ALL_SUESS, __LINE__);
					pComCheck->SendComand(CHARG_SHUT_COM);   // 关闭充电
					SET_MSG_SEND(mpi, mpi->ch, MSG_T_CHARGE_CLOSE);
				}else {
					if((tNow.GetTime() - mpi->tStatus.GetTime()) >= 2) {
						pComCheck->SendComand(CHARG_OPEN_COM);        // 再次发送打开充电命令
						mpi->tStatus = tNow;
						TRACE("charge reopen\n");
					}
				}
				break;
			case RUNST_CHECK_SLEEP_CURRENT:                    //检测休眠电流
				pComBoard->SendComand(SLEEP);
				pComCheck->SendComand(CURRENT_UART_CLOSE);
				t_sleepcmd_time = CTime::GetCurrentTime();
				SET_MSG_SEND(mpi, mpi->ch, MSG_T_SLEEP_CMD);
				ChangeNewStatuc(mpi, RUNST_CHECK_WAIT_SLEEP, __LINE__);
				break;
			case RUNST_CHECK_WAIT_SLEEP:                    //检测休眠电流
				if((tNow.GetTime() - t_sleepcmd_time.GetTime()) >= current_timedelay) {//sleep命令发完会等30秒才休眠
					if(mpi->sleepcurrent < mpi->burncheck_option_instance.g_sleepCurrent){
						pComCheck->SendComand(CURRENT_UART_OPEN);
						SET_MSG_SEND(mpi, mpi->ch, MSG_T_SLEEP_CUR_SUSS);
						ChangeNewStatuc(mpi, RUNST_ALL_SUESS, __LINE__);
					}else{
						SET_MSG_SEND(mpi, mpi->ch, MSG_T_SLEEP_CUR_ERR);
						ChangeNewStatuc(mpi, 100, __LINE__);
					}
				}
				break;
			case RUNST_ALL_SUESS:
				ChangeNewStatuc(mpi, RUNST_ALL_SUCC_IDLE, __LINE__);						
				if(mpi->general_option_instance.g_check_mes  == 1){		
					if(mpi->general_option_instance.g_check_mes_type == 1){
						if(mesptr->SendChecke((LPCSTR)mpi->sNoSN) == 0){               //check 第二个MES的发送
							mpi->Mesvalue = mesptr->Messtatus;
							memcpy(mpi->Mesresring,mesptr->line,strlen(mesptr->line));
							SET_MSG_SEND(mpi,mpi->ch,MSG_T_MES_STRING); 
							SET_MSG_SEND(mpi,mpi->ch,MSG_T_EMES_SEND);
						}else
							SET_MSG_SEND(mpi,mpi->ch,MSG_T_EMES_ERR);
						if(mpi->Mesvalue == 3){
							SET_MSG_SEND(mpi, mpi->ch, MSG_T_ALL_SUSS);
						}else{
							AfxMessageBox("mes返回数据错误，不可以继续check！ ");
						}
					}else if(mpi->general_option_instance.g_check_mes_type == 2){
						struct ch_mes_data ch_mes_data; 
						ch_mes_data.data_flag = 2;
						mesptr->changh_mes_int(mpi->sNoSN, ch_mes_data);
					}
				}
				SET_MSG_SEND(mpi, mpi->ch, MSG_T_ALL_SUSS);
				break;

			case RUNST_ALL_SUCC_IDLE:
				if((tNow.GetTime() - mpi->tStatus.GetTime()) >= 1) {
					pComCheck->SendComand(CHARG_SHUT_COM);      // 关闭充电
					pComBoard->SendComand(STOP_PLAY_COM);       // 关闭音频
					TRACE("CLOSE CHARGE & AUDIO2OUT\r\n");				
					mpi->tStatus = CTime::GetCurrentTime();
				}
				//SET_MSG_SEND(mpi, mpi->ch, MSG_T_SHOW_CURRENT);      // 显示电流
				break;

			default:
				//SET_MSG_SEND(mpi, mpi->ch, MSG_T_SHOW_CURRENT);      // 显示电流
				break;
		}
		Sleep(1);
	}

out:
	TRACE("End doExit=%d\r\n", mpi->doExit);
	mpi->idThread = 0;
	pComBoard->EndComm();
	pComCheck->EndComm();
	return 0;
}
int software_check_fz::PlaysoundThreadProc(void *param)
{
	static BYTE _audfile[6][2] = {{1,2}, {1,3}, {1,4}, {2,3}, {2,4}, {3,4}};
	UINT fNo;
	MPIPtr  mpi = (MPIPtr)param;

	while(0 == mpi->doExit) {
		if(mpi->iAudSetloops != mpi->iAudPlayloops) {
			TRACE("PlayaudNO1=%d\n", _audfile[mpi->audNo][0]);
			::waveOutSetVolume(0, 0xFFFF);          // LEFT
			fNo = IDR_WAVE7 + _audfile[mpi->audNo][0] - 1;
			PlaySound((LPCTSTR)fNo, NULL, SND_NODEFAULT|SND_SYNC|SND_RESOURCE);

			if(0 != mpi->doExit) break;
			
			TRACE("PlayaudNO2=%d\n", _audfile[mpi->audNo][1]);
			::waveOutSetVolume(0, 0xFFFF<<16);      // RIGHT
			fNo = IDR_WAVE7 + _audfile[mpi->audNo][1] - 1;
			PlaySound((LPCTSTR)fNo, NULL, SND_NODEFAULT|SND_SYNC|SND_RESOURCE);

			mpi->iAudPlayloops = mpi->iAudSetloops;
		}
		Sleep(1);
	}
	
	TRACE("EndAud doExit=%d\r\n", mpi->doExit);
	mpi->idAudThread = 0;
	return 0;
}

// software_check_fz 消息处理程序

void software_check_fz::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString sText;
	UpdateData(TRUE);
	m_EditSn.GetWindowTextA(sText);
	if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		if( sText.Find("-") < 0 ){
			if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("当前产品SN号(v2)不正确");
				m_MPInfo.bcheckStart = FALSE;
				m_MPInfo.bEnableWriter = FALSE;
			}else{
				if( check_firmware_canuse_v2sn(burncheck_option_instance.g_wrFirm) ){
					m_MPInfo.bcheckStart = TRUE;
					m_MPInfo.bEnableWriter = TRUE;
				}else{
					m_MPInfo.bcheckStart = FALSE;
					m_MPInfo.bEnableWriter = FALSE;					
				}
			}
			memset((void*)m_MPInfo.sSetSN, 0, sizeof(m_MPInfo.sSetSN));
			memcpy((void*)m_MPInfo.sSetSN, sText, 18);
		}
	}
	if(sText.GetLength() >= 19){
		if(CheckSN(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
			AfxMessageBox("当前产品SN号(v1)不正确");
			m_MPInfo.bcheckStart = FALSE;
			m_MPInfo.bEnableWriter = FALSE;
		}else{
			m_MPInfo.bcheckStart = TRUE;
			m_MPInfo.bEnableWriter = TRUE;
		}
		memset((void*)m_MPInfo.sSetSN, 0, sizeof(m_MPInfo.sSetSN));
		memcpy((void*)m_MPInfo.sSetSN, sText, 19);
	}
	// TODO:  在此添加控件通知处理程序代码
}

void software_check_fz::SetAllItemRestart(void)
{
	CString sText;
	m_EditBtAddr.SetWindowTextA("");
	m_EditSn.SetWindowTextA("");
	m_readSn.SetWindowTextA("");
	m_EditCurrect.SetWindowTextA("");
	m_sleepCurrect.SetWindowTextA("");
	m_Epass.SetWindowTextA("");
	m_Epass.SetWindowTextA("");

	m_btnBoot.SetColor(STATIC_WND_DISABLE);
	m_btnKeyU.SetColor(STATIC_WND_DISABLE);
	m_btnKeyD.SetColor(STATIC_WND_DISABLE);
	m_btnKeyHW.SetColor(STATIC_WND_DISABLE);
	m_btnL35.SetColor(STATIC_WND_DISABLE);
	m_btnPA.SetColor(STATIC_WND_DISABLE);
	m_btnTouch.SetColor(STATIC_WND_DISABLE);
	m_btnAud.SetColor(STATIC_WND_DISABLE);
	m_EditCurrect.SetColor(STATIC_WND_DISABLE);
	m_normalCurrect.SetColor(STATIC_WND_DISABLE);
	m_sleepCurrect.SetColor(STATIC_WND_DISABLE);

	m_snRead.SetColor(STATIC_WND_DISABLE);
	m_snRead.SetWindowTextA("Wait");

	for(int id=IDC_RADIO_MUSIC1; id <= IDC_RADIO_MUSIC6; id++) {
		((CButton*)GetDlgItem(id))->SetCheck(0);
		((CButton*)GetDlgItem(id))->EnableWindow(FALSE);
	}
	
	sText.Format("0.00.00", m_MPInfo.ch);
	GetDlgItem(IDC_EDIT_FIRMVER)->SetWindowText(sText);

	m_listEvt.ResetContent();
}
extern void ChangeNewStatuc(MPIPtr  mpi, UINT status, int line = -1);
void software_check_fz::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	MPIPtr mpi = (MPIPtr)&m_MPInfo;
	SetAllItemRestart();
	AddEvent2List("用户复位检测");
	m_EditSn.SetFocus();
	MassProdReinit(mpi);
}

void software_check_fz::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	burncheck_option_instance = g_p->burncheck_option_instance;
}

BOOL software_check_fz::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	CString dir_path;
	if(last_sel_soft.Find("方舟") >= 0){
		fz_jn_flag = 1;
		dir_path = LOG_PATH_CHECK_fz;
	}else if (last_sel_soft.Find("金牛") >= 0){
		fz_jn_flag = 2;
		dir_path = LOG_PATH_CHECK_jn;
	}

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

	current_checkBox.EnableWindow(FALSE);
	current_checkBox.SetCheck(burncheck_option_instance.g_current_test);
	sleep_current_checkBox.EnableWindow(FALSE);
	sleep_current_checkBox.SetCheck(burncheck_option_instance.g_sleep_current_test);
	sleep_current_delay_checkBox.EnableWindow(FALSE);
	sleep_current_delay_checkBox.SetCheck(burncheck_option_instance.g_sleep_current_delay);
	if(burncheck_option_instance.g_current_test){
		if(!burncheck_option_instance.g_sleep_current_test){
			((CButton*)GetDlgItem(IDC_STATIC_sleep))->EnableWindow(FALSE);	
		}
	}else{
		((CButton*)GetDlgItem(IDC_STATIC_charge))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_STATIC_normal))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_STATIC_sleep))->EnableWindow(FALSE);	
	}
	m_editFont.CreatePointFont(700, "宋体");
	m_Epass.SetFont(&m_editFont);

	DWORD idThread;
	MPIPtr mpi = (MPIPtr)&m_MPInfo;
	mpi->runStatus = RUNST_NONE;
	mpi->hWnd       = this->m_hWnd;
	mpi->doExit    = 0;
	mpi->bEnableWriter = FALSE;
	MassProdReinit(mpi);
	SetAllItemRestart();
	mpi->comInfo[COM_No_BOARD].portCom = general_option_instance.g_ttycom_port;
	mpi->comInfo[COM_No_CHECK].portCom = burncheck_option_instance.g_currentcom_port;
	mpi->general_option_instance = general_option_instance;
	mpi->burncheck_option_instance = burncheck_option_instance;
	mpi->fz_jn_flag = fz_jn_flag;
	mpi->comInfo[COM_No_BOARD].g_out_str = burncheck_option_instance.g_out_str;
	mpi->comInfo[COM_No_CHECK].g_out_str = burncheck_option_instance.g_out_str;
	mpi->ch = 0;
	mpi->mesInfo.g_mes_ip = general_option_instance.g_mes_ip;
	mpi->mesInfo.g_mes_port = general_option_instance.g_mes_port;
	mpi->hThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_fz::WorkerThreadProc, mpi, 0, &idThread);
	mpi->idThread  = idThread;

	mpi->hAudThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_fz::PlaysoundThreadProc, mpi, 0, &idThread);
	mpi->idAudThread  = idThread;
	mpi->iAudSetloops = 0;
	mpi->iAudPlayloops = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
}


HBRUSH software_check_fz::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(pWnd->GetDlgCtrlID()==IDC_STATIC10)//如果是静态编辑框
    {
		if(pass_green_color){
			pDC->SetTextColor(RGB(0,255,0));//修改字体的颜色
			pDC->SetBkMode(TRANSPARENT);//把字体的背景变成透明的
		}else{
		    pDC->SetTextColor(RGB(255,0,0));//修改字体的颜色
			pDC->SetBkMode(TRANSPARENT);//把字体的背景变成透明的
		}
    }

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void software_check_fz::CheckUserSelectAud(int iSelectNo)
{
	MPIPtr mpi = (MPIPtr)&m_MPInfo;
	mpi->audSelect = iSelectNo;
}

void software_check_fz::OnBnClickedRadioMusic1()
{
	// TODO: 在此添加控件通知处理程序代码
	CheckUserSelectAud(0);
}


void software_check_fz::OnBnClickedRadioMusic2()
{
	// TODO: 在此添加控件通知处理程序代码
	CheckUserSelectAud(1);
}


void software_check_fz::OnBnClickedRadioMusic3()
{
	// TODO: 在此添加控件通知处理程序代码
	CheckUserSelectAud(2);
}


void software_check_fz::OnBnClickedRadioMusic4()
{
	// TODO: 在此添加控件通知处理程序代码
	CheckUserSelectAud(3);
}


void software_check_fz::OnBnClickedRadioMusic5()
{
	// TODO: 在此添加控件通知处理程序代码
	CheckUserSelectAud(4);
}


void software_check_fz::OnBnClickedRadioMusic6()
{
	// TODO: 在此添加控件通知处理程序代码
	CheckUserSelectAud(5);
}


BOOL software_check_fz::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

