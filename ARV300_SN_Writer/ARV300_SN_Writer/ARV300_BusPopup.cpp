// ARV300_BusPopup.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300_BusPopup.h"
#include "afxdialogex.h"
#include "ARV300_SN_WriterDlg.h"

static CARV300_SN_WriterDlg *m_parent;

// CARV300_BusPopup ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CARV300_BusPopup, CDialogEx)

CARV300_BusPopup::CARV300_BusPopup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CARV300_BusPopup::IDD, pParent)
{
	m_parent = (CARV300_SN_WriterDlg *)pParent;
}

CARV300_BusPopup::~CARV300_BusPopup()
{
}

void CARV300_BusPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CARV300_BusPopup)
	DDX_Control(pDX, IDC_ANIMATED_IMAGE, m_picProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CARV300_BusPopup, CDialogEx)
	ON_MESSAGE(ARV300_USER_MESSAGE_POPUPEXIT, &CARV300_BusPopup::PopupExit)
	ON_STN_CLICKED(IDC_ANIMATED_IMAGE, &CARV300_BusPopup::OnStnClickedAnimatedImage)
END_MESSAGE_MAP()


// CARV300_BusPopup �޽��� ó�����Դϴ�.

LRESULT CARV300_BusPopup::PopupExit(WPARAM wParam, LPARAM lParam)
{
	this->EndDialog(1);
	return 0;
}

BOOL CARV300_BusPopup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFont *m_Font1 = new CFont;
    m_Font1->CreatePointFont(100, "Arial Bold");
	GetDlgItem(IDC_STATIC_WAIT)->SetFont(m_Font1);

	if (m_picProgress.Load(MAKEINTRESOURCE(IDR_PROGRESS),_T("GIF")))
	{
		m_picProgress.Draw();
	}

	m_parent->m_popupInfo.self = this->GetSafeHwnd();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CARV300_BusPopup::OnStnClickedAnimatedImage()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
