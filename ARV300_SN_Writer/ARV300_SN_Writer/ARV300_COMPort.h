#pragma once


// CARV300_COMPort ��ȭ �����Դϴ�.

class CARV300_COMPort : public CDialogEx
{
	DECLARE_DYNAMIC(CARV300_COMPort)

public:
	CARV300_COMPort(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CARV300_COMPort();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_COMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
