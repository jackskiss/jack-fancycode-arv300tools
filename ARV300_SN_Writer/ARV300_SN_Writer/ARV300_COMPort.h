#pragma once
#include "afxwin.h"


// CARV300_COMPort 대화 상자입니다.

class CARV300_COMPort : public CDialogEx
{
	DECLARE_DYNAMIC(CARV300_COMPort)

public:
	CARV300_COMPort(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CARV300_COMPort();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_COMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboMPort;
	CComboBox m_comboSPort;
	void comportlist(void);
	afx_msg void OnCbnSelchangeComboMport();
	CString m_strMPort;
	CString m_strSPort;
	afx_msg void OnCbnSelchangeComboSport();
	bool m_statusMPort;
	bool m_statusSPort;
};
