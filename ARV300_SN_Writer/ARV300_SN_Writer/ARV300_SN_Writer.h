
// ARV300_SN_Writer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CARV300_SN_WriterApp:
// �� Ŭ������ ������ ���ؼ��� ARV300_SN_Writer.cpp�� �����Ͻʽÿ�.
//

class CARV300_SN_WriterApp : public CWinApp
{
public:
	CARV300_SN_WriterApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	afx_msg void OnAboutArv300snwriter();
};

extern CARV300_SN_WriterApp theApp;