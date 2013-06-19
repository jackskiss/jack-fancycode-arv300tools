
// ARV300_SN_WriterDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"


// CARV300_SN_WriterDlg ��ȭ ����
class CARV300_SN_WriterDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CARV300_SN_WriterDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ARV300_SN_WRITER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpenSn();
	CListCtrl m_SNListCtrl;
	CString strSNFileName;
private:
	int ExcelToListCtrl(CString strExcelFilePath);
	CString GetExcelDriver(void);
	int InsertRow(CString strSN, CString strMasterDate, CString strSlaveDate);

};
