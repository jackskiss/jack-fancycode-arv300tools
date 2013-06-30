
// ARV300_SN_WriterDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"

// For Database
#include <afxdb.h>         // MFC ODBC database classes
#include <odbcinst.h>
#include "afxwin.h"


typedef enum {
	MS_BOTH,
	M_ONLY,
	S_ONLY
} WR_Type;


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
	CDatabase m_SNDB;
	CRecordset m_SNRS;

private:
	int ExcelToListCtrl(CString strExcelFilePath);
	CString GetExcelDriver(void);
	int InsertRow(CString strSN, CString strMasterDate, CString strSlaveDate);
	int DataBaseConnection(CString strExcelFilePath);
	int DataBaseClose();
	int m_curRSIndex; // Current Recordset Index 
	BOOL m_statusMPort; // Master Port Status
	BOOL m_statusSPort; // Slave Port Status

	/* Display String Information */
	CString m_strMDate; // Master Write Date
	CString m_strSDate; // Slave Write Date
	CString m_strMSN; // Master Serial Number
	CString m_strSSN; // Slave Serial Number

	CString m_strMPort; // Master Serial Port
	CString m_strSport; // Slave Serial Port

	/* Write/Read Type */
	WR_Type m_WType;
	WR_Type m_RType;

public:
	afx_msg void OnNMCustomdrawListSn(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListSn(NMHDR *pNMHDR, LRESULT *pResult);
private:
	static int master_display_info(CString SerialNumber, CString WriteDate);
	static int slave_display_info(CString SerialNumber, CString WriteDate);
public:
	CEdit m_editMSN;
	CEdit m_editSSN;
	CStatic m_staticMDATE;
	CStatic m_staticSDATE;
	CStatic m_staticMPORT;
	CStatic m_staticSPORT;
	afx_msg void OnBnClickedWriteBtn();
	afx_msg void OnBnClickedReadBtn();
};
