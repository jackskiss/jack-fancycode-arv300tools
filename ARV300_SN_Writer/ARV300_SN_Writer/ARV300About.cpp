// ARV300About.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300About.h"
#include "afxdialogex.h"


// CARV300About 대화 상자입니다.

IMPLEMENT_DYNAMIC(CARV300About, CDialogEx)

CARV300About::CARV300About(CWnd* pParent /*=NULL*/)
	: CDialogEx(CARV300About::IDD, pParent)
{

}

CARV300About::~CARV300About()
{
}

void CARV300About::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CARV300About, CDialogEx)
END_MESSAGE_MAP()


// CARV300About 메시지 처리기입니다.
