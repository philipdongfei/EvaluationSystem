
// EvaluationSystemDlg.cpp : ʵ���ļ�
//




#include "stdafx.h"
#include "EvaluationSystem.h"
#include "EvaluationSystemDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <codecvt>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "Markup.h"

#include ".\\alglib\\alglibinternal.h"
#include ".\\alglib\\alglibmisc.h"
#include ".\\alglib\\linalg.h"
#include ".\\alglib\\statistics.h"
#include ".\\alglib\\dataanalysis.h"
#include ".\\alglib\\specialfunctions.h"
#include ".\\alglib\\solvers.h"
#include ".\\alglib\\optimization.h"
#include ".\\alglib\\diffequations.h"
#include ".\\alglib\\fasttransforms.h"
#include ".\\alglib\\integration.h"
#include ".\\alglib\\interpolation.h"

using namespace alglib;

#ifndef OS_LINUX
#include <Windows.h>
#pragma comment(lib, "ICTCLAS50.lib") //ICTCLAS50.lib����뵽������
#endif
#include "ICTCLAS50.h"

std::wstring s2ws(const std::string& str);
std::string ws2s(const std::wstring& wstr);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_FINISH_THREAD       (WM_USER+100) 
bool bExitThread = false;

UINT   ThreadBulk(LPVOID para)
{
	CEvaluationSystemDlg *pDlg = (CEvaluationSystemDlg*)para;
	if (pDlg->ProcessBulk())
		PostMessageA(pDlg->GetSafeHwnd(),WM_FINISH_THREAD,1,0);
	return 0;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEvaluationSystemDlg �Ի���




CEvaluationSystemDlg::CEvaluationSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEvaluationSystemDlg::IDD, pParent)
	, m_strScore(_T(""))
	, m_bSegment(TRUE)
	, m_strScoreOR1(_T(""))
	, m_strScoreSR1(_T(""))
	, m_strScoreOR2(_T(""))
	, m_strScoreSR2(_T(""))
	, m_strScoreOR3(_T(""))
	, m_strScoreSR3(_T(""))
	, m_strScoreORL(_T(""))
	, m_strScoreSRL(_T(""))
	, m_strScoreORNPL(_T(""))
	, m_strScoreSRNPL(_T(""))
	, m_strScoreORW(_T(""))
	, m_strScoreSRW(_T(""))
	, m_strScoreORS(_T(""))
	, m_strScoreSRS(_T(""))
	, m_strScoreORSU(_T(""))
	, m_strScoreSRSU(_T(""))
	, m_strScoreORC2(_T(""))
	, m_strScoreSRC2(_T(""))
	, m_strScoreORC3(_T(""))
	, m_strScoreSRC3(_T(""))
	, m_str2gramO(_T(""))
	, m_str2gramS(_T(""))
	, m_str3gramO(_T(""))
	, m_str3gramS(_T(""))
	, m_str4gramO(_T(""))
	, m_str4gramS(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEvaluationSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_CANDIDATE, m_credtCandidate);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbType);
	DDX_Control(pDX, IDC_STATIC_SCORCE, m_stcScore);
	DDX_Text(pDX, IDC_STATIC_SCORCE, m_strScore);
	DDX_Control(pDX, IDC_RICHEDIT_REFERENCE, m_credtReference);
	DDX_Check(pDX, IDC_CHECK_SEGMENTATION, m_bSegment);
	DDX_Text(pDX, IDC_SC_OR1, m_strScoreOR1);
	DDX_Text(pDX, IDC_SC_SR1, m_strScoreSR1);
	DDX_Text(pDX, IDC_SC_OR2, m_strScoreOR2);
	DDX_Text(pDX, IDC_SC_SR2, m_strScoreSR2);
	DDX_Text(pDX, IDC_SC_OR3, m_strScoreOR3);
	DDX_Text(pDX, IDC_SC_SR3, m_strScoreSR3);
	DDX_Text(pDX, IDC_SC_ORL, m_strScoreORL);
	DDX_Text(pDX, IDC_SC_SRL, m_strScoreSRL);
	DDX_Text(pDX, IDC_SC_ORNPL, m_strScoreORNPL);
	DDX_Text(pDX, IDC_SC_SRNPL, m_strScoreSRNPL);
	DDX_Text(pDX, IDC_SC_ORW, m_strScoreORW);
	DDX_Text(pDX, IDC_SC_SRW, m_strScoreSRW);
	DDX_Text(pDX, IDC_SC_ORS, m_strScoreORS);
	DDX_Text(pDX, IDC_SC_SRS, m_strScoreSRS);
	DDX_Text(pDX, IDC_SC_ORSU, m_strScoreORSU);
	DDX_Text(pDX, IDC_SC_SRSU, m_strScoreSRSU);
}

BEGIN_MESSAGE_MAP(CEvaluationSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CEvaluationSystemDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_SEGMENTATION, &CEvaluationSystemDlg::OnBnClickedCheckSegmentation)
	ON_BN_CLICKED(ID_BTN_BULK, &CEvaluationSystemDlg::OnBnClickedBtnBulk)
	ON_BN_CLICKED(IDCANCEL, &CEvaluationSystemDlg::OnBnClickedCancel)
	ON_MESSAGE( WM_FINISH_THREAD,OnFinishThread)
	ON_BN_CLICKED(IDC_BTN_CORRELATION, &CEvaluationSystemDlg::OnBnClickedBtnCorrelation)
END_MESSAGE_MAP()


// CEvaluationSystemDlg ��Ϣ�������

BOOL CEvaluationSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if (LoadLibraryA("RICHED20.DLL") == NULL)
	{
		AfxMessageBox(_T("Fail to load ""riched20.dll"" ."),MB_OK | MB_ICONERROR);
		PostMessage(WM_QUIT,0,0);
		return FALSE;
	}

	m_strScore = (_T("Score:"));
	m_cmbType.AddString("ROUGE-1");//0
	m_cmbType.AddString("ROUGE-2");//1
	m_cmbType.AddString("ROUGE-3");//2
	m_cmbType.AddString("ROUGE-L");//3
	m_cmbType.AddString("Normalized Pariwise LCS");//4
	m_cmbType.AddString("ROUGE-W");//5
	m_cmbType.AddString("ROUGE-S");//6
	m_cmbType.SetCurSel(0);

	for (int nI=0; nI < 8; nI++)
	{
		m_dbRaw[nI] = 0.0;
		m_dbSeg[nI] = 0.0;
	}
	m_hThread = NULL;
	UpdateData(0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEvaluationSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEvaluationSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CEvaluationSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEvaluationSystemDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_credtCandidate.GetTextLength() == 0)
		return;
	if (m_credtReference.GetTextLength() == 0)
		return;
	double dbScore(0.0);
	int nType = m_cmbType.GetCurSel();
	/*char *pBuffer = new char[1024];
	int nLineCount = m_credtCandidate.GetLineCount();
	//m_credtCandidate.GetWindowTextA(pBuffer,1024);
	m_wstrCandidate.clear();
	m_wstrReference.clear();
	m_vecReference.clear();
	int nIndex;
	for(nIndex = 0; nIndex < nLineCount;nIndex++)
	{
		memset(pBuffer,0,1024);

		int nLen = m_credtCandidate.GetLine(nIndex,pBuffer,1024);
		int pp = pBuffer[nLen-1];
		if (nLen > 0)
		{
			if (m_bSegment)
			{
				if (!SegmentSentence(m_wstrCandidate,pBuffer))
					return;
			}
			else {
				m_wstrCandidate += ByteToWString(pBuffer);
			}
		}
	}
	 nLineCount = m_credtReference.GetLineCount();
	 for(nIndex = 0; nIndex < nLineCount;nIndex++)
	{
		memset(pBuffer,0,1024);

		int nLen = m_credtReference.GetLine(nIndex,pBuffer,1024);
		
		if (nLen > 0)
		{
			if (m_bSegment)
			{
				if (!SegmentSentence(m_wstrReference,pBuffer))
					return;
			}
			else {
				m_wstrReference += ByteToWString(pBuffer);
			}
		}
	}
	 size_t pos(0),pos1(0);
	 while((pos1 = m_wstrReference.find(L"|",pos)) != std::string::npos)
	 {
		 std::wstring t_wstrRef = m_wstrReference.substr(pos,pos1-pos);
		 m_vecReference.push_back(t_wstrRef);
		 if (m_bSegment)
			 pos = pos1+3;
		 else
			pos = pos1+1;
	 }
	 if (m_vecReference.size() == 0)
	 {
		  m_vecReference.push_back(m_wstrReference);
	 }*/
	///segment
//	m_bSegment = TRUE;
	PretreatmentUI(m_bSegment);
	ComputeRough(m_bSegment);
	/////no segment
//	m_bSegment = FALSE;
//	PretreatmentUI(m_bSegment);
//	ComputeRough(m_bSegment);




/*	 m_bSegment = FALSE;
	 dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,1,m_bSegment);
	 m_strScoreOR1.Format("%.04f",dbScore);
	 dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,2,m_bSegment);
	 m_strScoreOR2.Format("%.04f",dbScore);
	 dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,3,m_bSegment);
	 m_strScoreOR3.Format("%.04f",dbScore);
	 dbScore = m_Rouge.Rouge_L(m_wstrCandidate,m_vecReference,m_bSegment);
	 m_strScoreORL.Format("%.04f",dbScore);
	 dbScore = m_Rouge.NormalizedPairwiseLCS(m_wstrCandidate,m_vecReference,m_bSegment);
	 m_strScoreORNPL.Format("%.04f",dbScore);
	 dbScore =  m_Rouge.Rouge_W(m_wstrCandidate,m_vecReference,m_bSegment);
	 m_strScoreORW.Format("%.04f",dbScore);
	 dbScore = m_Rouge.Rouge_S(m_wstrCandidate,m_vecReference,m_bSegment);
	 m_strScoreORS.Format("%.04f",dbScore);


	  m_bSegment = TRUE;
	 dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,1,m_bSegment);
	 m_strScoreSR1.Format("%.04f",dbScore);
	 dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,2,m_bSegment);
	 m_strScoreSR2.Format("%.04f",dbScore);
	 dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,3,m_bSegment);
	 m_strScoreSR3.Format("%.04f",dbScore);
	 dbScore = m_Rouge.Rouge_L(m_wstrCandidate,m_vecReference,m_bSegment);
	 m_strScoreSRL.Format("%.04f",dbScore);
	 dbScore = m_Rouge.NormalizedPairwiseLCS(m_wstrCandidate,m_vecReference,m_bSegment);
	 m_strScoreSRNPL.Format("%.04f",dbScore);
	 dbScore =  m_Rouge.Rouge_W(m_wstrCandidate,m_vecReference,m_bSegment);
	 m_strScoreSRW.Format("%.04f",dbScore);
	 dbScore = m_Rouge.Rouge_S(m_wstrCandidate,m_vecReference,m_bSegment);
	 m_strScoreSRS.Format("%.04f",dbScore);*/

/*	switch(nType)
	{
	case 0://rouge-1
		{
			dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,nType+1,m_bSegment);
			
			//m_wstrCandidate = m_credtCandidate.GetWindowTextA(strline,
		//	const wchar_t *wpText = m_wstrCandidate.c_str();
		//	std::string pp = WstringToByte(wpText);
		//	wchar_t wTest = m_wstrCandidate[0];
		//std::wcout <<wTest<<std::endl;
		}
		break;
	case 1://rouge-2
		{
			dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,nType+1,m_bSegment);
		}
		break;
	case 2://rouge-3
		{
			dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,nType+1,m_bSegment);
		}
		break;
	case 3://rouge-L
		{
			dbScore = m_Rouge.Rouge_L(m_wstrCandidate,m_vecReference,m_bSegment);
		}
		break;
	case 4://normalized pairwise
		{
			dbScore = m_Rouge.NormalizedPairwiseLCS(m_wstrCandidate,m_vecReference,m_bSegment);
		}
		break;
	case 5://rouge-w
		{
			dbScore =  m_Rouge.Rouge_W(m_wstrCandidate,m_vecReference,m_bSegment);
		}
		break;
	case 6://rouge-s
		{
			dbScore = m_Rouge.Rouge_S(m_wstrCandidate,m_vecReference,m_bSegment);
		}
		break;
	default:
		break;
	}*/

/*	if (pBuffer)
	{
		delete []pBuffer;
		pBuffer = NULL;
	}*/
	m_strScore = (_T(""));
//	m_strScore.Format("Score:%.04f",dbScore);
	UpdateData(0);
//	CDialogEx::OnOK();
}


bool  CEvaluationSystemDlg::SegmentSentence(std::wstring &wstrSentence,char *pBuffer)//�ִ�
{
	if(ICTCLAS_Init()) //��ʼ���ִ������
	{
		ICTCLAS_SetPOSmap(0);
		unsigned int nPaLen=strlen(pBuffer); // ��Ҫ�ִʵĳ���
		char* sRst=0;   //�û����з���ռ䣬���ڱ�������
		sRst=(char *)malloc(nPaLen*6); //���鳤��Ϊ�ַ������ȵı���
		int nRstLen=0; //�ִʽ���ĳ���

		nRstLen = ICTCLAS_ParagraphProcess(pBuffer,nPaLen,sRst,CODE_TYPE_UNKNOWN,1);  //�ַ�������
		wstrSentence += ByteToWString(sRst);
		free(sRst);
		ICTCLAS_Exit();	//�ͷ���Դ�˳�
		return true;
	}
	else
	{
		printf("Init fails\n"); 
		return false;
	}
}

std::wstring CEvaluationSystemDlg::ByteToWString(char *pChar)
{
	int size=MultiByteToWideChar(CP_ACP,0,pChar,-1,NULL,0);
	wchar_t *pWch=new wchar_t[size+1];
	wmemset(pWch,0,size+1);
	if(!MultiByteToWideChar(CP_ACP,0,pChar,-1,pWch,size))
	{
		return false;
	}
	std::wstring wstr=pWch;
	int nlen = wstr.size();
	if (pWch)
	{
		delete []pWch;
		pWch = NULL;
	}
	return wstr;
}

std::string  CEvaluationSystemDlg::WstringToByte(const wchar_t *pContent)
{
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,pContent,-1,NULL,0,NULL,FALSE);
   char *psText;
   psText = new char[dwNum+1];
   if(!psText)
   {
    delete []psText;
   }
   memset(psText,0,dwNum+1);
   WideCharToMultiByte (CP_OEMCP,NULL,pContent,-1,psText,dwNum,NULL,FALSE);
   
   std::string strContent = psText;
   if (psText)
   {
	   delete []psText;
	   psText = NULL;
   }
   return strContent;
}


std::wstring s2ws(const std::string& str)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

void CEvaluationSystemDlg::OnBnClickedCheckSegmentation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CEvaluationSystemDlg::OnBnClickedBtnBulk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*	CFileDialog mFileDlg(TRUE, NULL,NULL, 
                         OFN_ALLOWMULTISELECT,
                         	_T("TXT Files (*.XML)|*.XML|All Files (*.*)|*.*||"),
                         AfxGetMainWnd()); 
	int NAMEBUF = 1024*1024;
	mFileDlg.m_ofn.lpstrFile=new TCHAR[NAMEBUF];   // ���¶��� lpstrFile �����С
	memset(mFileDlg.m_ofn.lpstrFile,0,NAMEBUF);  // ��ʼ������Ļ��� 
	mFileDlg.m_ofn.nMaxFile = NAMEBUF;           // �ض��� nMaxFile 
    CString pathName,FileName;
	CString  strXmlFile;

	char CurrentDir[256];
	memset(CurrentDir,0,256);
	
    GetCurrentDirectory(255,CurrentDir);
   
    if(mFileDlg.DoModal ()==IDOK)
    { 
		SetCurrentDirectory(CurrentDir);
	
        POSITION mPos=mFileDlg.GetStartPosition(); 
        while(mPos!=NULL) 
        { 
             pathName=mFileDlg.GetNextPathName(mPos);
		
		
			
		} 
	
    }
	delete [] mFileDlg.m_ofn.lpstrFile;             // �м�ʹ������ͷ���Դ  */
	
/*	std::ofstream out; 
	std::string filename;
	char *pBuffer = new char[256];

	for (int i=1; i<=100; i++)
	{
		memset(pBuffer,0,256);
		sprintf(pBuffer,".\\sample\\r%d.txt",i);
		out.open(pBuffer,std::ios::binary);
		out.close();
		memset(pBuffer,0,256);
		sprintf(pBuffer,".\\sample\\c%d.txt",i);
		out.open(pBuffer,std::ios::binary);
		out <<";" ;
		out.close();

	}
	if (pBuffer)
	{
		delete []pBuffer;
		pBuffer = NULL;
	}*/



/*	std::ifstream in;
	std::string filename,strReference,strCandidate,strLine;
	double nManualScore(0);
	char *pBuffer = new char [256];
	for (int i=1; i<=100; i++)
	{
		memset(pBuffer,0,256);
		sprintf(pBuffer,".\\sample\\r%d.txt",i);
		TRACE(pBuffer);
		TRACE("\n");
		in.open(pBuffer,std::ios::binary);
		while(in >> strLine)
		{
			strReference += strLine;

		}

		in.close();
		in.clear();

		memset(pBuffer,0,256);
		sprintf(pBuffer,".\\sample\\c%d.txt",i);
		in.open(pBuffer,std::ios::binary);
		while(in >> strLine)
		{
			if (strLine.at(0) == ';')
			{
				nManualScore = atof(strLine.substr(1,strLine.length()-1).c_str());
				m_strManualScore = strLine.substr(1,strLine.length()-1).c_str();
				continue;
			}
			strCandidate += strLine;
		}
		in.close();
		in.clear();
		////////////////raw
		m_bSegment = FALSE;
		PretreatmentBulk(m_bSegment,strReference,strCandidate);
		ComputeRough(m_bSegment);
		////////////segment
		 m_bSegment = TRUE;
		 m_wstrCandidate.clear();
		 m_wstrReference.clear();
		 m_vecReference.clear();
		 PretreatmentBulk(m_bSegment,strReference,strCandidate);
		ComputeRough(m_bSegment);

		
	
	}

	m_strScoreOR1.Format("%.02f",m_dbRaw[0]/100*100);
	m_strScoreOR2.Format("%.02f",m_dbRaw[1]/100*100);
	m_strScoreOR3.Format("%.02f",m_dbRaw[2]/100*100);
	m_strScoreORL.Format("%.02f",m_dbRaw[3]/100*100);
	m_strScoreORNPL.Format("%.02f",m_dbRaw[4]/100*100);
	m_strScoreORW.Format("%.02f",m_dbRaw[5]/100*100);
	m_strScoreORS.Format("%.02f",m_dbRaw[6]/100*100);
	m_strScoreORSU.Format("%.02f",m_dbRaw[7]/100*100);
	 m_strScoreSR1.Format("%.02f",m_dbSeg[0]/100*100);
	 m_strScoreSR2.Format("%.02f",m_dbSeg[1]/100*100);
	 m_strScoreSR3.Format("%.02f",m_dbSeg[2]/100*100);
	 m_strScoreSRL.Format("%.02f",m_dbSeg[3]/100*100);
	 m_strScoreSRNPL.Format("%.02f",m_dbSeg[4]/100*100);
	 m_strScoreSRW.Format("%.02f",m_dbSeg[5]/100*100);
	 m_strScoreSRS.Format("%.02f",m_dbSeg[6]/100*100);
	 m_strScoreSRSU.Format("%.02f",m_dbSeg[7]/100*100);
	if (pBuffer)
	{
		delete []pBuffer;
		pBuffer = NULL;
	}*/

	m_hThread = AfxBeginThread(ThreadBulk,(LPVOID)this);
	UpdateData(0);
	
}


bool  CEvaluationSystemDlg::ProcessBulk()
{
	std::ifstream in;
	std::string filename,strReference,strCandidate,strLine;
	double nManualScore(0);
	char *pBuffer = new char [256];

	CMarkup  xml;
	xml.AddElem( "TextSet" );
	CString strTitle;
	for (int i=1; i<=100; i++)
	{

		if (bExitThread)
		{
			if (pBuffer)
			{
				delete []pBuffer;
				pBuffer = NULL;
			}
			return false;
		}

		strTitle.Format("EvaluationSystem   Text%d",i);
		SetWindowTextA(strTitle);

		strReference.clear();
		strCandidate.clear();

		xml.AddChildElem( "TEXT" );
		xml.SetChildAttrib("No",i);
		memset(pBuffer,0,256);
		sprintf_s(pBuffer,256,".\\sample\\r%d.txt",i);
		TRACE(pBuffer);
		TRACE("\n");
		in.open(pBuffer,std::ios::binary);
		while(in >> strLine)
		{
			strReference += strLine;

		}

		in.close();
		in.clear();

		memset(pBuffer,0,256);
		sprintf(pBuffer,".\\sample\\c%d.txt",i);
		in.open(pBuffer,std::ios::binary);
		while(in >> strLine)
		{
			if (strLine.at(0) == ';')
			{
				nManualScore = atof(strLine.substr(1,strLine.length()-1).c_str());
				m_strManualScore = strLine.substr(1,strLine.length()-1).c_str();
				continue;
			}
			strCandidate += strLine;
		}
		in.close();
		in.clear();
		////////////////raw
		m_bSegment = FALSE;
		m_wstrCandidate.clear();
		 m_wstrReference.clear();
		 m_vecReference.clear();
		PretreatmentBulk(m_bSegment,strReference,strCandidate);
		ComputeRough(m_bSegment);
		////////////segment
		 m_bSegment = TRUE;
		 m_wstrCandidate.clear();
		 m_wstrReference.clear();
		 m_vecReference.clear();
		 PretreatmentBulk(m_bSegment,strReference,strCandidate);
		ComputeRough(m_bSegment);


		xml.SetChildAttrib("ManualScore",(int)nManualScore);
		xml.AddChildElem("ROUGE1");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_strScoreOR1 );
		xml.AddChildElem( "SEG",m_strScoreSR1 );
		xml.OutOfElem();

		xml.AddChildElem("ROUGE2");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_strScoreOR2 );
		xml.AddChildElem( "SEG",m_strScoreSR2 );
		xml.OutOfElem();

		xml.AddChildElem("ROUGE3");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_strScoreOR3 );
		xml.AddChildElem( "SEG",m_strScoreSR3 );
		xml.OutOfElem();

		xml.AddChildElem("ROUGE_C2");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_strScoreORC2 );
		xml.AddChildElem( "SEG",m_strScoreSRC2 );
		xml.OutOfElem();

		xml.AddChildElem("ROUGE_C3");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_strScoreORC3);
		xml.AddChildElem( "SEG",m_strScoreSRC3);
		xml.OutOfElem();

		xml.AddChildElem("N2gram");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_str2gramO);
		xml.AddChildElem( "SEG",m_str2gramS);
		xml.OutOfElem();

		xml.AddChildElem("N3gram");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_str3gramO);
		xml.AddChildElem( "SEG",m_str3gramS);
		xml.OutOfElem();

		xml.AddChildElem("N4gram");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_str4gramO);
		xml.AddChildElem( "SEG",m_str4gramS);
		xml.OutOfElem();


		xml.AddChildElem("ROUGEL");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_strScoreORL );
		xml.AddChildElem( "SEG",m_strScoreSRL );
		xml.OutOfElem();

		xml.AddChildElem("ROUGENPL");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_strScoreORNPL );
		xml.AddChildElem( "SEG",m_strScoreSRNPL );
		xml.OutOfElem();

		xml.AddChildElem("ROUGEW");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_strScoreORW );
		xml.AddChildElem( "SEG",m_strScoreSRW );
		xml.OutOfElem();

		xml.AddChildElem("ROUGES");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_strScoreORS );
		xml.AddChildElem( "SEG",m_strScoreSRS );
		xml.OutOfElem();

		xml.AddChildElem("ROUGESU");
		xml.IntoElem();
		xml.AddChildElem( "ORG", m_strScoreORSU );
		xml.AddChildElem( "SEG",m_strScoreSRSU );
		xml.OutOfElem();
		
	
	}
	xml.Save(".\\sample\\result.xml");
	m_strScoreOR1.Format("%.02f",m_dbRaw[0]/100*100);
	m_strScoreOR2.Format("%.02f",m_dbRaw[1]/100*100);
	m_strScoreOR3.Format("%.02f",m_dbRaw[2]/100*100);
	m_strScoreORL.Format("%.02f",m_dbRaw[3]/100*100);
	m_strScoreORNPL.Format("%.02f",m_dbRaw[4]/100*100);
	m_strScoreORW.Format("%.02f",m_dbRaw[5]/100*100);
	m_strScoreORS.Format("%.02f",m_dbRaw[6]/100*100);
	m_strScoreORSU.Format("%.02f",m_dbRaw[7]/100*100);
	 m_strScoreSR1.Format("%.02f",m_dbSeg[0]/100*100);
	 m_strScoreSR2.Format("%.02f",m_dbSeg[1]/100*100);
	 m_strScoreSR3.Format("%.02f",m_dbSeg[2]/100*100);
	 m_strScoreSRL.Format("%.02f",m_dbSeg[3]/100*100);
	 m_strScoreSRNPL.Format("%.02f",m_dbSeg[4]/100*100);
	 m_strScoreSRW.Format("%.02f",m_dbSeg[5]/100*100);
	 m_strScoreSRS.Format("%.02f",m_dbSeg[6]/100*100);
	 m_strScoreSRSU.Format("%.02f",m_dbSeg[7]/100*100);
	if (pBuffer)
	{
		delete []pBuffer;
		pBuffer = NULL;
	}
	m_hThread = NULL;
	AfxMessageBox("computing finish!");
	return true;
}

void   CEvaluationSystemDlg::PretreatmentBulk(BOOL bSegment,std::string strReference, std::string strCandidate)
{
	if (bSegment)
	{
		char *pBuffer = new char[strReference.length()+1];
		memset(pBuffer,0,strReference.length()+1);
		memcpy(pBuffer,strReference.c_str(),strReference.length());
		if (!SegmentSentence(m_wstrReference,pBuffer))
			return;
		
		delete []pBuffer;
		pBuffer = NULL;
		size_t pos(0),pos1(0);
		while((pos1 = m_wstrReference.find(L"|",pos)) != std::string::npos)
		{
			std::wstring t_wstrRef = m_wstrReference.substr(pos,pos1-pos);
			m_vecReference.push_back(t_wstrRef);
			if (bSegment)
				pos = pos1+3;
			else
				pos = pos1+1;
		}
		if (m_vecReference.size() == 0)
		{
			m_vecReference.push_back(m_wstrReference);
		}

		pBuffer = new char[strCandidate.length()+1];
		memset(pBuffer,0,strCandidate.length()+1);
		memcpy(pBuffer,strCandidate.c_str(),strCandidate.length());
		if (!SegmentSentence(m_wstrCandidate,pBuffer))
			return;
		delete []pBuffer;
		pBuffer = NULL;

	}
	else
	{
		char *pBuffer = new char[strReference.length()+1];
		memset(pBuffer,0,strReference.length()+1);
		memcpy(pBuffer,strReference.c_str(),strReference.length());
		m_wstrReference = ByteToWString(pBuffer);
		delete []pBuffer;
		pBuffer = NULL;
		size_t pos(0),pos1(0);
		while((pos1 = m_wstrReference.find(L"|",pos)) != std::string::npos)
		{
			std::wstring t_wstrRef = m_wstrReference.substr(pos,pos1-pos);
			m_vecReference.push_back(t_wstrRef);
			if (bSegment)
				pos = pos1+3;
			else
				pos = pos1+1;
		}
		if (m_vecReference.size() == 0)
		{
			m_vecReference.push_back(m_wstrReference);
		}

		pBuffer = new char[strCandidate.length()+1];
		memset(pBuffer,0,strCandidate.length()+1);
		memcpy(pBuffer,strCandidate.c_str(),strCandidate.length());
		m_wstrCandidate = ByteToWString(pBuffer);
		delete []pBuffer;
		pBuffer = NULL;
	}
}

void  CEvaluationSystemDlg::ComputeRough(BOOL bSegment)
{
	double dbScore(0.0);
	char   t_Score[8];
	memset(t_Score,0,sizeof(t_Score));

	if (!m_bSegment)
	{
	/*	dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,1,m_bSegment);
		m_strScoreOR1.Format("%.04f",dbScore);
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (t_Score == m_strManualScore)
			++m_dbRaw[0]; 
		dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,2,m_bSegment);
		m_strScoreOR2.Format("%.04f",dbScore);
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (t_Score == m_strManualScore)
			++m_dbRaw[1]; 
		dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,3,m_bSegment);
		m_strScoreOR3.Format("%.04f",dbScore);
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (t_Score == m_strManualScore)
			++m_dbRaw[2]; */


		m_Rouge.Rouge_N_All(m_wstrCandidate,m_vecReference,m_bSegment);
		double s1,s2,s3;
		m_Rouge.GetScroeNAll(s1,s2,s3);
		m_strScoreOR1.Format("%.04f",s1);
		if (m_strScoreOR1 == "0")
			m_strScoreOR1 = "0.0001";
		sprintf_s(t_Score,sizeof(t_Score),"%.0f",(s1)*10);
	//	if (strcmp(t_Score,"0") == 0)
	//		strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbRaw[0]; 
		m_strScoreOR2.Format("%.04f",s2);
		if (m_strScoreOR2 == "0")
			m_strScoreOR2 = "0.0001";
		sprintf_s(t_Score,sizeof(t_Score),"%.0f",(s2)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbRaw[1]; 
		m_strScoreOR3.Format("%.04f",s3);
		if (m_strScoreOR3 == "0")
			m_strScoreOR3 = "0.0001";
		sprintf_s(t_Score,sizeof(t_Score),"%.0f",(s3)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbRaw[2]; 

		double c2(0),c3(0);
		m_Rouge.GetScoreC(c2,c3);

		m_strScoreORC2.Format("%.04f",c2);
		m_strScoreORC3.Format("%.04f",c3);

		double Ng2(0.0),Ng3(0),Ng4(0);
		m_Rouge.GetScoreNgram(Ng2,Ng3,Ng4);
		 m_str2gramO.Format("%.04f",Ng2);
		 m_str3gramO.Format("%.04f",Ng3);
		m_str4gramO.Format("%.04f",Ng4);


		dbScore = m_Rouge.Rouge_L(m_wstrCandidate,m_vecReference,m_bSegment);

	/*	m_strScoreORL.Format("%.04f",dbScore);
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (t_Score == m_strManualScore)
			++m_dbRaw[3]; 
		dbScore = m_Rouge.NormalizedPairwiseLCS(m_wstrCandidate,m_vecReference,m_bSegment);
		m_strScoreORNPL.Format("%.04f",dbScore);
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (t_Score == m_strManualScore)
			++m_dbRaw[4]; 
		

		dbScore =  m_Rouge.Rouge_W(m_wstrCandidate,m_vecReference,m_bSegment);
		m_strScoreORW.Format("%.04f",dbScore);
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (t_Score == m_strManualScore)
			++m_dbRaw[5]; */

		/////////////
		double sL,sNPL,sW;
		m_Rouge.GetScoreLAll(sL,sNPL,sW);
		m_strScoreORL.Format("%.04f",sL);
		if (m_strScoreORL == "0")
			m_strScoreORL = "0.0001";
		sprintf(t_Score,"%.0f",(sL)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbRaw[3]; 
		m_strScoreORNPL.Format("%.04f",sNPL);
		if (m_strScoreORNPL == "0")
			m_strScoreORNPL = "0.0001";
		sprintf(t_Score,"%.0f",(sNPL)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"0.0001");
		if (t_Score == m_strManualScore)
			++m_dbRaw[4]; 
		m_strScoreORW.Format("%.04f",sW);
		if (m_strScoreORW == "0")
			m_strScoreORW = "0.0001";
		sprintf(t_Score,"%.0f",(sW)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbRaw[5]; 
		/////////////////////

		dbScore = m_Rouge.Rouge_S(m_wstrCandidate,m_vecReference,m_bSegment);
		m_strScoreORS.Format("%.04f",dbScore);
		if (m_strScoreORS == "0")
			m_strScoreORS = "0.0001";
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"0.0001");
		if (t_Score == m_strManualScore)
			++m_dbRaw[6]; 

		double sS(0.0),sSU(0.0);
		m_Rouge.GetScoreSSU(sS,sSU);
		m_strScoreORSU.Format("%.04f",sSU);
		if (m_strScoreORSU == "0")
			m_strScoreORSU = "0.0001";
		sprintf(t_Score,"%.0f",(sSU)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbRaw[7]; 
	}
	else {

	/*	dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,1,m_bSegment);
		m_strScoreSR1.Format("%.04f",dbScore);
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (t_Score == m_strManualScore)
			++m_dbSeg[0]; 
		dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,2,m_bSegment);
		m_strScoreSR2.Format("%.04f",dbScore);
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (t_Score == m_strManualScore)
			++m_dbSeg[1]; 
		dbScore = m_Rouge.Rouge_N(m_wstrCandidate,m_vecReference,3,m_bSegment);
		m_strScoreSR3.Format("%.04f",dbScore);
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (t_Score == m_strManualScore)
			++m_dbSeg[2]; */

		m_Rouge.Rouge_N_All(m_wstrCandidate,m_vecReference,m_bSegment);
		double s1,s2,s3;
		m_Rouge.GetScroeNAll(s1,s2,s3);
		m_strScoreSR1.Format("%.04f",s1);
		if (m_strScoreSR1 == "0")
			m_strScoreSR1 = "0.0001";
		sprintf_s(t_Score,sizeof(t_Score),"%.0f",(s1)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbSeg[0]; 
		m_strScoreSR2.Format("%.04f",s2);
		if (m_strScoreSR2 == "0")
			m_strScoreSR2 = "0.0001";
		sprintf_s(t_Score,sizeof(t_Score),"%.0f",(s2)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbSeg[1]; 
		m_strScoreSR3.Format("%.04f",s3);
		if (m_strScoreSR3 == "0")
			m_strScoreSR3 = "0.0001";
		sprintf_s(t_Score,sizeof(t_Score),"%.0f",(s3)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbSeg[2]; 


		double c2(0),c3(0);
		m_Rouge.GetScoreC(c2,c3);
		m_strScoreSRC2.Format("%.04f",c2);
		m_strScoreSRC3.Format("%.04f",c3);

		double Ng2(0.0),Ng3(0),Ng4(0);
		m_Rouge.GetScoreNgram(Ng2,Ng3,Ng4);
		m_str2gramS.Format("%.04f",Ng2);
		m_str3gramS.Format("%.04f",Ng3);
		m_str4gramS.Format("%.04f",Ng4);

		dbScore = m_Rouge.Rouge_L(m_wstrCandidate,m_vecReference,m_bSegment);
	/*	m_strScoreSRL.Format("%.04f",dbScore);
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (t_Score == m_strManualScore)
			++m_dbSeg[3]; 
		dbScore = m_Rouge.NormalizedPairwiseLCS(m_wstrCandidate,m_vecReference,m_bSegment);
		m_strScoreSRNPL.Format("%.04f",dbScore);
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (t_Score == m_strManualScore)
			++m_dbSeg[4]; 

		

		dbScore =  m_Rouge.Rouge_W(m_wstrCandidate,m_vecReference,m_bSegment);
		m_strScoreSRW.Format("%.04f",dbScore);
		sprintf(t_Score,"%.0f",(dbScore)*10);
		if (t_Score == m_strManualScore)
			++m_dbSeg[5]; */
		/////////////
		double sL,sNPL,sW;
		m_Rouge.GetScoreLAll(sL,sNPL,sW);
		m_strScoreSRL.Format("%.04f",sL);
		if (m_strScoreSRL == "0")
			m_strScoreSRL = "0.0001";
		sprintf_s(t_Score,sizeof(t_Score),"%.0f",(sL)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbSeg[3]; 
		m_strScoreSRNPL.Format("%.04f",sNPL);
		if (m_strScoreSRNPL == "0")
			m_strScoreSRNPL = "0.0001";
		sprintf_s(t_Score,sizeof(t_Score),"%.0f",(sNPL)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbSeg[4]; 
		m_strScoreSRW.Format("%.04f",sW);
		if (m_strScoreSRW == "0")
			m_strScoreSRW = "0.0001";
		sprintf_s(t_Score,sizeof(t_Score),"%.0f",(sW)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbSeg[5]; 
		/////////////////////

		dbScore = m_Rouge.Rouge_S(m_wstrCandidate,m_vecReference,m_bSegment);
		m_strScoreSRS.Format("%.04f",dbScore);
		if (m_strScoreSRS == "0")
			m_strScoreSRS = "0.0001";
		sprintf_s(t_Score,sizeof(t_Score),"%.0f",(dbScore)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbSeg[6]; 

		double sS,sSU;
		m_Rouge.GetScoreSSU(sS,sSU);
		m_strScoreSRSU.Format("%.04f",sSU);
		if (m_strScoreSRSU == "0")
			m_strScoreSRSU = "0.0001";
		sprintf_s(t_Score,sizeof(t_Score),"%.0f",(sSU)*10);
		if (strcmp(t_Score,"0") == 0)
			strcpy(t_Score,"1");
		if (t_Score == m_strManualScore)
			++m_dbSeg[7]; 

	}
	
}

void  CEvaluationSystemDlg::PretreatmentUI(BOOL bSegment)
{
//	UpdateData();
	char *pBuffer = new char[1024];
	int nLineCount = m_credtCandidate.GetLineCount();
	//m_credtCandidate.GetWindowTextA(pBuffer,1024);
	m_wstrCandidate.clear();
	m_wstrReference.clear();
	m_vecReference.clear();
	int nIndex;
	for(nIndex = 0; nIndex < nLineCount;nIndex++)
	{
		memset(pBuffer,0,1024);

		int nLen = m_credtCandidate.GetLine(nIndex,pBuffer,1024);
		int pp = pBuffer[nLen-1];
		if (nLen > 0)
		{
			if (bSegment)
			{
				if (!SegmentSentence(m_wstrCandidate,pBuffer))
					return;
			}
			else {
				m_wstrCandidate += ByteToWString(pBuffer);
			}
		}
	}
	nLineCount = m_credtReference.GetLineCount();
	for(nIndex = 0; nIndex < nLineCount;nIndex++)
	{
		memset(pBuffer,0,1024);

		int nLen = m_credtReference.GetLine(nIndex,pBuffer,1024);

		if (nLen > 0)
		{
			if (bSegment)
			{
				if (!SegmentSentence(m_wstrReference,pBuffer))
					return;
			}
			else {
				m_wstrReference += ByteToWString(pBuffer);
			}
		}
	}
	size_t pos(0),pos1(0);
	while((pos1 = m_wstrReference.find(L"|",pos)) != std::string::npos)
	{
		std::wstring t_wstrRef = m_wstrReference.substr(pos,pos1-pos);
		m_vecReference.push_back(t_wstrRef);
		if (bSegment)
			pos = pos1+3;
		else
			pos = pos1+1;
	}
	if (m_vecReference.size() == 0)
	{
		m_vecReference.push_back(m_wstrReference);
	}

	if (pBuffer)
	{
		delete []pBuffer;
		pBuffer = NULL;
	}
}

void CEvaluationSystemDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (m_hThread)
	{
		bExitThread = true;

		while(1)
		{
			DWORD result;
			MSG msg;

			result = MsgWaitForMultipleObjects(1,&(m_hThread->m_hThread),FALSE,INFINITE,QS_ALLINPUT);
			if (result == WAIT_OBJECT_0)
			{
				break;
			}
			else
			{
				PeekMessage(&msg,NULL,0,0,PM_REMOVE);
				DispatchMessage(&msg);
			}
		}

	}
	CDialogEx::OnCancel();
}


LRESULT  CEvaluationSystemDlg::OnFinishThread(WPARAM wParam, LPARAM lParam)
{
	int nFlag = (int)wParam;
	if (nFlag == 1)
		UpdateData(0);

	return 0;
}

void CEvaluationSystemDlg::OnBnClickedBtnCorrelation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	real_1d_array arrManual;
	//	real_1d_array arrR1_org,arrR1_seg,arrR2_org,arrR2_seg,arrR3_org,arrR3_seg;
	arrManual.setlength(100);
	CMarkup  xml,correlation;
	CString strPath(_T("")),manualscore(_T("")),score(_T(""));
	strPath.Format(".\\sample\\result.xml");
	double  r1_od(0.0),r1_sd(0.0),r2_od(0.0),r2_sd(0.0),r3_od(0.0),r3_sd(0.0),rL_od(0.0),rL_sd(0.0),
		rNPL_od(0.0),rNPL_sd(0.0),rW_od(0.0),rW_sd(0.0),rS_od(0.0),rS_sd(0.0),rSU_od(0.0),rSU_sd(0.0),
		//////////spearman correlation
		rho_r1o(0.0),rho_r1s(0.0),rho_r2o(0.0),rho_r2s(0.0),rho_r3o(0.0),rho_r3s(0.0),rho_rlo(0.0),
		rho_rls(0.0),rho_rnplo(0.0),rho_rnpls(0.0),rho_rwo(0.0),rho_rws(0.0),rho_rso(0.0),rho_rss(0.0),
		rho_rsuo(0.0),rho_rsus(0.0),rho_rc2o(0.0),rho_rc2s(0.0),rho_rc3o(0.0),rho_rc3s(0.0),
		rho_2gramo(0),rho_2grams(0),rho_3gramo(0),rho_3grams(0),rho_4gramo(0),rho_4grams(0),
		////////pearson correlation
		ps_rho_r1o(0.0),ps_rho_r1s(0.0),ps_rho_r2o(0.0),ps_rho_r2s(0.0),ps_rho_r3o(0.0),ps_rho_r3s(0.0),ps_rho_rlo(0.0),
		ps_rho_rls(0.0),ps_rho_rnplo(0.0),ps_rho_rnpls(0.0),ps_rho_rwo(0.0),ps_rho_rws(0.0),ps_rho_rso(0.0),ps_rho_rss(0.0),
		ps_rho_rsuo(0.0),ps_rho_rsus(0.0),ps_rho_rc2o(0.0),ps_rho_rc2s(0.0),ps_rho_rc3o(0.0),ps_rho_rc3s(0.0),
		ps_rho_2gramo(0),ps_rho_2grams(0),ps_rho_3gramo(0),ps_rho_3grams(0),ps_rho_4gramo(0),ps_rho_4grams(0);
	real_1d_array arrR1_o ,arrR2_o,arrR3_o,arrRL_o,arrRNPL_o,arrRW_o,arrRS_o,arrRSU_o,arrR1_s ,
		arrR2_s,arrR3_s,arrRL_s,arrRNPL_s,arrRW_s,arrRS_s,arrRSU_s,arrRC2_o,arrRC3_o,arrRC2_s,arrRC3_s,
		arr2gram_o,arr2gram_s,arr3gram_o,arr3gram_s,arr4gram_o,arr4gram_s;
	arrR1_o.setlength(100) ;
	arrR2_o.setlength(100);
	arrR3_o.setlength(100),
		arrRL_o.setlength(100);
	arrRNPL_o.setlength(100);
	arrRW_o.setlength(100);
	arrRS_o.setlength(100);
	arrRSU_o.setlength(100);
	arrR1_s.setlength(100) ;
	arrR2_s.setlength(100);
	arrR3_s.setlength(100);
	arrRL_s.setlength(100);
	arrRNPL_s.setlength(100);
	arrRW_s.setlength(100),
		arrRS_s.setlength(100);
	arrRSU_s.setlength(100);
	arrRC2_o.setlength(100);
	arrRC3_o.setlength(100);
	arrRC2_s.setlength(100);
	arrRC3_s.setlength(100);
	arr2gram_o.setlength(100);
	arr2gram_s.setlength(100);
	arr3gram_o.setlength(100);
	arr3gram_s.setlength(100);
	arr4gram_o.setlength(100);
	arr4gram_s.setlength(100);
	double     x(0),y1(0),y2(0);
	int	 nIndex(0);
	correlation.AddElem("Correlation");
	
//	double  arrManual[101],arrRouge1[101];
	if(xml.Load(strPath))
	{
		while ( xml.FindChildElem("TEXT") )
		{
			manualscore = xml.GetChildAttrib("ManualScore");
			x = atof(manualscore);
			arrManual(nIndex) = x;
			if( xml.FindChildElem("ROUGE1"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arrR1_o(nIndex) = y1;

				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arrR1_s(nIndex) = y2;
				xml.OutOfElem();

			}
		
			//////////////
			if( xml.FindChildElem("ROUGE2"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arrR2_o(nIndex) = y1;
				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arrR2_s(nIndex) = y2;
				xml.OutOfElem();

			}
		
			///////////
	
			if( xml.FindChildElem("ROUGE3"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arrR3_o(nIndex) = y1;
				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arrR3_s(nIndex) = y2;
				xml.OutOfElem();

			}
			
			if( xml.FindChildElem("ROUGE_C2"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arrRC2_o(nIndex) = y1;
				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arrRC2_s(nIndex) = y2;
				xml.OutOfElem();

			}

			if( xml.FindChildElem("ROUGE_C3"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arrRC3_o(nIndex) = y1;
				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arrRC3_s(nIndex) = y2;
				xml.OutOfElem();
			}

			if( xml.FindChildElem("N2gram"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arr2gram_o(nIndex) = y1;
				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arr2gram_s(nIndex) = y2;
				xml.OutOfElem();
			}

			if( xml.FindChildElem("N3gram"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arr3gram_o(nIndex) = y1;
				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arr3gram_s(nIndex) = y2;
				xml.OutOfElem();
			}

			if( xml.FindChildElem("N4gram"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arr4gram_o(nIndex) = y1;
				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arr4gram_s(nIndex) = y2;
				xml.OutOfElem();
			}

		
			///////////
		
			if( xml.FindChildElem("ROUGEL"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arrRL_o(nIndex) = y1;
				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arrRL_s(nIndex) = y2;
				xml.OutOfElem();

			}
	//		rL_od += pow((x-y1),2.0);
		//	rL_sd += pow((x-y2),2.0);
			///////////
		
			if( xml.FindChildElem("ROUGENPL"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arrRNPL_o(nIndex) = y1;
				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arrRNPL_s(nIndex) = y2;
				xml.OutOfElem();

			}
		
			///////////
		
			if( xml.FindChildElem("ROUGEW"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arrRW_o(nIndex) = y1;
				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arrRW_s(nIndex) = y2;
				xml.OutOfElem();

			}
			///////////
			
			if( xml.FindChildElem("ROUGES"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arrRS_o(nIndex) = y1;
				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arrRS_s(nIndex) = y2;
				xml.OutOfElem();

			}
		
			///////////
		
			if( xml.FindChildElem("ROUGESU"))
			{
				xml.IntoElem();
				xml.FindChildElem( "ORG" );
				score = xml.GetChildData();
				y1 = atof(score);
				arrRSU_o(nIndex) = y1;
				xml.FindChildElem( "SEG" );
				score = xml.GetChildData();
				y2 = atof(score);
				arrRSU_s(nIndex) = y2;
				xml.OutOfElem();

			}
		
			++nIndex;
		}
	
	
		//v = cov2(arrx, arry);

		/////spearman correlation
	
		rho_r1o= spearmancorr2(arrManual,arrR1_o);
		rho_r1s = spearmancorr2(arrManual,arrR1_o);
		rho_r2o = spearmancorr2(arrManual,arrR2_o);
		rho_r2s= spearmancorr2(arrManual,arrR2_s);
		rho_r3o  = spearmancorr2(arrManual,arrR3_o);
		rho_r3s= spearmancorr2(arrManual,arrR3_s);

	
		rho_rc2o = spearmancorr2(arrManual,arrRC2_o);
		rho_rc2s =  spearmancorr2(arrManual,arrRC2_s);
		rho_rc3o =  spearmancorr2(arrManual,arrRC3_o);
		rho_rc3s = spearmancorr2(arrManual,arrRC3_s);

	
		rho_2gramo = spearmancorr2(arrManual,arr2gram_o);
		rho_2grams = spearmancorr2(arrManual,arr2gram_s);
		rho_3gramo = spearmancorr2(arrManual,arr3gram_o);
		rho_3grams = spearmancorr2(arrManual,arr3gram_s);
		rho_4gramo = spearmancorr2(arrManual,arr4gram_o);
		rho_4grams = spearmancorr2(arrManual,arr4gram_s);

		rho_rlo  = spearmancorr2(arrManual,arrRL_o);
		rho_rls= spearmancorr2(arrManual,arrRL_s);

		rho_rnplo  = spearmancorr2(arrManual,arrRNPL_o);
		rho_rnpls= spearmancorr2(arrManual,arrRNPL_s);

		rho_rwo  = spearmancorr2(arrManual,arrRW_o);
		rho_rws= spearmancorr2(arrManual,arrRW_s);

		rho_rso  = spearmancorr2(arrManual,arrRS_o);
		rho_rss= spearmancorr2(arrManual,arrRS_s);
		rho_rsuo  = spearmancorr2(arrManual,arrRSU_o);
		rho_rsus= spearmancorr2(arrManual,arrRSU_s);
		//////pearson correlation

		ps_rho_r1o = pearsoncorr2(arrManual,arrR1_o);
		ps_rho_r1s = pearsoncorr2(arrManual,arrR1_o);
		ps_rho_r2o= pearsoncorr2(arrManual,arrR2_o);
		ps_rho_r2s = pearsoncorr2(arrManual,arrR2_s);
		ps_rho_r3o=  pearsoncorr2(arrManual,arrR3_o);
		ps_rho_r3s = pearsoncorr2(arrManual,arrR3_s);

		ps_rho_rc2o = pearsoncorr2(arrManual,arrRC2_o);
		ps_rho_rc2s =  pearsoncorr2(arrManual,arrRC2_s);
		ps_rho_rc3o =  pearsoncorr2(arrManual,arrRC3_o);
		ps_rho_rc3s = pearsoncorr2(arrManual,arrRC3_s);

		ps_rho_rlo= pearsoncorr2(arrManual,arrRL_o);
		ps_rho_rls = pearsoncorr2(arrManual,arrRL_s);
		ps_rho_rnplo= pearsoncorr2(arrManual,arrRNPL_o);
		ps_rho_rnpls =  pearsoncorr2(arrManual,arrRNPL_s);
		ps_rho_rwo = pearsoncorr2(arrManual,arrRW_o);
		ps_rho_rws = pearsoncorr2(arrManual,arrRW_s);
		ps_rho_rso = pearsoncorr2(arrManual,arrRS_o);
		ps_rho_rss = pearsoncorr2(arrManual,arrRS_s);
		ps_rho_rsuo = pearsoncorr2(arrManual,arrRSU_o);
		ps_rho_rsus  =  pearsoncorr2(arrManual,arrRSU_s);


		ps_rho_2gramo = pearsoncorr2(arrManual,arr2gram_o);
		ps_rho_2grams = pearsoncorr2(arrManual,arr2gram_s);
		ps_rho_3gramo = pearsoncorr2(arrManual,arr3gram_o);
		ps_rho_3grams = pearsoncorr2(arrManual,arr3gram_s);
		ps_rho_4gramo = pearsoncorr2(arrManual,arr4gram_o);
		ps_rho_4grams = pearsoncorr2(arrManual,arr4gram_s);

	}
	
	CString strTemp(_T(""));
	correlation.AddChildElem("Correlation");
	correlation.AddChildElem("ROUGE1");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_r1o);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_r1o);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_r1s);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_r1s);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();

	correlation.AddChildElem("ROUGE2");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_r2o);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_r2o);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_r2s);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_r2s);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();

	correlation.AddChildElem("ROUGE3");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_r3o);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_r3o);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_r3s);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_r3s);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();

	correlation.AddChildElem("C2");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_rc2o);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_rc2o);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_rc2s);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_rc2s);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();

	correlation.AddChildElem("C3");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_rc3o);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_rc3o);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_rc3s);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_rc3s);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();

	correlation.AddChildElem("N2gram");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_2gramo);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_2gramo);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_2grams);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_2grams);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();

	correlation.AddChildElem("N3gram");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_3gramo);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_3gramo);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_3grams);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_3grams);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();

	correlation.AddChildElem("N4gram");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_4gramo);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_4gramo);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_4grams);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_4grams);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();

	correlation.AddChildElem("RougeL");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_rlo);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_rlo);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_rls);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_rls);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();

	correlation.AddChildElem("RougeNPL");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_rnplo);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_rnplo);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_rnpls);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_rnpls);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();

	correlation.AddChildElem("RougeW");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_rwo);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_rwo);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_rws);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_rws);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();

	correlation.AddChildElem("RougeS");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_rso);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_rso);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_rss);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_rss);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();

	correlation.AddChildElem("RougeSU");
	correlation.IntoElem();
	strTemp.Format("%.04f",rho_rsuo);
	correlation.AddChildElem( "SPEARMAN_CORR_ORG", strTemp );
	strTemp.Format("%.04f",ps_rho_rsuo);
	correlation.AddChildElem( "PEARSON_CORR_ORG", strTemp );
	strTemp.Format("%.04f",rho_rsus);
	correlation.AddChildElem( "SPEARMAN_CORR_SEG",strTemp );
	strTemp.Format("%.04f",ps_rho_rsus);
	correlation.AddChildElem( "PEARSON_CORR_SEG",strTemp );
	correlation.OutOfElem();



	correlation.Save(".\\sample\\correlation.xml");



	TRACE("rho1 = %f,rho2 = %f,rho_r2o= %f,rho_r2s= %f,rho_r3o= %f,rho_r3s= %f,rho_rlo= %f,rho_rls= %f,rho_rnplo= %f,rho_rnpls= %f,rho_rwo= %f,rho_rws= %f,rho_rso= %f,rho_rss= %f,rho_rsuo= %f,rho_rsus= %f,rho_2gramo= %f,rho_2grams= %f,rho_3gramo= %f,rho_3grams= %f,rho_4gramo= %f,rho_4grams= %f\n",
		rho_r1o,rho_r1s,rho_r2o,rho_r2s,rho_r3o,rho_r3s,rho_rlo,rho_rls,rho_rnplo,rho_rnpls,rho_rwo,rho_rws,rho_rso,rho_rss,rho_rsuo,rho_rsus,rho_2gramo,rho_2grams,rho_3gramo,rho_3grams,rho_4gramo,rho_4grams);
	TRACE("ps_rho1 = %f,ps_rho2 = %f,ps_rho_r2o= %f,ps_rho_r2s= %f,ps_rho_r3o= %f,ps_rho_r3s= %f,ps_rho_rlo= %f,ps_rho_rls= %f,ps_rho_rnplo= %f,ps_rho_rnpls= %f,ps_rho_rwo= %f,ps_rho_rws= %f,ps_rho_rso= %f,ps_rho_rss= %f,ps_rho_rsuo= %f,ps_rho_rsus= %f,ps_rho_2gramo= %f,ps_rho_2grams= %f,ps_rho_3gramo= %f,ps_rho_3grams= %f,ps_rho_4gramo= %f,ps_rho_4grams= %f\n",
		ps_rho_r1o,ps_rho_r1s,ps_rho_r2o,ps_rho_r2s,ps_rho_r3o,ps_rho_r3s,ps_rho_rlo,ps_rho_rls,ps_rho_rnplo,ps_rho_rnpls,ps_rho_rwo,
		ps_rho_rws,ps_rho_rso,ps_rho_rss,ps_rho_rsuo,ps_rho_rsus,ps_rho_2gramo,ps_rho_2grams,ps_rho_3gramo,ps_rho_3grams,ps_rho_4gramo,ps_rho_4grams);
}
