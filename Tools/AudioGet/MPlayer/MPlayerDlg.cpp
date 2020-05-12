
// MPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MPlayer.h"
#include "MPlayerDlg.h"
#include "afxdialogex.h"
#include "G722Play.h"
#include "acodec.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SEND_USER_MSG(wnd,m,w,l)    ::SendMessage((wnd), (m), (w), (l));

PlayTime gPlayTimeInfo = {0};
int g_toal_time;
int current_time = 0;
// CMPlayerDlg 对话框
int old_offset;

WavHeader gWavHead = {
	{0x52, 0x49, 0x46, 0x46},
	0,  /* other len */
	{0x57, 0x41, 0x56, 0x45},
	{0x66, 0x6D, 0x74, 0x20},
	0x10, 0x01, 0x01,
	16000, 16000*2,
	0x02, 0x10,
	{0x64, 0x61, 0x74, 0x61},
	0,  /* data len */
};
CMPlayerDlg::CMPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMPlayerDlg::IDD, pParent)
	, m_iChNum(4)
	, m_TrunkSize(368640)
{
	m_hIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ICON));
	hour=0;
	minute=0;
	second=0;
	cs=0;
	ch=0;
	cm=0;
	mode=SINGLE;
}

void CMPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	/*	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, list);
	DDX_Control(pDX, IDC_SLIDER_VOL, vol);
	DDX_Control(pDX, IDC_EDIT_CONVER, m_Econver);
	DDX_Control(pDX, IDC_FILENAME, m_Efllename);*/
	DDX_Control(pDX, IDC_LISTBOX, m_ListFile);
	DDX_Control(pDX, IDC_BUTTON_CON, button);
	DDX_Text(pDX, IDC_EDIT_CH_NUM, m_iChNum);
	DDX_Text(pDX, IDC_EDIT_TRUNK_SIZE, m_TrunkSize);
}

BEGIN_MESSAGE_MAP(CMPlayerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_LBN_SELCHANGE(IDC_LIST, &CMPlayerDlg::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_PLAY, &CMPlayerDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_ADD, &CMPlayerDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_PAUSE, &CMPlayerDlg::OnBnClickedPause)
	//ON_BN_CLICKED(IDC_STOP, &CMPlayerDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_DELETE, &CMPlayerDlg::OnBnClickedDelete)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_VOL, &CMPlayerDlg::OnCustomdrawSliderVol)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_TIME, &CMPlayerDlg::OnCustomdrawSliderTime)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &CMPlayerDlg::OnMSGMassProduct)
	ON_LBN_DBLCLK(IDC_LIST, &CMPlayerDlg::OnDblclkList)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_CON, &CMPlayerDlg::OnBnClickedButtonCon)
	ON_BN_CLICKED(IDC_BUTTON_ORDPLAY, &CMPlayerDlg::OnBnClickedButtonOrdplay)
	ON_BN_CLICKED(IDC_BUTTON_STOPPLAY, &CMPlayerDlg::OnBnClickedButtonStopplay)
	ON_LBN_SELCHANGE(IDC_PAUSE, &CMPlayerDlg::OnLbnSelchangePause)
END_MESSAGE_MAP()


// CMPlayerDlg 消息处理程序

BOOL CMPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//process.SetRange(0,100);                   // 声音显示滑块调试
	
	CString sCmdline = AfxGetApp()->m_lpCmdLine;
	if(sCmdline.IsEmpty() == false){      
		CString  sect = sCmdline.Mid(1);
		sect = sect.Left(sect.GetLength() - 1);
		list.InsertString(list.GetCount(),sect);
		list.SetCurSel(list.GetCount()-1);
	}

	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CMPlayerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBitmap bitmap;
	bitmap.LoadBitmapA(MAKEINTRESOURCE(IDB_BG));
	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);
	dcCompatible.SelectObject(&bitmap);
	CRect rc;
	GetClientRect(&rc);
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);
	pDC->StretchBlt(0,0,rc.Width(),rc.Height(),&dcCompatible,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	return TRUE;
}


void CMPlayerDlg::OnLbnSelchangeList()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMPlayerDlg::OnBnClickedPlay()                                           //播放
{
	
	CString temp;
	PlayStatusPtr playptr = &gPlayInfo;
	PlayTimePtr timeptr = &gPlayTimeInfo;
	GetDlgItemText(IDC_PAUSE,temp);           //将文本拷到temp中
	if(temp.Compare("继续播放")==0)
	{
		SetDlgItemText(IDC_PAUSE,"暂停");
    }
	CString filename;
	int index=list.GetCurSel();                //list 变量
	if(index==-1)
	{
		MessageBox("_(:3 _ 2 没有音乐或者没选中...请添加或选择");
		return;
	}
		
	list.GetText(index,filename);             //获取文件名
	SetDlgItemText(IDC_FILENAME,filename);    // 

	CFile m_file;
	if(!m_file.Open(filename,CFile::modeRead ))           //怎么获取的文件路径
	{
		AfxMessageBox(_T("无法打开文件"));
		//timeptr->path.Empty();
		return;
	}
	m_file.Close();
	playptr->stop = 0;          
	m_G722Play.PlayFile(filename);         //播放文件
}

void CMPlayerDlg::getFiles(const std::string & path, std::vector<std::string> & files)
{
    //文件句柄
    long hFile = 0;
    //文件信息，_finddata_t需要io.h头文件
    struct _finddata_t fileinfo;
    std::string p;
	this->con=0;
	m_ListFile.ResetContent();
	button.SetWindowTextA("点击pcm转化为四通道");
    if((hFile = _findfirst(p.assign(path).append("\\*.pcm").c_str(),&fileinfo)) != -1)
    {
        do
        {
            //如果是目录,迭代之
            //如果不是,加入列表
            if((fileinfo.attrib & _A_SUBDIR))
            {
                if(strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				
				this->pushFile[this->con] = fileinfo.name;
				this->push[this->con] = this->szDisplayName +"\\"+ fileinfo.name;
				this->Preservation[this->con] = this->szDisplayName +"\\Conversion\\"+ fileinfo.name;
				CString sDir = this->Preservation[this->con] = this->szDisplayName +"\\Conversion\\";
				::CreateDirectoryA(sDir, 0);
				this->con++;
				m_ListFile.AddString(fileinfo.name);
				TRACE(traceAppMsg, 0,"%d %s\r\n",this->con-1,this->push[this->con-1]);
            }
			
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}

void CMPlayerDlg::OnBnClickedAdd()                                //添加文件
{
	std::vector<std::string> filenames;
	std::string path;
	char buf[256]={0};
	BROWSEINFO  bi;
	 bi.hwndOwner=NULL;
	 bi.pidlRoot=NULL;
	 bi.pszDisplayName=NULL;
	 bi.lpszTitle=NULL;
	 bi.ulFlags=0;
	 bi.lpfn =NULL;
	 bi.iImage =0;
	 LPCITEMIDLIST pidl=SHBrowseForFolder(&bi);
	 if(!pidl)
	  return;
	SHGetPathFromIDList(pidl,buf);
	this->szDisplayName = buf;
	if(buf != 0)
    this->getFiles(buf, filenames);
}


void CMPlayerDlg::OnBnClickedPause()                //暂停播放
{
	CString temp;
	PlayStatusPtr playptr = &gPlayInfo;
	GetDlgItemText(IDC_PAUSE,temp);
	if(temp.Compare("暂停")==0)
	{
		//player.Pause();
		playptr->pause = 1;
		playptr->con_play = 0;
		SetDlgItemText(IDC_PAUSE,"继续播放");
		KillTimer(0);
	}
	else if(temp.Compare("继续播放")==0)
	{
		//player.resume();
		playptr->con_play = 1;
		playptr->pause = 2;
		SetDlgItemText(IDC_PAUSE,"暂停");
		SetTimer(0,1000,NULL);
	}

	// TODO: 在此添加控件通知处理程序代码
}


void CMPlayerDlg::OnBnClickedStop()                //停止播放
{
	//KillTimer(0);
	//CString temp;
	PlayStatusPtr playptr = &gPlayInfo;
	playptr->stop = 1;
	CString temp;
	temp.Format("音乐播放时间: 0:0:0 / 0:0:0");
	SetDlgItemText(IDC_STATIC_TIME,temp);
	KillTimer(0);
	KillTimer(1);

	// TODO: 在此添加控件通知处理程序代码
}

void CMPlayerDlg::OnBnClickedDelete()                    //删除文件
{
	list.DeleteString(list.GetCurSel());
	// TODO: 在此添加控件通知处理程序代码
}


void CMPlayerDlg::OnCustomdrawSliderVol(NMHDR *pNMHDR, LRESULT *pResult)          //设置音量
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	volume=vol.GetPos();
	player.SetVolume(volume);
	CString Vtemp;
	Vtemp.Format("音量大小: %d%%",volume/10);
	SetDlgItemText(IDC_STATIC_VOL,Vtemp);
	*pResult = 0;
}


void CMPlayerDlg::OnCustomdrawSliderTime(NMHDR *pNMHDR, LRESULT *pResult)   ////////
{
	
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CMPlayerDlg::OnDblclkList()
{
	OnBnClickedPlay();
}

#define MAX_RDBUF_SIZE    (4*1024*1024)    //  申请大内存
#define MAX_WRBUF_SIZE    (4*1024*1024)

int CMPlayerDlg::FindTags(BYTE *pbuffer, UINT count)
{
	int offset = -1;

	for(int i = 0; i < (int)(count-8); i++) {
		if(pbuffer[i]==0x11 && pbuffer[i+1]==0xAA && pbuffer[i+2]==0x22 && pbuffer[i+3]==0xBB
			&&pbuffer[i+4]==0x33 && pbuffer[i+5]==0xCC && pbuffer[i+6]==0x44 && pbuffer[i+7]==0xDD) {
			offset = i;
			break;
		}
	}

	return offset;
}

int CMPlayerDlg::FindTags(CFile &hFile, UINT iFrom)
{
	int rdsize, offset = -1, tmp;

	while(1) {
		hFile.Seek(iFrom, SEEK_SET);
		rdsize = hFile.Read(m_pRdBuffer, MAX_RDBUF_SIZE);

		if(rdsize <= 0)
			break;

		if((tmp=FindTags(m_pRdBuffer, rdsize)) >= 0) {
			offset = iFrom + tmp;
			break;
		}

		if(offset >= 0) break;

		iFrom += rdsize;
	}


	return offset;
}

int CMPlayerDlg::SaveOneTags(CFile &hFile, UINT iFrom, CString outPrefix, int sector)
{
	int i, j, ret = 0, count, trunk = m_iChNum * 2;
	int geted = 0, end_sect = 0, outsize = 0, secStart = iFrom;
	CString sOut[6];
	CString sText;
	CFile hOut[6];
	
	for(i = 0; i < (int)m_iChNum; i++) {
		CString tmp;
		tmp.Format("_%d_A%d.pcm", sector, i);
		sOut[i] = outPrefix + tmp;
		if(hOut[i].Open(sOut[i], CFile::modeWrite | CFile::typeBinary | CFile::modeCreate) != TRUE) {
			TRACE("open wav file success\n");
			break;
		}
	}
	if(i != m_iChNum) {
		AfxMessageBox("打开输出文件失败");
		ret = -3;
		goto out;
	}

	iFrom += 8;    // skip 0x11aa22bb33cc44dd
	
	while(1) {
		hFile.Seek(iFrom, SEEK_SET);

		int readed = hFile.Read(m_pRdBuffer, MAX_RDBUF_SIZE);
		if(readed <= 8) {
			end_sect = 2;
			break;
		}

		count = FindTags(m_pRdBuffer, readed);
		if(count < 0) count = readed;
		else {
			end_sect = 1;

			if(m_iSectSize && count != m_iSectSize) {
				sText.Format("Error tagsize=%d offset=0x%x--0x%x skip it\n", count, secStart, iFrom+count);
				TRACE(sText);
				AfxMessageBox(sText);
				iFrom += count;
				break;
			}
		}

		count = (count / trunk) * trunk;
		geted = 0;
		for(i = 0; i < count; i+=trunk) {
			for(j = 0; j < (int)m_iChNum; j++) {
				m_pWrBuffer[j][geted]   = m_pRdBuffer[i+j*2];
				m_pWrBuffer[j][geted+1] = m_pRdBuffer[i+j*2+1];				
			}
			geted += 2;
		}
		
//		TRACE("Geted=%d count=%d from=%d\n", geted, count, iFrom);
		outsize += geted;
		for(i = 0; i < (int)m_iChNum; i++) {
//		    TRACE("Write Wrbuf[%d]=%p\n", i, m_pWrBuffer[i]);
			hOut[i].Write(m_pWrBuffer[i], geted);
		}

		iFrom += count;

		if(2 == end_sect)
			break;
		else if(1 == end_sect){
			TRACE("End sect:%d sectsize=%d(%d) (readed=%d,cnt=%d,geted=%d) nextoffset=%x\n", end_sect, outsize, geted, readed, count, geted, iFrom);
	//		if(m_iSectSize != outsize) {
	//			CString sText;
	//			sText.Format("Error size=%d offset=0x%x--0x%x\n", outsize, secStart, iFrom);
	//			AfxMessageBox(sText);
	//		}
			iFrom += 8;          // next tag
		}
	}
	ret = iFrom;

out:
	for(i = 0; i < (int)m_iChNum; i++) {
		if(hOut[i].m_hFile != INVALID_HANDLE_VALUE) hOut[i].Close();
	}

	return ret;
}


int CMPlayerDlg::ConvertFile(CString sDir, CString inFile)
{
	int ret=0, index, offset = -1, sector = 0;
	CString sIn,sOutPrefix;
	CString sText;
	CFile hIn;

	index = inFile.ReverseFind('.');
	sIn = sDir + "\\" + inFile;
	sOutPrefix = sDir + "\\Conversion\\" + inFile.Left(index);

	TRACE("%s ==> %s\n", sIn, sOutPrefix);

	if(hIn.Open(sIn, CFile::modeRead | CFile::typeBinary, NULL, NULL) != TRUE) {
		sText.Format("打开原文件失败:%s", sIn);
		AfxMessageBox(sText);
		return -1;
	}
	if((offset=FindTags(hIn, 0)) < 0) {
		sText.Format("没找到文件标记\n %s", sIn);
		AfxMessageBox(sText);
		return -2;
	}

	while(1) {
		int tmp = SaveOneTags(hIn, offset, sOutPrefix, sector);
		if(tmp < 0) {
			break;
		}

		offset = tmp;
		hIn.Seek(offset, SEEK_SET);
		if(hIn.Read(m_pRdBuffer, 64) < 64 || FindTags(m_pRdBuffer, 64) < 0) {
			TRACE("Endoffile:%s\n", sIn);
			break;
		}

		sector += 1;
	}

//	for(int i = 0; i < (int)m_iChNum; i++) {
//		TRACE("EndFile Wrbuf[%d]=%p\n", i, m_pWrBuffer[i]);
//	}

	ret = 0;

	if(hIn.m_hFile != INVALID_HANDLE_VALUE) hIn.Close();

	return ret;
}

void CMPlayerDlg::OnBnClickedButtonCon()
{
	CString out_filename;

	m_pRdBuffer = NULL;
	UpdateData(TRUE);

	if(this->con==0) {
		AfxMessageBox("请选择需要转换的文件");
		return;
	}

	if(m_iChNum < 1 || m_iChNum > 6) {
		AfxMessageBox("通道数为1...6");
		return;
	}

	m_iSectSize = m_TrunkSize * m_iChNum;
	m_pRdBuffer = new BYTE [MAX_RDBUF_SIZE];
	for(int i = 0; i < 6; i++) {
		m_pWrBuffer[i] = new BYTE[MAX_WRBUF_SIZE];
//		TRACE("m_pWrbuf[%d]=%p\n", i, m_pWrBuffer[i]);
	}

	button.SetWindowTextA("正在转化....");

	for(int i=0;i<this->con;i++)
	{
#if 10
		if(ConvertFile(this->szDisplayName, this->pushFile[i]) < 0) {
			CString sText;
			sText.Format("Fail Conv:%s", pushFile[i]);
			AfxMessageBox(sText);
			break;
		}

#else
		int value = this->Preservation[i].Find('.');
		CString sect = this->Preservation[i].Left(value);
		out_filename= sect + ".A1pcm";
		if(wavFile1.Open( out_filename, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate) == TRUE)
		{
			TRACE("open wav file success\n");
		}
		out_filename= sect + ".A2pcm";
		if(wavFile2.Open( out_filename, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate) == TRUE)
		{
			TRACE("open wav file success\n");
		}
		out_filename= sect + ".A3pcm";
		if(wavFile3.Open( out_filename, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate) == TRUE)
		{
			TRACE("open wav file success\n");
		}
		out_filename= sect + ".A4pcm";
		if(wavFile4.Open( out_filename, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate) == TRUE)
		{
			TRACE("open wav file success\n");
		}
		out_filename= sect + ".Apcm";
		if(wavFile5.Open( out_filename, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate) == TRUE)
		{
			TRACE("open wav file success\n");
		}
		if(g722File.Open(this->push[i] , CFile::modeRead | CFile::typeBinary, NULL, NULL) == TRUE)
		{
			wavFile1.Seek(0,CFile::begin);
			wavFile2.Seek(0,CFile::begin);
			wavFile3.Seek(0,CFile::begin);
			wavFile4.Seek(0,CFile::begin);
			wavFile5.Seek(0,CFile::begin);
			int len=16*1024,i=0;
			while(1){
				int rdLen1 = g722File.Read(m_holdbuf, len);
				if(rdLen1 < 8){
					wavFile1.Close();
					wavFile2.Close();
					wavFile3.Close();
					wavFile4.Close();
					wavFile5.Close();
					g722File.Close();
					button.SetWindowTextA("没找到头");
					return;
				}
				for(i=0;i<(16*1024-8);i++)
				{
					if(m_holdbuf[i]==0x11&&m_holdbuf[i+1]==0xAA&&m_holdbuf[i+2]==0x22&&m_holdbuf[i+3]==0xBB&&m_holdbuf[i+4]==0x33&&
							m_holdbuf[i+5]==0xCC&&m_holdbuf[i+6]==0x44&&m_holdbuf[i+7]==0xDD)
					{
						break;
					}
				}
				if(i==(16*1024-8))
				{
					wavFile1.Close();
					wavFile2.Close();
					wavFile3.Close();
					wavFile4.Close();
					wavFile5.Close();
					g722File.Close();
					button.SetWindowTextA("没找到头");
				}else
				{
					g722File.Seek(i+8,CFile::begin);
					TRACE("%d\r\n",i);
					break;
				}
		
			}
			while(1)
			{
				int rdLen = g722File.Read(m_holdbuf, 16*1024);
				if(rdLen < 8)
					break;
				TRACE("%x,%x,%x,%x,%d\r\n",m_holdbuf[0],m_holdbuf[1],m_holdbuf[2],m_holdbuf[3],rdLen);
				for(int j=0;j<rdLen/8;j++)
				{
					wavFile1buf[2*j]=m_holdbuf[j*8+0];
					wavFile1buf[2*j+1]=m_holdbuf[j*8+1];
					wavFile2buf[2*j]=m_holdbuf[j*8+2];
					wavFile2buf[2*j+1]=m_holdbuf[j*8+3];
					wavFile3buf[2*j]=m_holdbuf[j*8+4];
					wavFile3buf[2*j+1]=m_holdbuf[j*8+5];
					wavFile4buf[2*j]=m_holdbuf[j*8+6];
					wavFile4buf[2*j+1]=m_holdbuf[j*8+7];
					if(j==2)
					TRACE("%x,%x,%x,%x\r\n",wavFile2buf[0],wavFile2buf[1],wavFile2buf[2],wavFile2buf[3]);
					

				}
				TRACE("%x,%x,%x,%x\r\n",wavFile2buf[0],wavFile2buf[1],wavFile2buf[2],wavFile2buf[3]);
				wavFile1.Write(wavFile1buf,rdLen/4);
				wavFile2.Write(wavFile2buf,rdLen/4);
				wavFile3.Write(wavFile3buf,rdLen/4);
				wavFile4.Write(wavFile4buf,rdLen/4);
				wavFile5.Write(&m_holdbuf[0],rdLen);
			}
			wavFile1.Close();
			wavFile2.Close();
			wavFile3.Close();
			wavFile4.Close();
			wavFile5.Close();
			g722File.Close();
		}
	#endif
	}
	delete m_pRdBuffer, m_pRdBuffer = NULL;
	for(int i = 0; i < 6; i++) {
//		TRACE("freem_pWrbuf[%d]=%p\n", i, m_pWrBuffer[i]);
		delete m_pWrBuffer[i], m_pWrBuffer[i] = NULL;;
	}

	button.SetWindowTextA("转化成功");

}


INT CMPlayerDlg::Write_Wav(CString filename)
{
	CString out_filename;
	unsigned int datalen = 0,sample_rate = 16000;
	unsigned short outSize; 
	int value = filename.Find('.');
	CString sect = filename.Left(value);
	out_filename = sect + ".wav";
	if(wavFile.Open( out_filename, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate) == TRUE)
	{
		TRACE("open wav file success\n");
	}
	
	if(g722File.Open(filename , CFile::modeRead | CFile::typeBinary, NULL, NULL) == TRUE)
	{
		m_Econver.SetWindowTextA("正在转化....");
		wavFile.Seek(44,CFile::begin);
		while(1)
		{
			int rdLen = g722File.Read(m_g722Buf, AUDIO_ENCODE_PACKET_SIZE);
			int ret = acodec_decoder(m_g722Buf, AUDIO_ENCODE_PACKET_SIZE, m_wavBuf, &outSize);
			wavFile.Write(m_wavBuf,outSize);
			datalen = datalen + outSize;
			if(rdLen < AUDIO_ENCODE_PACKET_SIZE)
				break;
		}
		Write_WavHead(datalen,sample_rate);
		wavFile.Close();
		g722File.Close();
		m_Econver.SetWindowTextA("转化成功");
	}

	return 0;
}


INT CMPlayerDlg::Write_WavHead(int datalen, int sample_rate)
{
	gWavHead.dataSize = datalen;
	gWavHead.riffSize = datalen + 36;

	gWavHead.sampleRate = sample_rate;
	gWavHead.bytesPerSecond = gWavHead.sampleRate * 2;    // 16 BIT

	wavFile.Seek(0,CFile::begin);
	wavFile.Write(&gWavHead,sizeof(gWavHead));

	TRACE("Create WavHead SampleRate:%d datLen=%d\n", sample_rate, datalen);

	return 0;
}

void CMPlayerDlg::OnBnClickedButtonOrdplay()                    //播放第一个文件
{
	// TODO: 在此添加控件通知处理程序代码
	PlayStatusPtr playptr = &gPlayInfo;
	//PlayTimePtr timeptr = &gPlayTimeInfo;
	m_ncount = list.GetCount();
	m_listncount = 0;
	list.GetText(m_listncount,filename);                         //获取文件名
	CFile m_file;
	if(!m_file.Open(filename,CFile::modeRead )){                 //怎么获取的文件路径
		AfxMessageBox(_T("无法打开文件"));
		return;
	}
	m_file.Close();
	m_G722Play.SetWnd(this->m_hWnd);
	m_listncount ++;
	playptr->stop = 0;
	Play_Ord(filename);
}

void CMPlayerDlg::NextPplay()              //第一个文件以后播放
{
	PlayStatusPtr playptr = &gPlayInfo;
	if(m_listncount >= m_ncount){
		AfxMessageBox(_T("所有文件已播放完毕！"));
		return;
	}
	list.GetText(m_listncount,filename);                         //获取文件名
	CFile m_file;
	if(!m_file.Open(filename,CFile::modeRead )){                 //怎么获取的文件路径
		AfxMessageBox(_T("无法打开文件"));
		return;
	}
	m_file.Close();
	m_listncount ++;
	Sleep(1000);
	playptr->stop = 0;
	Play_Ord(filename);
}
void CMPlayerDlg::Play_Ord(CString filename) 
{
	m_G722Play.PlayFile(filename);                      //播放文件
}

LRESULT CMPlayerDlg::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	CString sText;
	switch((INT)wParam) {
	case WPARAM_GETFILE:
		sText.Format(filename);
		m_Efllename.SetWindowTextA(sText);
		break;

	case WPARAM_THREAD_EXITED:
		NextPplay();
		break;
	}
	return 0;
}

void CMPlayerDlg::OnBnClickedButtonStopplay()
{
	// TODO: 在此添加控件通知处理程序代码

	PlayStatusPtr playptr = &gPlayInfo;
	playptr->stop = 1;
}


void CMPlayerDlg::OnLbnSelchangePause()
{
	// TODO: 在此添加控件通知处理程序代码
}
