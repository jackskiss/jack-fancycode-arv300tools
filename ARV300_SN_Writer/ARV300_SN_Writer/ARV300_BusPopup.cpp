// ARV300_BusPopup.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300_BusPopup.h"
#include "afxdialogex.h"


// CARV300_BusPopup ��ȭ �����Դϴ�.

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
}


BEGIN_MESSAGE_MAP(CARV300_BusPopup, CDialogEx)
	ON_MESSAGE(ARV300_USER_MESSAGE_POPUPEXIT, &CARV300_BusPopup::PopupExit)
END_MESSAGE_MAP()


// CARV300_BusPopup �޽��� ó�����Դϴ�.

LRESULT CARV300_BusPopup::PopupExit(WPARAM wParam, LPARAM lParam)
{
	this->EndDialog(1);
	return 0;
}