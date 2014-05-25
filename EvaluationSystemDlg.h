
// EvaluationSystemDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Rouge.h"
#include <string>
#include <vector>


// CEvaluationSystemDlg �Ի���
class CEvaluationSystemDlg : public CDialogEx
{
// ����
public:
	CEvaluationSystemDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EVALUATIONSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
///
	std::wstring ByteToWString(char *pChar);
	std::string  WstringToByte(const wchar_t *pContent);
	bool    SegmentSentence(std::wstring &wstrSentence,char *pBuffer);//�ִ�

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
