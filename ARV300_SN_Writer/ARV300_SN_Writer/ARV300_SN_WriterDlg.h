
// ARV300_SN_WriterDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"

// For Database
#include <afxdb.h>         // MFC ODBC database classes
#include <odbcinst.h>
#include "afxwin.h"
#include "ARV300_COMPort.h"
#include "ARV300About.h"
#include "ARV300_BusPopup.h"

enum ARV300_ERROR {
	ARV300_ERROR_NO_ERROR,
	ARV300_ERROR_NO_MS_TYPE,
	ARV300_ERROR_DB_CONNECTION_FAIL,
	ARV300_ERROR_DB_ADDROW,
	ARV300_ERROR_COMMIT_FILESAVE,
};

enum ARV300_DB_FIELD {
	ARV300_DB_FIELD_SN,
	ARV300_DB_FIELD_MASTER,
	ARV300_DB_FIELD_SLAVE,
}; 
typedef enum WR_Type {
	MS_BOTH,
	M_ONLY,
	S_ONLY
} WR_Type;

typedef enum MS_TYPE {
	MASTER,
	SLAVE
} MS_TYPE;

typedef enum CMD_Type {
	WRITE_CMD,
	READ_CMD
} CMD_Type;

typedef struct Thread_Info_Data_Type {
	CMD_Type cmd;
	MS_TYPE ms;
	unsigned int index;
	CString fname;
	CString time;
	CString port;
	HWND	hdlg;
} Thread_Info_Data_Type;

// CARV300_SN_WriterDlg 대화 상자
class CARV300_SN_WriterDlg : public CDialogEx //,public CSerialIO
{
// 생성입니다.
public:
	CARV300_SN_WriterDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ARV300_SN_WRITER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnEventOpen(BOOL bSuccess);
	virtual void OnEventClose(BOOL bSuccess);
	virtual void OnEventRead(char *inPacket,int inLength);
	virtual void OnEventWrite(int nWritten);

	afx_msg void OnFileOpenSn();
	CListCtrl m_SNListCtrl;
	CString m_strSNFileName;
	CARV300_COMPort m_dlgARV300;
	CARV300About m_dlgAbout;
	CARV300_BusPopup m_dlgPopup;

	Thread_Info_Data_Type m_data;

private:
	int ExcelToListCtrl(CString strExcelFilePath);
	int InsertRow(CString strSN, CString strMasterDate, CString strSlaveDate);
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
	afx_msg void OnCbnSelchangeComboWtype();
	afx_msg void OnCbnSelchangeComboRtype();
	afx_msg void OnBnClickedWriteBtn();
	afx_msg void OnBnClickedReadBtn();

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
	CComboBox m_comboWType;
	CComboBox m_comboRType;

private:
	int SNWrite(MS_TYPE type);
	int SNRead(MS_TYPE type);
public:
	afx_msg void OnFileOption();
private:
	bool ARV300_WRButtonEnable(bool Enable);
public:
	afx_msg void OnHelpAbout();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
