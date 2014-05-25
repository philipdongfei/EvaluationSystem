
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
}

BEGIN_MESSAGE_MAP(CEvaluationSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CEvaluationSystemDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_SEGMENTATION, &CEvaluationSystemDlg::OnBnClickedCheckSegmentation)
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
	m_cmbType.SetCurSel(0);
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
	double dbScore(0.0);
	int nType = m_cmbType.GetCurSel();
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
	 }

	switch(nType)
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
	case 4:
		{
			dbScore = m_Rouge.NormalizedPairwiseLCS(m_wstrCandidate,m_vecReference,m_bSegment);
		}
		break;
	default:
		break;
	}

	if (pBuffer)
	{
		delete []pBuffer;
		pBuffer = NULL;
	}
	m_strScore = (_T(""));
	m_strScore.Format("Score:%.04f",dbScore);
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
