// ARV300_COMPort.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300_COMPort.h"
#include "afxdialogex.h"


// CARV300_COMPort ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CARV300_COMPort, CDialogEx)

CARV300_COMPort::CARV300_COMPort(CWnd* pParent /*=NULL*/)
	: CDialogEx(CARV300_COMPort::IDD, pParent)
{

}

CARV300_COMPort::~CARV300_COMPort()
{
}

void CARV300_COMPort::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CARV300_COMPort, CDialogEx)
END_MESSAGE_MAP()


// CARV300_COMPort �޽��� ó�����Դϴ�.
