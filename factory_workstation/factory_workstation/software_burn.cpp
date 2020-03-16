// software_burn.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_burn.h"
#include "afxdialogex.h"

// software_burn �Ի���

IMPLEMENT_DYNAMIC(software_burn, CDialogEx)

software_burn::software_burn(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_burn::IDD, pParent)
{

}

software_burn::~software_burn()
{
	m_hLogFile.m_hFile = INVALID_HANDLE_VALUE;
}

void software_burn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_WrProgress);
	DDX_Control(pDX, IDC_STATIC9, m_Epass);
	DDX_Control(pDX, IDC_LIST1, m_listEvt);
	DDX_Control(pDX, IDC_EDIT2, m_EditSn);
	DDX_Control(pDX, IDC_STATIC4, m_EditBtAddr);
	DDX_Control(pDX, IDC_STATIC7, m_snRead);
	DDX_Control(pDX, IDC_STATIC5, m_readSn);
}


BEGIN_MESSAGE_MAP(software_burn, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_burn::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON1, &software_burn::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &software_burn::OnEnChangeEdit2)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// software_burn ��Ϣ�������

void software_burn::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	burncheck_option_instance = g_p->burncheck_option_instance;
}

void ChangeNewStatuc(MPIPtr  mpi, UINT status, int line = -1)
{
	mpi->tStatus = CTime::GetCurrentTime();    //״̬�ı��ʱ��
	TRACE("------------LINE:%d, ChangeNewStatus %d -->%d\n", line, mpi->runStatus, status);
	mpi->runStatus = status;
}

UINT CreateCLPrograma(MPIPtr  mpi, UINT type)
{
	CString sCommand;
	CString g_clProg = G_CLPROG;
	CString g_wrParam;
	CString g_binpath_reset;
	mpi->execProcess.Create(CExecProcess::ASYNC);
	
	if(mpi->fz_jn_flag == 1)
		g_wrParam = mpi->burncheck_option_instance.g_wrFirm_folder + "\\nvparam_fz.bin";
	else if(mpi->fz_jn_flag == 2){
		g_wrParam = mpi->burncheck_option_instance.g_wrFirm_folder + "\\nvparam_jn.bin";
	}
	g_binpath_reset = mpi->burncheck_option_instance.g_wrFirm_folder + "\\reset_cmd.bin";
	switch(type) {
	case RUNST_START_WRITE0:
		sCommand.Format("%s --gdb-cmd \"\\\"%s\\\" -if SWD -device Cortex-M0 \
			-singlerun -silent -speed 12000\" gdbserver chip_erase_qspi",
			g_clProg, mpi->burncheck_option_instance.g_sJLinkPath);
		break;
	case RUNST_START_WRITE1:
		mpi->iWrFileSize = (1<<20);
		sCommand.Format("%s --gdb-cmd \"\\\"%s\\\" -if SWD -device Cortex-M0 -singlerun -silent -speed 12000\" gdbserver write_qspi 0 %s",
			g_clProg, mpi->burncheck_option_instance.g_sJLinkPath, mpi->burncheck_option_instance.g_wrFirm);
		break;
	case RUNST_START_WRITE2:                    ////��д�����ļ�
		CreateParamFile(g_wrParam, mpi);
		sCommand.Format("%s --gdb-cmd \"\\\"%s\\\" -if SWD -device Cortex-M0 -singlerun -silent -speed 12000\" gdbserver write_qspi 0x0C3000 %s.%d",
			g_clProg, mpi->burncheck_option_instance.g_sJLinkPath, g_wrParam, mpi->ch);
		SET_MSG_SEND(mpi, mpi->ch, MSG_T_ADDRSN);
		break;
	case RUNST_START_RESET:
		sCommand.Format("%s --gdb-cmd \"\\\"%s\\\" -if SWD -device Cortex-M0 -singlerun -silent -speed 12000\" gdbserver write 0x400c3050 %s",
			g_clProg, mpi->burncheck_option_instance.g_sJLinkPath,g_binpath_reset);
		Sleep(100);
		sCommand.Format("%s --gdb-cmd \"\\\"%s\\\" -if SWD -device Cortex-M0 -singlerun -silent -speed 12000\" gdbserver write 0x400c3050 %s",
			g_clProg, mpi->burncheck_option_instance.g_sJLinkPath,g_binpath_reset);
		SET_MSG_SEND(mpi, mpi->ch, MSG_T_RESTART);
		break;
	}

	TRACE("%s\n", sCommand);
	mpi->execProcess.Execute(sCommand);

	return 0;
}
INT GetLineCLPrograma(MPIPtr  mpi, UINT type)
{
	int tmp, readed, less = MAX_PIPELINE_LEN -  mpi->outLineLen;
	
	mpi->outLineLen = 0;
	readed = mpi->execProcess.ReadLine(&mpi->pipeData[mpi->outLineLen], less);

	if(readed == 0) 
		return 0;
	
	mpi->pipeData[readed] = '\0';
	if(readed > 2)
		TRACE("LEN%02d:\t%s", readed, (char*)mpi->pipeData);
	CString sText(mpi->pipeData), sDigit;

	if(RUNST_START_WRITE0 == type) { // Erase End
		if((tmp=sText.Find("cli_programmer")) >= 0) {
			mpi->iWriteProgress = 10;
			SET_MSG_SEND(mpi, mpi->ch, MSG_T_PROGRESS);
		}else if((tmp=sText.Find("done")) >= 0) {
			mpi->iWriteProgress = 100;
			SET_MSG_SEND(mpi, mpi->ch, MSG_T_PROGRESS);
			
			ChangeNewStatuc(mpi, RUNST_START_WRITE1, __LINE__);
			CreateCLPrograma(mpi, mpi->runStatus);     // Write firm
		}else if((tmp=sText.Find("cannot open gdb interface")) >= 0) {
			SET_MSG_SEND(mpi, mpi->ch, MSG_T_JLINK_ERR);
			ChangeNewStatuc(mpi, RUNST_SYS_ERROR, __LINE__);
		}
	}else if(RUNST_START_WRITE1 == type) { 
		if((tmp=sText.Find("cli_programmer")) >= 0) {
			mpi->iWriteProgress = 110;
			SET_MSG_SEND(mpi, mpi->ch, MSG_T_PROGRESS);
		}if(sText.Find("Writing to address") >= 0) {
			int cur_write = 0;
			if((tmp = sText.Find("offset: ")) < 0)
				return -1;
			tmp += 8;

			sDigit = sText.Mid(tmp, 10);
			cur_write = tmp = strtoul(sDigit, (char**)NULL, 16);
			if(tmp == 0 && mpi->iWriteProgress > 110)
				cur_write = mpi->iWrFileSize;

			mpi->iWriteProgress = 110 + ((cur_write * 790) / mpi->iWrFileSize);
			SET_MSG_SEND(mpi, mpi->ch, MSG_T_PROGRESS);
		}else if((tmp=sText.Find("done")) >= 0) {
			mpi->iWriteProgress = 900;
			SET_MSG_SEND(mpi, mpi->ch, MSG_T_PROGRESS);
			ChangeNewStatuc(mpi, RUNST_START_WRITE2, __LINE__);
			CreateCLPrograma(mpi, mpi->runStatus);   // Write Param
		}else if((tmp=sText.Find("cannot open gdb interface")) >= 0) {
			SET_MSG_SEND(mpi, mpi->ch, MSG_T_JLINK_ERR);
			ChangeNewStatuc(mpi, RUNST_SYS_ERROR, __LINE__);
		}
	}else if(RUNST_START_WRITE2 == type) {       // Write Param
		if((tmp=sText.Find("cli_programmer")) >= 0) {
			mpi->iWriteProgress = 910;
			SET_MSG_SEND(mpi, mpi->ch, MSG_T_PROGRESS);
		}else if((tmp=sText.Find("done")) >= 0) {
			mpi->iWriteProgress = 1000;
			SET_MSG_SEND(mpi, mpi->ch, MSG_T_PROGRESS);
			
			ChangeNewStatuc(mpi, RUNST_START_RESET, __LINE__);  //��һ�����ص�
			CreateCLPrograma(mpi, mpi->runStatus);
		}else if((tmp=sText.Find("cannot open gdb interface")) >= 0) {
			SET_MSG_SEND(mpi, mpi->ch, MSG_T_JLINK_ERR);
			ChangeNewStatuc(mpi, RUNST_SYS_ERROR, __LINE__);
		}
	}else if(RUNST_START_RESET == type) {                        //��������
	    if((tmp=sText.Find("done")) >= 0) {
			mpi->iWriteProgress = 1000;		
			ChangeNewStatuc(mpi, RUNST_CHECK_INIT, __LINE__);
			TRACE("�����������\r\n");
		}else if((tmp=sText.Find("cannot open gdb interface")) >= 0) {
			SET_MSG_SEND(mpi, mpi->ch, MSG_T_JLINK_ERR);
			ChangeNewStatuc(mpi, RUNST_SYS_ERROR, __LINE__);
		}
	}
	return 0;
}

int need_file_ifexit(MPIPtr  mpi)
{
	CString sText;
	sText = G_CLPROG;
	if(PathFileExists(mpi->burncheck_option_instance.g_sJLinkPath) != TRUE){
		AfxMessageBox("J-Link·�����ó���");
		return 1;
	}
	if(PathFileExists(mpi->burncheck_option_instance.g_wrFirm) != TRUE){
		AfxMessageBox("�̼�·�����ó���");
		return 1;
	}
	if(PathFileExists(sText) != TRUE){
		AfxMessageBox("cli_programmer.exe�ļ�δ�ҵ�");
		return 1;
	}
	if(PathFileExists(mpi->burncheck_option_instance.g_wrFirm_folder+"\\reset_cmd.bin") != TRUE){
		AfxMessageBox("δ�ҵ�reset_cmd.bin�ļ�");
		return 1;
	}
	if(mpi->fz_jn_flag == 1){
		if(PathFileExists(mpi->burncheck_option_instance.g_wrFirm_folder+"\\nvparam_fz.bin") != TRUE){
			AfxMessageBox("δ�ҵ�nvparam_fz.bin�ļ�");
			return 1;
		}
	}else if(mpi->fz_jn_flag == 2){
		if(PathFileExists(mpi->burncheck_option_instance.g_wrFirm_folder+"\\nvparam_jn.bin") != TRUE){
			AfxMessageBox("δ�ҵ�nvparam_jn.bin�ļ�");
			return 1;
		}	
	}
	return 0;
}

int software_burn::WorkerThreadProc(void *param)
{
	UINT bitStat;
	CTime tNow = CTime::GetCurrentTime();;
	MPIPtr  mpi = (MPIPtr)param;
	CContrlComm *pComBoard =  &mpi->comInfo[COM_No_BOARD], *pComCheck = &mpi->comInfo[COM_No_CHECK];
	CCurlCtrl *mesptr = &mpi->mesInfo;
	Sleep(500);
	if(pComBoard->GetPort() < 0)
		sprintf_s((char*)mpi->sWndMsg, 256, "δ����");
	else
		sprintf_s((char*)mpi->sWndMsg, 256, "�˿� %d %d", pComBoard->portCom,  pComCheck->portCom);
	SET_MSG_INFO(mpi, mpi->ch, MSG_T_STRING);
	::SendMessage(mpi->hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)&mpi->lparam);
	Sleep(20);

	if(pComBoard->GetPort() < 0 || need_file_ifexit(mpi))
		goto out;
	// �򿪴����豸
	if(FALSE == pComBoard->InitComm(pComBoard->portCom, DEIGHT, SONE, PNONE, 115200)) {
		sprintf_s((char*)mpi->sWndMsg, WND_MSG_STR_SIZE, "��board����ʧ��");
		SET_MSG_SEND(mpi, mpi->ch, MSG_T_STRING);
	}

	if(FALSE == pComCheck->InitComm(pComCheck->portCom, DEIGHT, SONE, PNONE, 115200)) {
		sprintf_s((char*)mpi->sWndMsg, WND_MSG_STR_SIZE, "��check����ʧ��");
		SET_MSG_SEND(mpi, mpi->ch, MSG_T_STRING);
	}
	else pComCheck->SendComand(CHARG_SHUT_COM);      // �رճ��

	memset(mpi->sWndMsg, 0, WND_MSG_STR_SIZE);
	mpi->tStatus = CTime::GetCurrentTime();
	srand((unsigned int)::GetTickCount());

	//ChangeNewStatuc(mpi, RUNST_GET_BOARD, __LINE__);
	
	while(0 == mpi->doExit) {
		tNow = CTime::GetCurrentTime();
		pComBoard->Process();
		pComCheck->Process();
		if(mpi->burncheck_option_instance.g_burn_current_test){
			mpi->current = pComCheck->GetCurrent();                   //��ȡ����
			if(mpi->current > mpi->burncheck_option_instance.g_addCurrent && 
				mpi->current <= mpi->burncheck_option_instance.g_addlarge) {         // ��⵽�忨����
				if(mpi->runStatus < RUNST_GET_BOARD) {
					ChangeNewStatuc(mpi, RUNST_GET_BOARD, __LINE__);
					SET_MSG_SEND(mpi, mpi->ch, MSG_T_BTN_START);
				}
			}
		}else{
			if(mpi->runStatus < RUNST_GET_BOARD){
				ChangeNewStatuc(mpi, RUNST_GET_BOARD, __LINE__);
			}
		}
		switch(mpi->runStatus) {
			case RUNST_GET_BOARD:
				if(mpi->bEnableWriter == TRUE && mpi->bcheckStart == TRUE) {//��ⰴ���Ƿ���
					if(mpi->general_option_instance.g_check_mes  == 1){
						if(mpi->general_option_instance.g_check_mes_type == 1){
							if(mesptr->SendBurns((LPCSTR)mpi->sSetSN) == 0){//�ϴ�MES���̼��ӿ�   ��дģʽ��һ��mes�ϴ�
								mpi->Mesvalue = mesptr->Messtatus;
								int b = strlen(mesptr->line);
								strncpy_s(mpi->Mesresring,mesptr->line,strlen(mesptr->line));

								SET_MSG_SEND(mpi,mpi->ch,MSG_T_MES_STRING);//��ʾmes�����ַ�
								SET_MSG_SEND(mpi,mpi->ch,MSG_T_SMES_SEND);
							}else
								SET_MSG_SEND(mpi,mpi->ch,MSG_T_EMES_ERR);

							if(mpi->Mesvalue == 1){
								SET_MSG_SEND(mpi, mpi->ch, MSG_T_WRITEFIRM);
								ChangeNewStatuc(mpi, RUNST_START_WRITE0, __LINE__);
							}else{
								ChangeNewStatuc(mpi, RUNST_ALL_SUCC_IDLE, __LINE__);
								AfxMessageBox("mes�������ݴ��󣬲����Լ�����д�� ");
							}
						}else{
							ChangeNewStatuc(mpi, RUNST_START_WRITE0, __LINE__);//����mes������
							SET_MSG_SEND(mpi, mpi->ch, MSG_T_WRITEFIRM);						
						}
					}else{
						ChangeNewStatuc(mpi, RUNST_START_WRITE0, __LINE__);//�������MES����
						SET_MSG_SEND(mpi, mpi->ch, MSG_T_WRITEFIRM);
					}
					CreateCLPrograma(mpi, mpi->runStatus);/////��һ����д�������
				}
				break;
			case RUNST_START_WRITE0:
			case RUNST_START_WRITE1:
			case RUNST_START_WRITE2:
				pComBoard->bitStatus = 0;
			case RUNST_START_RESET:
				GetLineCLPrograma(mpi, mpi->runStatus);   // ��ⷴ������������һ����д
				break;
			case RUNST_CHECK_INIT:                   //����У����ӵ�״̬���ɰ��Ӵ�ӡ�����ݻ��   7
				bitStat = pComBoard->GetStatus();

				if(bitStat != mpi->bitStatus) {
					for(int i = 0; i < BIT_END; i++) {
						if((bitStat & (1<<i)) && (!(mpi->bitStatus & (1<<i)))) {
							mpi->bitStatusMsg = i;   //0��7
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
									AfxMessageBox("sn��δ��д�ɹ�����������д��");  
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
							break;
						}
					}
				}

				if(mpi->bitStatus >= BIT_START_DISPLY){       //��¼����ʾ�ɹ�
					if(mpi->fz_jn_flag == 1){
						if(mpi->bitStatus & (1<<BIT_NAU88L24) ){
							AfxMessageBox("�̼��Ƿ��۰�̼�����������д��");   
							ChangeNewStatuc(mpi, 100, __LINE__);
							break;
						}
					}else if(mpi->fz_jn_flag == 2){
						if(mpi->bitStatus & (1<<BIT_CS47L35)){
							AfxMessageBox("�̼��ǽ�ţ��̼�����������д��");  
							ChangeNewStatuc(mpi, 100, __LINE__);
							break;
						}
					}
					if(mpi->general_option_instance.g_check_mes  == 1){
						if(mpi->general_option_instance.g_check_mes_type == 1){
							if(mesptr->SendBurne((LPCSTR)mpi->sNoSN) == 0){            //�����ϱ��ӿ�    ��¼�ڶ���MES�ϴ�
								mpi->Mesvalue = mesptr->Messtatus;
								memcpy(mpi->Mesresring,mesptr->line,strlen(mesptr->line));
								SET_MSG_SEND(mpi,mpi->ch,MSG_T_MES_STRING); 
								SET_MSG_SEND(mpi,mpi->ch,MSG_T_EMES_SEND);
							}else
								SET_MSG_SEND(mpi,mpi->ch,MSG_T_EMES_ERR);
							if(mpi->Mesvalue == 3)
								ChangeNewStatuc(mpi, RUNST_BUREALL_SUESS, __LINE__);
							else{
								AfxMessageBox("mes�������ݴ��󣬲����Լ�����д�� ");
								ChangeNewStatuc(mpi, 100, __LINE__);
							}
						}else if(mpi->general_option_instance.g_check_mes_type == 2){
							struct ch_mes_data ch_mes_data; 
							ch_mes_data.data_flag = 1;
							mesptr->changh_mes_int(mpi->sNoSN, ch_mes_data);
							ChangeNewStatuc(mpi, RUNST_BUREALL_SUESS, __LINE__);
						}
					}else
						ChangeNewStatuc(mpi, RUNST_BUREALL_SUESS, __LINE__);
				}
				break;
			case RUNST_BUREALL_SUESS:
				SET_MSG_SEND(mpi, mpi->ch, MSG_T_ALL_SUSS);
				ChangeNewStatuc(mpi, 100, __LINE__);
				Sleep(200);
				break;
			default:
				//SET_MSG_SEND(mpi, mpi->ch, MSG_T_SHOW_CURRENT);      // ��ʾ����
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

void software_burn::AddEvent2List(CString sText)
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

void software_burn::Burndisplay(MPIPtr mpi, MsgInfo *pmsg)
{
	BOOL snOK = FALSE;
	CString sText;

	if(!mpi || !pmsg)
		return;

	switch(mpi->bitStatusMsg) {
		case BIT_SN_OK:
			m_readSn.SetWindowTextA((char*)mpi->sNoSN);    //��ʾSN�������ϴ�MES
			sText.Format("%s", (char*)mpi->sSetSN);
			if( sText.Find("-") < 0 ){
				snOK = (memcmp(mpi->sNoSN, mpi->sSetSN, 18) == 0) ? TRUE : FALSE;
			}else{
				snOK = (memcmp(mpi->sNoSN, mpi->sSetSN, 19) == 0) ? TRUE : FALSE;
			}
			if(snOK) {
				snok_green_color = 1;
				m_snRead.SetWindowTextA("OK");
			}else {
				snok_green_color = 2;
				m_snRead.SetWindowTextA("ERR");
			}
			Invalidate();	
			//UpdateWindow();
			break;

		case BIT_ADDR_OK:
			m_EditBtAddr.SetWindowTextA((char*)mpi->sBtAddr);
			break;

		case BIT_FIRMVER_OK:
			sText.Format("%s", (char*)mpi->sFirmVer);
			GetDlgItem(IDC_STATIC2)->SetWindowTextA(sText);
			break;

		default:
			break;
	}
}
LRESULT software_burn::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	UINT a;
	CString sText;
	MsgInfo *pmsg = (MsgInfo*)lParam;
	MPIPtr mpi =  (MPIPtr)&m_MPInfo;

	switch(pmsg->dwMsgType) {
		case MSG_T_STRING:
			AddEvent2List(CString((char*)mpi->sWndMsg));
			break;
		case MSG_T_BTN_START:
			AddEvent2List("��⵽�忨���ȴ���д");
			break;
		case MSG_T_WRITEFIRM:
			sText.Format("��ʼ��д....");
			AddEvent2List(sText);
			break;
		case MSG_T_PROGRESS:
			m_WrProgress.SetPos(mpi->iWriteProgress);
			if(mpi->iWriteProgress == 1000) 
				AddEvent2List("��д���");
			break;
		case MSG_T_ADDRSN:         //��ʾ���кź�����
			sText.Format("%02x:%02x:%02x:%02x:%02x:%02x",
				mpi->sBtAddr[0],mpi->sBtAddr[1],mpi->sBtAddr[2],
				mpi->sBtAddr[3],mpi->sBtAddr[4],mpi->sBtAddr[5]);
			m_EditBtAddr.SetWindowTextA(sText);
			m_readSn.SetWindowTextA((char*)mpi->sNoSN);
			AddEvent2List(sText);
			AddEvent2List((char*)mpi->sNoSN);
			break;
		case MSG_T_BITCHECK:
			Burndisplay(mpi, pmsg);
			break;
		case MSG_T_RESTART:
			AddEvent2List("ϵͳ��������...");
			break;
		case MSG_T_ALL_SUSS:
			sText.Format("------------------�忨��д���\r\n");
			AddEvent2List(sText);
			m_Epass.SetWindowTextA("PASS!");
			pass_green_color = 1;
			Invalidate();	
			//UpdateWindow();
			break;
		case MSG_T_JLINK_ERR:
			sText.Format("ϵͳjlinkû�мӵ��JLINK����");
			AddEvent2List(sText);
			AfxMessageBox(sText, MB_ICONERROR);
			break;
		case MSG_T_MES_STRING:
			sText.Format(_T("mes��������:%s"),mpi->Mesresring);
			a = sText.Find("}") + 1;
			if(a > 0)
				sText = sText.Left(a);
			else
				sText = sText;
			AddEvent2List(sText);
			break;
		case MSG_T_SMES_SEND:         //mes��д��ʼ
			sText.Format("SN��:%s  mes�жϽ��:%d", mpi->sSetSN,mpi->Mesvalue);
			AddEvent2List(sText);
			break;
		case MSG_T_EMES_ERR:
			sText.Format("mes�����Ϸ�������");
			AfxMessageBox("mes�����Ϸ�����! ");
			AddEvent2List(sText);
			break;
		default:
			break;
	}
	return 0;
}

BOOL software_burn::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString dir_path;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	if(last_sel_soft.Find("����") >= 0){
		fz_jn_flag = 1;
		dir_path = LOG_PATH_BURN_fz;
	}else if (last_sel_soft.Find("��ţ") >= 0){
		fz_jn_flag = 2;
		dir_path = LOG_PATH_BURN_jn;
	}
	
	snok_green_color = 0;
	CString soft_name;
	soft_name += last_sel_soft;
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	m_WrProgress.SetRange(0, 1000);
	m_WrProgress.SetStep(1);

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

	m_editFont.CreatePointFont(700, "����");
	m_Epass.SetFont(&m_editFont);
	//m_Epass.SetWindowTextA("pass");

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
	mpi->hThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_burn::WorkerThreadProc, mpi, 0, &idThread);
	mpi->idThread  = idThread;

	return TRUE;  // return TRUE unless you set the focus to a control
}

void software_burn::SetAllItemRestart(void)
{
	CString sText;
	m_WrProgress.SetPos(0);
	m_EditBtAddr.SetWindowTextA("");
	m_EditSn.SetWindowTextA("");
	m_readSn.SetWindowTextA("");
	m_Epass.SetWindowTextA("");

	snok_green_color = 0;
	m_snRead.SetWindowTextA("Wait");
	
	sText.Format("0.00.00", m_MPInfo.ch);
	GetDlgItem(IDC_STATIC2)->SetWindowText(sText);

	m_listEvt.ResetContent();
}
void MassProdReinit(MPIPtr mpi)        //��ʼ��
{
	if(!mpi)
		return;
	mpi->mesInfo.Messtatus = 0;
	mpi->mesInfo.log = 0;
	mpi->bcheckStart = 0;
	mpi->iWriteProgress = 0;
	mpi->outLineLen= 0;
	memset(mpi->pipeData, 0, MAX_PIPELINE_LEN);
	memset(mpi->sNoSN, 0, sizeof(mpi->sNoSN));
	memset(mpi->sFirmVer, 0, sizeof(mpi->sFirmVer));
	memset(mpi->sBtAddr, 0, sizeof(mpi->sBtAddr));
	mpi->playTimes = 0;
	mpi->audSelect = MAX_AUDIO_PLAY;
	mpi->runStatus = RUNST_NONE;
	mpi->bEnableWriter = FALSE;
	mpi->bitStatus = 0;
	mpi->bitStatusMsg = 0;
	mpi->current   = 0;
	mpi->chagrcurrent = 0;
	mpi->sleepcurrent = 0;
	for(int i = COM_No_BOARD; i < COM_No_COUNT; i++) {
		mpi->comInfo[i].bitStatus = 0;
		mpi->comInfo[i].current_mA = 0;
		memset(mpi->comInfo[i].devAddr, 0, sizeof(mpi->comInfo[i].devAddr));
		memset(mpi->comInfo[i].devSN, 0, sizeof(mpi->comInfo[i].devSN));
		memset(mpi->comInfo[i].firmVer, 0, sizeof(mpi->comInfo[i].firmVer));
	}
	return;
}
void software_burn::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_Epass.SetWindowTextA("PASS!");
	//if(pass_green_color)
	//	pass_green_color = 0;
	//else
	//	pass_green_color = 1;

	MPIPtr mpi = (MPIPtr)&m_MPInfo;
	SetAllItemRestart();
	AddEvent2List("�û���λ��д");
	m_EditSn.SetFocus();
	MassProdReinit(mpi);
}


void software_burn::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString sText;
	UpdateData(TRUE);
	m_EditSn.GetWindowTextA(sText);
	if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		if( sText.Find("-") < 0 ){
			if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("��ǰ��ƷSN��(v2)����ȷ");
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
			AfxMessageBox("��ǰ��ƷSN��(v1)����ȷ");
			m_MPInfo.bcheckStart = FALSE;
			m_MPInfo.bEnableWriter = FALSE;
		}else{
			m_MPInfo.bcheckStart = TRUE;
			m_MPInfo.bEnableWriter = TRUE;
		}
		memset((void*)m_MPInfo.sSetSN, 0, sizeof(m_MPInfo.sSetSN));
		memcpy((void*)m_MPInfo.sSetSN, sText, 19);
	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

HBRUSH software_burn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID()==IDC_STATIC9)//����Ǿ�̬�༭��
    {
		if(pass_green_color){
			pDC->SetTextColor(RGB(0,255,0));//�޸��������ɫ
			pDC->SetBkMode(TRANSPARENT);//������ı������͸����
		}else{
		    pDC->SetTextColor(RGB(255,0,0));//�޸��������ɫ
			pDC->SetBkMode(TRANSPARENT);//������ı������͸����
		}
    }
	if(pWnd->GetDlgCtrlID()==IDC_STATIC7)//����Ǿ�̬�༭��
    {
		if(snok_green_color == 1){
			pDC->SetTextColor(RGB(0,255,0));//�޸��������ɫ
			pDC->SetBkMode(TRANSPARENT);//������ı������͸����
		}else if(snok_green_color == 2){
		    pDC->SetTextColor(RGB(255,0,0));//�޸��������ɫ
			pDC->SetBkMode(TRANSPARENT);//������ı������͸����
		}
    }
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL software_burn::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}
