
// EvaluationSystem.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEvaluationSystemApp:
// �йش����ʵ�֣������ EvaluationSystem.cpp
//

class CEvaluationSystemApp : public CWinApp
{
public:
	CEvaluationSystemApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEvaluationSystemApp theApp;