#pragma once


// CARV300About ��ȭ �����Դϴ�.

class CARV300About : public CDialogEx
{
	DECLARE_DYNAMIC(CARV300About)

public:
	CARV300About(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CARV300About();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
