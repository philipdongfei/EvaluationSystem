
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

// 对话框数据
	enum { IDD = IDD_EVALUATIONSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
///
	std::wstring ByteToWString(char *pChar);
	std::string  WstringToByte(const wchar_t *pContent);
	bool    SegmentSentence(std::wstring &wstrSentence,char *pBuffer);//分词

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
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
	BOOL m_bSegment;
};
