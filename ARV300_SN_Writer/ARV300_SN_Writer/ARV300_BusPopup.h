#pragma once
#include "afxwin.h"
#include "PictureEx.h"

// CARV300_BusPopup ��ȭ �����Դϴ�.

class CARV300_BusPopup : public CDialogEx
{
	DECLARE_DYNAMIC(CARV300_BusPopup)

public:
	CARV300_BusPopup(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CARV300_BusPopup();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_POPUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	LRESULT CARV300_BusPopup::PopupExit(WPARAM wParam, LPARAM lParam);
public:
	CEdit m_testEdit;
	CPictureEx m_picProgress;
	virtual BOOL OnInitDialog();
};
