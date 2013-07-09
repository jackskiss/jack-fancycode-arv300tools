// ARV300_BusPopup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300_BusPopup.h"
#include "afxdialogex.h"


// CARV300_BusPopup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CARV300_BusPopup, CDialogEx)

CARV300_BusPopup::CARV300_BusPopup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CARV300_BusPopup::IDD, pParent)
{

}

CARV300_BusPopup::~CARV300_BusPopup()
{
}

void CARV300_BusPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ANIMATED_IMAGE, m_picProgress);
}


BEGIN_MESSAGE_MAP(CARV300_BusPopup, CDialogEx)
	ON_MESSAGE(ARV300_USER_MESSAGE_POPUPEXIT, &CARV300_BusPopup::PopupExit)
END_MESSAGE_MAP()


// CARV300_BusPopup 메시지 처리기입니다.

LRESULT CARV300_BusPopup::PopupExit(WPARAM wParam, LPARAM lParam)
{
	this->EndDialog(1);
	return 0;
}

BOOL CARV300_BusPopup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_picProgress.Load(MAKEINTRESOURCE(IDR_PROGRESS),_T("GIF")))
		m_picProgress.Draw();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
