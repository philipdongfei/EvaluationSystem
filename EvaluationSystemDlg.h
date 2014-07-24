
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
	CString m_strScoreOR1;
	CString m_strScoreSR1;
	CString m_strScoreOR2;
	CString m_strScoreSR2;
	CString m_strScoreOR3;
	CString m_strScoreSR3;
	CString m_strScoreORL;
	CString m_strScoreSRL;
	CString m_strScoreORNPL;
	CString m_strScoreSRNPL;
	CString m_strScoreORW;
	CString m_strScoreSRW;
	CString m_strScoreORS;
	CString m_strScoreSRS;
	CString m_strScoreORSU;
	CString m_strScoreSRSU;

	double  m_dbRaw[8];
	double  m_dbSeg[8];
	std::string m_strManualScore;
	
	afx_msg void OnBnClickedCancel();

	CWinThread *m_hThread;
	afx_msg void OnBnClickedBtnCorrelation();
};
