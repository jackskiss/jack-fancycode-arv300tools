#pragma once


// CARV300About 대화 상자입니다.

class CARV300About : public CDialogEx
{
	DECLARE_DYNAMIC(CARV300About)

public:
	CARV300About(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CARV300About();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
