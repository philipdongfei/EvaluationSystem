
// EvaluationSystemDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Rouge.h"
#include <string>
#include <vector>


// CEvaluationSystemDlg 对话框
class CEvaluationSystemDlg : public CDialogEx
{
// 构造
public:
	CEvaluationSystemDlg(CWnd* pParent = NULL);	// 标准构造函数
	bool    ProcessBulk();//
	CString m_strScoreORC2;
	CString m_strScoreSRC2;
	CString m_strScoreORC3;
	CString m_strScoreSRC3;
// 对话框数据
	enum { IDD = IDD_EVALUATIONSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
///
	std::wstring ByteToWString(char *pChar);
	std::string  WstringToByte(const wchar_t *pContent);
	bool    SegmentSentence(std::wstring &wstrSentence,char *pBuffer);//分词
	void    ComputeRough(BOOL bSegment);
	void    PretreatmentUI(BOOL bSegment);
	void    PretreatmentBulk(BOOL bSegment,std::string strReference, std::string strCandidate);
	
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT  OnFinishThread(WPARAM , LPARAM );
	DECLARE_MESSAGE_MAP()
public:
	CRichEditCtrl m_credtCandidate;
	CRichEditCtrl m_credtReference;
	CComboBox m_cmbType;
	afx_msg void OnBnClickedOk();
	CStatic m_stcScore;
	CString m_strScore;

	std::wstring  m_wstrCandidate;
	std::wstring  m_wstrReference;
	std::vector<std::wstring>   m_vecReference; 
	CRouge    m_Rouge;
	afx_msg void OnBnClickedCheckSegmentation();
	afx_msg void OnBnClickedBtnBulk();
	BOOL m_bSegment;
	CString m_strScoreOR1, m_strScoreSR1, m_strScoreOR2, m_strScoreSR2, m_strScoreOR3, m_strScoreSR3;
	CString m_strScoreORL, m_strScoreSRL, m_strScoreORNPL, m_strScoreSRNPL, m_strScoreORW, m_strScoreSRW;
	CString m_strScoreORS, m_strScoreSRS, m_strScoreORSU, m_strScoreSRSU;
	CString m_str2gramO, m_str2gramS, m_str3gramO, m_str3gramS, m_str4gramO, m_str4gramS;
	CString m_strRLO, m_strRLS, m_strRNPLO, m_strRNPLS, m_strRWO, m_strRWS;

	double  m_dbRaw[8];
	double  m_dbSeg[8];
	std::string m_strManualScore;
	
	afx_msg void OnBnClickedCancel();

	CWinThread *m_hThread;
	afx_msg void OnBnClickedBtnCorrelation();
};
