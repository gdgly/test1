// software_sz_burn_sn.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_sz_burn_sn.h"
#include "afxdialogex.h"


// software_sz_burn_sn �Ի���

IMPLEMENT_DYNAMIC(software_sz_burn_sn, CDialogEx)

software_sz_burn_sn::software_sz_burn_sn(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_sz_burn_sn::IDD, pParent)
{

}

software_sz_burn_sn::~software_sz_burn_sn()
{
}

void software_sz_burn_sn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, edit_out);
	DDX_Control(pDX, IDC_LIST4, m_listEvt);
	DDX_Control(pDX, IDC_STATIC11, m_Epass);
	DDX_Control(pDX, IDC_STC_SN_READ, m_sn_read_l);
	DDX_Control(pDX, IDC_STC_SN_READ2, m_sn_read_r);
	DDX_Control(pDX, IDC_STC_SN_READ3, m_sn_read_c);
	DDX_Control(pDX, IDC_EDIT2, m_EditSn_l);
	DDX_Control(pDX, IDC_EDIT15, m_EditSn_r);
	DDX_Control(pDX, IDC_EDIT3, m_EditSn_c);
}


BEGIN_MESSAGE_MAP(software_sz_burn_sn, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_sz_burn_sn::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON1, &software_sz_burn_sn::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &software_sz_burn_sn::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT15, &software_sz_burn_sn::OnEnChangeEdit15)
	ON_EN_CHANGE(IDC_EDIT3, &software_sz_burn_sn::OnEnChangeEdit3)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void software_sz_burn_sn::AddEvent2List(CString sText)
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

void software_sz_burn_sn::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	burnsn_sz_option_instance = g_p->burnsn_sz_option_instance;
}
// software_sz_burn_sn ��Ϣ�������

int software_sz_burn_sn::Parse(Com_callback_type com_cb_ins)
{
	int from, start, linelen, getend = 0;
	int i;
	//BYTE commbuf[40];
	start = from = 0;

	if(strstr((char *)bufCom, "Starting")) {
		TRACE("DEV:%s\n", bufCom);
	}

NextLine:
	for(; from < (int)bufReaded; from += 1) {
		// ��ȡһ������
		if('\n' == bufCom[from] || '\r' == bufCom[from]) {
			for(i=start; i<from; i++){
				if(bufCom[i] == '\0')
					bufCom[i] = 255;
			}
			bufCom[from] = '\0';
			getend = 1;
			break;
		}
	}
	
	if(getend) {                       //����ҵ����з�
		linelen = from-start;

		if(linelen > 2){
			//ParseLine(&bufCom[start], linelen);       //�����һ���ֽ�Ϊ/0�Ļ����ᱻ��Ϊ�ǿյ�buf
			com_cb_ins(this, &bufCom[start], linelen);       //�����һ���ֽ�Ϊ/0�Ļ����ᱻ��Ϊ�ǿյ�buf
			if( ((CButton *)GetDlgItem(IDC_CHECK6))->GetCheck() ){
				char buffer[4096];
				memset(buffer, 0, 4096);
				if(linelen >= 4096){
					memcpy(buffer, &bufCom[start], 4096);
				}else{
					memcpy(buffer, &bufCom[start], linelen);
				}
				edit_out.ReplaceSel(buffer);
				edit_out.ReplaceSel("\r\n");
			}
		}

		from += 1;
		start = from;
		getend = 0;
		goto NextLine;
	}
	
	for(; start < (int)bufReaded; start++)               //������һ�����ݺ󣬻���start����/0
		if(bufCom[start] != '\0') break;

	// �����Ƿ�Ϊһ����,��ʣ�µ����ݷŵ���ʼ��
	if(start > 0 && start <= (int)(bufReaded)) {
		bufReaded = bufReaded - start;
		memcpy(bufCom, &bufCom[start], bufReaded);
	}else if(bufReaded >= COM_BUF_SIZE)
		bufReaded = 0;

	return 0;
}

int software_sz_burn_sn::Process(Com_callback_type com_cb_ins)
{
	int rdlen, less = COM_BUF_SIZE - bufReaded;
	int hand_len = 0,offset = 0;

	if((rdlen = com_board.ReadCommBlock((LPSTR)&bufCom[bufReaded], less)) <= 0){
		if(rdlen==0)
			return 0;
	}

	// ������ȡ��������           
	if(rdlen>0){
		bufReaded += rdlen;
	}

	Parse(com_cb_ins);
	return rdlen;
}

LRESULT software_sz_burn_sn::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	switch(wParam){
		case 1:
			m_EditSn_r.SetFocus();
			break;
		case 2:
			m_EditSn_c.SetFocus();
			break;
	}
	return 0;
}

int software_sz_burn_sn::ParseLine(void *param, BYTE *buf, int len)
{
	CString sText(buf);
	software_sz_burn_sn *p = (software_sz_burn_sn*)param;
	if(sText.Find("check wrsn") >= 0){
		if(p->burn_flag == 1){
			if(sText.Find("succ") >= 0){
				p->m_sn_read_l.SetColor(STATIC_WND_SUCCESS);
				p->m_sn_read_l.SetWindowTextA("ok");
				p->left_suc_flag = 1;
			}else{
				p->m_sn_read_l.SetColor(STATIC_WND_WARNING);
				p->m_sn_read_l.SetWindowTextA("fail");
				p->pass_green_color = 0;
				p->m_Epass.SetWindowTextA("FAIL");
				p->Invalidate();	
			}
		}else if(p->burn_flag == 2){
			if(sText.Find("succ") >= 0){
				p->m_sn_read_r.SetColor(STATIC_WND_SUCCESS);
				p->m_sn_read_r.SetWindowTextA("ok");
				p->right_suc_flag = 1;
			}else{
				p->m_sn_read_r.SetColor(STATIC_WND_WARNING);
				p->m_sn_read_r.SetWindowTextA("fail");
				p->pass_green_color = 0;
				p->m_Epass.SetWindowTextA("FAIL");
				p->Invalidate();	
			}
		}else if(p->burn_flag == 3){
			if(sText.Find("succ") >= 0){
				p->m_sn_read_c.SetColor(STATIC_WND_SUCCESS);
				p->m_sn_read_c.SetWindowTextA("ok");
				p->case_suc_flag = 1;
			}else{
				p->m_sn_read_c.SetColor(STATIC_WND_WARNING);
				p->m_sn_read_c.SetWindowTextA("fail");
				p->pass_green_color = 0;
				p->m_Epass.SetWindowTextA("FAIL");
				p->Invalidate();	
			}
		}

		if( p->left_suc_flag == 1&&p->right_suc_flag == 1&&p->case_suc_flag == 1 ){
			p->pass_green_color = 1;
			p->m_Epass.SetWindowTextA("PASS");
			p->Invalidate();			
		}
		p->com_get_recv = 1;

		if(p->burn_flag == 1){
			::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 1, NULL);
		}else if(p->burn_flag == 2){
			::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 2, NULL);
		}
	}
	return 0;
}

int software_sz_burn_sn::process_com()
{
	Com_callback_type com_cb_ins = software_sz_burn_sn::ParseLine;
	CTime tNow = CTime::GetCurrentTime();
	CTime tsav = CTime::GetCurrentTime();
	const char *Commkey;
	CString sText = "";CString sTemp;
	if(burn_flag == 1){
		sText = "check wrsn left=";
		m_EditSn_l.GetWindowTextA(sTemp);
		sText += sTemp;
	}else if(burn_flag == 2){
		sText = "check wrsn right=";
		m_EditSn_r.GetWindowTextA(sTemp);
		sText += sTemp;	
	}else if(burn_flag == 3){
		sText = "check wrsn case=";
		m_EditSn_c.GetWindowTextA(sTemp);
		sText += sTemp;	
	}
	sText += "\r\n";
	Commkey = sText;
	com_board.WriteCommBlock((LPSTR)Commkey, strlen(Commkey));
	com_get_recv = 0;
	while(1){
		tNow = CTime::GetCurrentTime();
		Process(com_cb_ins);
		if(com_get_recv == 1){
			break;
		}
		if(tNow.GetTime() - tsav.GetTime() > 4){
			AddEvent2List("��ȡ���ڷ���ֵ��ʱ��");
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		}
	}
	AddEvent2List("���Խ�����");
	return 0;
}

int software_sz_burn_sn::WorkerThreadProc(void *param)
{
	software_sz_burn_sn *p = (software_sz_burn_sn*)param;
	CString sTemp;
	p->WorkerThreadProc_in_flag = 1;
	if(FALSE == p->com_board.InitComm(p->general_option_instance.g_ttycom_port, DEIGHT, SONE, PNONE, 
		p->general_option_instance.g_ttycom_baud)) {
		sTemp.Format("����%d��ʧ�ܣ�",p->general_option_instance.g_ttycom_port);
		p->AddEvent2List(sTemp);
		AfxMessageBox(sTemp);
		return -1;
	}else{
		sTemp.Format("����%d�򿪳ɹ���",p->general_option_instance.g_ttycom_port);
		p->AddEvent2List(sTemp);
		p->AddEvent2List("���Կ�ʼ��");
		p->bufReaded = 0;
	}
	p->process_com();
	p->com_board.EndComm();
	p->WorkerThreadProc_in_flag = 0;
	return 0;
}

void software_sz_burn_sn::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetAllItemRestart();
}


void software_sz_burn_sn::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString sText;
	CString stmp;
	m_EditSn_l.GetWindowTextA(sText);
	if(sText.GetLength() == burnsn_sz_option_instance.g_burn_sn_len){//v3
		if(sText.Find(burnsn_sz_option_instance.g_l_sn_prompt) >= 0){
			if(WorkerThreadProc_in_flag == 0){
				burn_flag = 1;
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_sz_burn_sn::WorkerThreadProc, this, 0, NULL);
				stmp = "sn������ɹ�" + sText;
				AddEvent2List(stmp);
			}else{
				AddEvent2List("��д������δ�˳������Ժ�");
			}
		}else{
			AddEvent2List("���sn�����ò�����");
			AfxMessageBox("���sn�����ò�����");
		}
	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void software_sz_burn_sn::OnEnChangeEdit15()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString sText;
	CString stmp;
	m_EditSn_r.GetWindowTextA(sText);
	if(sText.GetLength() == burnsn_sz_option_instance.g_burn_sn_len){//v3
		if(sText.Find(burnsn_sz_option_instance.g_r_sn_prompt) >= 0){
			if(WorkerThreadProc_in_flag == 0){
				burn_flag = 2;
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_sz_burn_sn::WorkerThreadProc, this, 0, NULL);
				stmp = "sn������ɹ�" + sText;
				AddEvent2List(stmp);
			}else{
				AddEvent2List("��д������δ�˳������Ժ�");
			}
		}else{
			AddEvent2List("�Ҷ�sn�����ò�����");
			AfxMessageBox("�Ҷ�sn�����ò�����");
		}
	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void software_sz_burn_sn::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString sText;
	CString stmp;
	m_EditSn_c.GetWindowTextA(sText);
	if(sText.GetLength() == burnsn_sz_option_instance.g_burn_sn_len){//v3
		if(sText.Find(burnsn_sz_option_instance.g_c_sn_prompt) >= 0){
			if(WorkerThreadProc_in_flag == 0){
				burn_flag = 3;
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_sz_burn_sn::WorkerThreadProc, this, 0, NULL);
				stmp = "sn������ɹ�" + sText;
				AddEvent2List(stmp);
			}else{
				AddEvent2List("��д������δ�˳������Ժ�");
			}
		}else{
			AddEvent2List("����sn�����ò�����");
			AfxMessageBox("����sn�����ò�����");
		}
	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


HBRUSH software_sz_burn_sn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID()==IDC_STATIC11)//����Ǿ�̬�༭��
    {
		if(pass_green_color){
			pDC->SetTextColor(RGB(0,255,0));//�޸��������ɫ
			pDC->SetBkMode(TRANSPARENT);//������ı������͸����
		}else{
		    pDC->SetTextColor(RGB(255,0,0));//�޸��������ɫ
			pDC->SetBkMode(TRANSPARENT);//������ı������͸����
		}
    }
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void software_sz_burn_sn::SetAllItemRestart(void)
{
	CString sText;
	m_EditSn_l.SetWindowTextA("");
	m_EditSn_r.SetWindowTextA("");
	m_EditSn_c.SetWindowTextA("");
	m_Epass.SetWindowTextA("");
	pass_green_color = 0;

	WorkerThreadProc_in_flag = 0;

	burn_flag = 0;
	left_suc_flag = 0;
	right_suc_flag = 0;
	case_suc_flag = 0;

	m_sn_read_l.SetColor(STATIC_WND_DISABLE);
	m_sn_read_r.SetColor(STATIC_WND_DISABLE);
	m_sn_read_c.SetColor(STATIC_WND_DISABLE);
	m_sn_read_l.SetWindowTextA("wait");
	m_sn_read_r.SetWindowTextA("wait");
	m_sn_read_c.SetWindowTextA("wait");

	edit_out.SetWindowTextA("");

	m_listEvt.ResetContent();
	m_EditSn_l.SetFocus();
	AddEvent2List("#####��򿪺и�#####");
	AddEvent2List("#####�ȴ��û�����sn��#####");
}

BOOL software_sz_burn_sn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	CString dir_path;
	dir_path = LOG_PATH_BURNSN_sz;

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

	m_editFont.CreatePointFont(700, "����");
	m_Epass.SetFont(&m_editFont);

	SetAllItemRestart();
	return TRUE;  // return TRUE unless you set the focus to a control
}


BOOL software_sz_burn_sn::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}
