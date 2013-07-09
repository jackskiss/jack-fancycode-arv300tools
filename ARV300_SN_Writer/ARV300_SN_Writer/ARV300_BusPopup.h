#pragma once
#include "afxwin.h"
#include "PictureEx.h"

// CARV300_BusPopup 대화 상자입니다.

class CARV300_BusPopup : public CDialogEx
{
	DECLARE_DYNAMIC(CARV300_BusPopup)

public:
	CARV300_BusPopup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CARV300_BusPopup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_POPUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	LRESULT CARV300_BusPopup::PopupExit(WPARAM wParam, LPARAM lParam);
public:
	CEdit m_testEdit;
	CPictureEx m_picProgress;
	virtual BOOL OnInitDialog();
};
