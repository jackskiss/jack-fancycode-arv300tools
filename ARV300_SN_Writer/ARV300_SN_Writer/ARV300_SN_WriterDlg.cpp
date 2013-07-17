
// ARV300_SN_WriterDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300_SN_WriterDlg.h"
#include "afxdialogex.h"
#include "cspreadsheet.h"
#include "SerialCtrl.h"
#include <conio.h>

#include "afxwin.h" // For Console

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.
#define PORT_STATUS_NOT_OPEN    "Not Open"
#define ARV300_SPREEDSHEET_NAME "Sheet1"

#define ROWLENGHT_FROM_FIELDNAME_TO_FIRSTROW	2

static UINT ARV300_PopUpThread(LPVOID lParam);
static UINT ARV300_MasterThread(LPVOID lParam);
static UINT ARV300_SlaveThread(LPVOID lParam);

BOOL WINAPI ConsoleHandler(DWORD CEvent);

static HANDLE m_hEventMasterFinished;
static HANDLE m_hEventSlaveFinished;
static HANDLE m_hPortOpen;
static HANDLE m_hWrite;
static HANDLE m_hRead;

static CCriticalSection cs;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CARV300_SN_WriterDlg ��ȭ ����




CARV300_SN_WriterDlg::CARV300_SN_WriterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CARV300_SN_WriterDlg::IDD, pParent)
	, m_strSNFileName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dlgPopup = NULL;
/*
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	*/
}

void CARV300_SN_WriterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SN, m_SNListCtrl);
	DDX_Control(pDX, IDC_MSN_EDIT, m_editMSN);
	DDX_Control(pDX, IDC_SSN_EDIT, m_editSSN);
	DDX_Control(pDX, IDC_MDATE_STATIC, m_staticMDATE);
	DDX_Control(pDX, IDC_SDATE_STATIC, m_staticSDATE);
	DDX_Control(pDX, IDC_MPORT_STATIC, m_staticMPORT);
	DDX_Control(pDX, IDC_SPORT_STATIC, m_staticSPORT);
	DDX_Control(pDX, IDC_COMBO_WTYPE, m_comboWType);
	DDX_Control(pDX, IDC_COMBO_RTYPE, m_comboRType);
}

BEGIN_MESSAGE_MAP(CARV300_SN_WriterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN_SN, &CARV300_SN_WriterDlg::OnFileOpenSn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SN, &CARV300_SN_WriterDlg::OnNMCustomdrawListSn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SN, &CARV300_SN_WriterDlg::OnLvnItemchangedListSn)
	ON_BN_CLICKED(IDC_WRITE_BTN, &CARV300_SN_WriterDlg::OnBnClickedWriteBtn)
	ON_BN_CLICKED(IDC_READ_BTN, &CARV300_SN_WriterDlg::OnBnClickedReadBtn)
	ON_CBN_SELCHANGE(IDC_COMBO_WTYPE, &CARV300_SN_WriterDlg::OnCbnSelchangeComboWtype)
	ON_CBN_SELCHANGE(IDC_COMBO_RTYPE, &CARV300_SN_WriterDlg::OnCbnSelchangeComboRtype)
	ON_COMMAND(ID_FILE_OPTION, &CARV300_SN_WriterDlg::OnFileOption)
	ON_COMMAND(ID_HELP_ABOUT, &CARV300_SN_WriterDlg::OnHelpAbout)
	ON_WM_TIMER()
	ON_COMMAND(ID_WIND_LOGWINDOW, &CARV300_SN_WriterDlg::OnWindLogwindow)
END_MESSAGE_MAP()


// CARV300_SN_WriterDlg �޽��� ó����

BOOL CARV300_SN_WriterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	m_SNListCtrl.InsertColumn(0, _T("S/N"));
	m_SNListCtrl.InsertColumn(1, _T("Master Date"));
	m_SNListCtrl.InsertColumn(2, _T("Slave Date"));
	m_SNListCtrl.SetColumnWidth(0, 130);
	m_SNListCtrl.SetColumnWidth(1, 130);
	m_SNListCtrl.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_SNListCtrl.SetExtendedStyle(
								m_SNListCtrl.GetExtendedStyle() & (~LVS_EX_HEADERDRAGDROP) | (LVS_EX_MULTIWORKAREAS | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES));


	/* Set Default Vaule */
	m_WType = MS_BOTH;
	m_RType = MS_BOTH;
	m_comboWType.SetCurSel(MS_BOTH);
	m_comboRType.SetCurSel(MS_BOTH);

    m_statusMPort = FALSE; // Master Port Status
	m_statusSPort = FALSE; // Slave Port Status
	ARV300_WRButtonEnable(FALSE);

	m_staticMPORT.SetWindowText(_T(PORT_STATUS_NOT_OPEN));
	m_staticSPORT.SetWindowText(_T(PORT_STATUS_NOT_OPEN));

	m_threadMasterData.cmd = (CMD_Type)NULL;
	m_threadMasterData.fname = _T("");
	m_threadMasterData.index = 0;
	m_threadMasterData.ms = MASTER;
	m_threadMasterData.parent = this;
	m_threadMasterData.port = _T("");
	m_threadMasterData.time = _T(""); 

	m_threadSlaveData.cmd = (CMD_Type)NULL;
	m_threadSlaveData.fname = _T("");
	m_threadSlaveData.index = 0;
	m_threadSlaveData.ms = SLAVE;
	m_threadSlaveData.parent = this;
	m_threadSlaveData.port = _T("");
	m_threadSlaveData.time = _T(""); 

	m_popupInfo.parent = this;
	m_popupInfo.self = NULL;
	m_popupInfo.wtype = (WR_Type)NULL;

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CARV300_SN_WriterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CARV300_SN_WriterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CARV300_SN_WriterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CARV300_SN_WriterDlg::InsertRow(CString strSN, CString strMasterDate, CString strSlaveDate)
{

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	LVITEM lvitem;

	 //S/N Number
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = m_SNListCtrl.GetItemCount();
	lvitem.iSubItem = 0;
	lvitem.pszText = strSN.GetBuffer(strSN.GetLength());
	int iActItem = m_SNListCtrl.InsertItem( &lvitem );

	strSN.ReleaseBuffer(strSN.GetLength());//�޸� ����

	//Master Write Date
	lvitem.iItem = iActItem;
	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 1;
	lvitem.pszText = strMasterDate.GetBuffer(strMasterDate.GetLength());
	m_SNListCtrl.SetItem( &lvitem );
	strMasterDate.ReleaseBuffer(strMasterDate.GetLength());//�޸� ����

	//Slave Write Date
	lvitem.iItem = iActItem;
	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 2;
	lvitem.pszText = strSlaveDate.GetBuffer(strSlaveDate.GetLength());
	m_SNListCtrl.SetItem( &lvitem );
	strSlaveDate.ReleaseBuffer(strSlaveDate.GetLength());//�޸� ����

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}

int CARV300_SN_WriterDlg::ExcelToListCtrl(CString strExcelFilePath)
{	
	if(strExcelFilePath.IsEmpty())
	{
		AfxMessageBox(_T("No file path"));
		return FALSE;
	}

	CSpreadSheet SS((LPCTSTR)strExcelFilePath, ARV300_SPREEDSHEET_NAME);

	long row_count = SS.GetTotalRows();
	
	CStringArray sa;
	CString strSN, strMasterInfo, strSlaveInfo;

	for(int i=2;i<row_count;i++)
	{
		SS.ReadRow(sa,i);
        strSN = sa.GetAt(0);
        strMasterInfo = sa.GetAt(1);
        strSlaveInfo = sa.GetAt(2);

			
		if(strSN.Right(2) == _T(".0"))//�� ���ڸ��� .0���� ������ ��� �� �������� �������� �Ǽ������� �о���̴� ���
			strSN = strSN.Left(strSN.GetLength() - 2);//.0�� ������ ���������� ��ȯ(3.0 -> 3)

		if(strMasterInfo.Right(2) == _T(".0"))//�� ���ڸ��� .0���� ������ ��� �� �������� �������� �Ǽ������� �о���̴� ���
			strMasterInfo = strMasterInfo.Left(strMasterInfo.GetLength() - 2);//.0�� ������ ���������� ��ȯ(3.0 -> 3)

		if(strSlaveInfo.Right(2) == _T(".0"))//�� ���ڸ��� .0���� ������ ��� �� �������� �������� �Ǽ������� �о���̴� ���
			strSlaveInfo = strSlaveInfo.Left(strSlaveInfo.GetLength() - 2);//.0�� ������ ���������� ��ȯ(3.0 -> 3)

		InsertRow(strSN, strMasterInfo,strSlaveInfo);
	}

	return TRUE;
}

void CARV300_SN_WriterDlg::OnFileOpenSn()
{
//	char szFilter[] = "Serial Number Data (*.xls, *.xlsx) | *.xls;*.xlsx; | All Files(*.*)|*.*||";
	CString szFilter = _T("Serial Number Data (*.xls, *.xlsx) | *.xls;*.xlsx; | All Files(*.*)|*.*||");

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, (LPCTSTR)szFilter);
	if(IDOK == dlg.DoModal())
	{
			m_strSNFileName = dlg.GetPathName();

			if(!m_strSNFileName.IsEmpty())
			{
				ExcelToListCtrl(m_strSNFileName);
			}
			else
			{
				m_strSNFileName = _T("");
			}
	}
}


void CARV300_SN_WriterDlg::OnNMCustomdrawListSn(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
    
	switch( pNMCD->dwDrawStage )
    {
    case CDDS_ITEMPREPAINT :               
//		if(0x2000 == m_SNListCtrl.GetItemState(pNMCD->dwItemSpec, LVIS_STATEIMAGEMASK))
//			lplvcd->clrTextBk = RGB(204, 255, 255); //üũ�ڽ��� ���õ� ��� ����� ��Ʈ
//        else
//			lplvcd->clrTextBk = RGB(255, 255, 255);  //�ƴѰ��� ����� ���  
		
		if(!m_SNListCtrl.GetItemText(m_curRSIndex, ARV300_DB_FIELD_MASTER).IsEmpty() && 
			!m_SNListCtrl.GetItemText(m_curRSIndex, ARV300_DB_FIELD_SLAVE).IsEmpty())		
			lplvcd->clrTextBk = RGB(204, 255, 255);
		else if(!m_SNListCtrl.GetItemText(m_curRSIndex, ARV300_DB_FIELD_SLAVE).IsEmpty())
			lplvcd->clrTextBk = RGB(204, 200, 255);
		else if(!m_SNListCtrl.GetItemText(m_curRSIndex, ARV300_DB_FIELD_MASTER).IsEmpty())
			lplvcd->clrTextBk = RGB(204, 200, 200);

//		*pResult = CDRF_NEWFONT;
		*pResult = CDRF_NOTIFYITEMDRAW;
        return;
        // break;
	}

	*pResult = 0;
}

void CARV300_SN_WriterDlg::OnLvnItemchangedListSn(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	// Read Current Row
	m_curRSIndex = pNMLV->iItem;
	
	// Read Data - Serial Number, Write Date
	m_strMSN = m_SNListCtrl.GetItemText(m_curRSIndex,0);
	m_strSSN = m_SNListCtrl.GetItemText(m_curRSIndex,0);
	m_strMDate = m_SNListCtrl.GetItemText(m_curRSIndex,1);
	m_strSDate = m_SNListCtrl.GetItemText(m_curRSIndex,2);
//	m_strMPort
//	m_strSPort

	// Display Information
	m_editMSN.SetWindowText((LPCTSTR)m_strMSN);
	m_editSSN.SetWindowText((LPCTSTR)m_strSSN);
	m_staticMDATE.SetWindowText((LPCTSTR)m_strMDate);
	m_staticSDATE.SetWindowText((LPCTSTR)m_strSDate);

	*pResult = 0;
}


int CARV300_SN_WriterDlg::master_display_info(CString SerialNumber, CString WriteDate)
{

	return 0;
}


int CARV300_SN_WriterDlg::slave_display_info(CString SerialNumber, CString WriteDate)
{
	return 0;
}


void CARV300_SN_WriterDlg::OnBnClickedWriteBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString errstr;
	CTime curtime = CTime::GetCurrentTime();
	CString strtime = curtime.Format("%c");
	int ret;

	if(!(m_statusMPort || m_statusSPort) )
	{
		AfxMessageBox(IDS_ERROR_NO_PORT_TO_WRITE ,NULL,IDOK);
		return;
	}

	m_popupInfo.wtype = m_WType;
	m_popupInfo.parent = this;
	m_popupInfo.self = NULL;

	AfxBeginThread(ARV300_PopUpThread, &m_popupInfo);//, THREAD_PRIORITY_HIGHEST);

	if(m_statusMPort && (m_WType == MS_BOTH || m_WType == M_ONLY))
	{
		m_threadMasterData.cmd = WRITE_CMD;
		m_threadMasterData.fname = (LPCTSTR)m_strSNFileName;
		m_threadMasterData.index = m_curRSIndex;
		m_threadMasterData.wr = m_WType;
		m_threadMasterData.ms = MASTER;
		m_threadMasterData.parent = this;
		m_threadMasterData.port = (LPCTSTR)m_strMPort;
		m_threadMasterData.time = (LPCTSTR)strtime; 
		AfxBeginThread(ARV300_MasterThread, &m_threadMasterData); 		
	}

	if(m_statusMPort && (m_WType == MS_BOTH || m_WType == S_ONLY))
	{
		m_threadSlaveData.cmd = WRITE_CMD;
		m_threadSlaveData.fname = (LPCTSTR)m_strSNFileName;
		m_threadSlaveData.index = m_curRSIndex;
		m_threadSlaveData.wr = m_WType;
		m_threadSlaveData.ms = SLAVE;
		m_threadSlaveData.parent = this;
		m_threadSlaveData.port = (LPCTSTR)m_strSPort;
		m_threadSlaveData.time = (LPCTSTR)strtime; 
		AfxBeginThread(ARV300_SlaveThread, &m_threadSlaveData); 		
	}
}


void CARV300_SN_WriterDlg::OnBnClickedReadBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!(m_statusMPort || m_statusSPort) )
	{
		AfxMessageBox(IDS_ERROR_NO_PORT_TO_READ ,NULL,IDOK);
		return;
	}

	if(m_statusMPort && (m_RType == MS_BOTH || m_RType == M_ONLY))
	{
		// Master Read Sequence
	}

	if(m_statusMPort && (m_WType == MS_BOTH || m_RType == S_ONLY))
	{
		// Slave Read Sequence
	}
}

int CARV300_SN_WriterDlg::SNWrite(MS_TYPE type)
{
	int ret = ARV300_ERROR_NO_ERROR;

	if(type == MASTER || type == SLAVE)
	{

/*************************** START DATABASE ******************************/

		if(m_strSNFileName.IsEmpty()) // DATABASE NOT OPENED
			return ARV300_ERROR_NO_ERROR;

		CSpreadSheet SS((LPCTSTR)m_strSNFileName,ARV300_SPREEDSHEET_NAME);
		CStringArray old_sa, new_sa;

		SS.ReadRow(old_sa, ROWLENGHT_FROM_FIELDNAME_TO_FIRSTROW+m_curRSIndex);

		CTime curtime = CTime::GetCurrentTime();
		CString strtime = curtime.Format("%c");

		new_sa.Add(old_sa.GetAt(ARV300_DB_FIELD_SN));
		if(type == MASTER)
		{
			new_sa.Add((LPCTSTR)strtime);
			new_sa.Add(old_sa.GetAt(ARV300_DB_FIELD_MASTER));
		} else if (type == SLAVE) {
			new_sa.Add(old_sa.GetAt(ARV300_DB_FIELD_SLAVE));
			new_sa.Add((LPCTSTR)strtime);
		}

		if(SS.AddRow(new_sa, ROWLENGHT_FROM_FIELDNAME_TO_FIRSTROW+m_curRSIndex, true))
		{
			if(SS.Commit())
			{
				if(type == MASTER)
				{
					m_SNListCtrl.SetItemText(m_curRSIndex, ARV300_DB_FIELD_MASTER, (LPCTSTR)strtime);
					m_staticMDATE.SetWindowTextA((LPCTSTR)strtime);
				}
				else
				{
					m_SNListCtrl.SetItemText(m_curRSIndex, ARV300_DB_FIELD_SLAVE, (LPCTSTR)strtime);
					m_staticSDATE.SetWindowTextA((LPCTSTR)strtime);
				}
				ret = ARV300_ERROR_NO_ERROR;
			}
			else
			{
				ret = -(ARV300_ERROR_COMMIT_FILESAVE);
			}
		} else {
			ret = -(ARV300_ERROR_DB_ADDROW);
		}
/*************************** END DATABASE ******************************/

	} else {
		ret = -(ARV300_ERROR_NO_MS_TYPE);
	}
	return ret;
}


int CARV300_SN_WriterDlg::SNRead(MS_TYPE type)
{
	return 0;
}


void CARV300_SN_WriterDlg::OnCbnSelchangeComboWtype()
{
	m_WType = (WR_Type)m_comboWType.GetCurSel();
}


void CARV300_SN_WriterDlg::OnCbnSelchangeComboRtype()
{
	m_RType = (WR_Type)m_comboRType.GetCurSel();
}


void CARV300_SN_WriterDlg::OnFileOption()
{
	if((m_dlgARV300.DoModal() == IDOK) && (m_dlgARV300.m_statusMPort || m_dlgARV300.m_statusSPort))
	{
		// 
		m_statusMPort = m_dlgARV300.m_statusMPort;
		m_statusSPort = m_dlgARV300.m_statusSPort;

		ARV300_WRButtonEnable(m_statusMPort || m_statusSPort);
		if(m_statusMPort)
			m_staticMPORT.SetWindowTextA((LPCTSTR)m_dlgARV300.m_strMPort);
		
		if(m_statusSPort)
			m_staticSPORT.SetWindowTextA((LPCTSTR)m_dlgARV300.m_strSPort);
	}
}


bool CARV300_SN_WriterDlg::ARV300_WRButtonEnable(bool Enable)
{
	GetDlgItem(IDC_WRITE_BTN)->EnableWindow(Enable);
	GetDlgItem(IDC_READ_BTN)->EnableWindow(Enable);

	return false;
}


void CARV300_SN_WriterDlg::OnHelpAbout()
{
	m_dlgAbout.DoModal();
}


//static HANDLE m_hPortOpen;
//static HANDLE m_hWrite;
//static HANDLE m_hRead;

static int SN_Write(HWND pophandle, Thread_Info_Data_Type *data)
{
    CARV300_SN_WriterDlg *parent = (CARV300_SN_WriterDlg *)data->parent;
	int ret = ARV300_ERROR_NO_ERROR;
	DWORD retcom;

	if(data->ms == MASTER || data->ms == SLAVE)
	{
		CSpreadSheet SS((LPCTSTR)data->fname,ARV300_SPREEDSHEET_NAME);
		CStringArray old_sa, new_sa;

//------ WRITE DEVICE -------------------------------------// 
		// Port Open
		m_hEventMasterFinished = CreateEvent(NULL, FALSE, FALSE, NULL); // auto reset, initially reset
		parent->OpenPort(data->port, _T("115200"));
		
//		WAIT_FAILED, WAIT_ABANDONED,WAIT_OBJECT_0, WAIT_TIMEOUT
		if(WaitForSingleObject(m_hPortOpen, INFINITE) != WAIT_OBJECT_0)
		{
			if(m_hPortOpen)
				CloseHandle(m_hPortOpen);
		
			return -(ARV300_ERROR_COMPORT_OPEN_FAILURE);
		}

		if(m_hPortOpen)
			CloseHandle(m_hPortOpen);

	//	WaitForSingleObject(

		// Port Status Check


		// Write Port


		// Check Write Value
//------ WRITE END ----------------------------------------//

		SS.ReadRow(old_sa, ROWLENGHT_FROM_FIELDNAME_TO_FIRSTROW+data->index);

		new_sa.Add(old_sa.GetAt(ARV300_DB_FIELD_SN));
		if(data->ms == MASTER)
		{
			new_sa.Add((LPCTSTR)data->time);
			new_sa.Add(old_sa.GetAt(ARV300_DB_FIELD_MASTER));
		} else if (data->ms == SLAVE) {
			new_sa.Add(old_sa.GetAt(ARV300_DB_FIELD_SLAVE));
			new_sa.Add((LPCTSTR)data->time);
		}

		if(SS.AddRow(new_sa, ROWLENGHT_FROM_FIELDNAME_TO_FIRSTROW+data->index, true))
		{
			if(SS.Commit())
			{
				ret = ARV300_ERROR_NO_ERROR;
			}
			else
			{
				ret = -(ARV300_ERROR_COMMIT_FILESAVE);
			}
		} else {
			ret = -(ARV300_ERROR_DB_ADDROW);
		}
	} else {
		ret = -(ARV300_ERROR_NO_MS_TYPE);
	}

	return ret;
//	::SendMessageA(pophandle, ARV300_USER_MESSAGE_POPUPEXIT, NULL, NULL);  
}

static void SN_Read(HWND pophandle, Thread_Info_Data_Type *data)
{

}

/* Thread to control popup window */
UINT ARV300_PopUpThread(LPVOID lParam)
{
	PopUp_Status_Info_Type *info = (PopUp_Status_Info_Type *)lParam;

	cs.Lock();
	CARV300_BusPopup popup(info->parent);
	popup.DoModal();

	cs.Unlock();

	return 0;

}


static UINT ARV300_MasterThread(LPVOID lParam)
{
	Thread_Info_Data_Type *info = (Thread_Info_Data_Type *)lParam;
	CARV300_SN_WriterDlg *parent = (CARV300_SN_WriterDlg *)info->parent;

	m_hEventMasterFinished = CreateEvent(NULL, FALSE, FALSE, NULL); // auto reset, initially reset

	if(info->cmd == WRITE_CMD)
	{
		if(SN_Write(parent->GetSafeHwnd(), info) == ARV300_ERROR_NO_ERROR)
		{
			parent->m_SNListCtrl.SetItemText(info->index, ARV300_DB_FIELD_MASTER, (LPCTSTR)info->time);
			parent->m_staticMDATE.SetWindowTextA((LPCTSTR)info->time);
		}
	}
	else // READ_CMD
	{

	}

	
	if(info->wr == MS_BOTH)
		SetEvent(m_hEventMasterFinished);
	else
	{
		if(parent->m_popupInfo.self)
			::SendMessageA(parent->m_popupInfo.self, ARV300_USER_MESSAGE_POPUPEXIT, NULL, NULL);  
	}

	return 0;
}

static UINT ARV300_SlaveThread(LPVOID lParam)
{
	Thread_Info_Data_Type *info = (Thread_Info_Data_Type *)lParam;
	CARV300_SN_WriterDlg *parent = (CARV300_SN_WriterDlg *)info->parent;

	if(info->wr != M_ONLY)
	{
		if(info->wr == MS_BOTH)
		{
			WaitForSingleObject(m_hEventMasterFinished, INFINITE);
		}

		if(info->cmd == WRITE_CMD)
		{
			if(SN_Write(parent->GetSafeHwnd(), info) == ARV300_ERROR_NO_ERROR)
			{
				parent->m_SNListCtrl.SetItemText(info->index, ARV300_DB_FIELD_SLAVE, (LPCTSTR)info->time);
				parent->m_staticSDATE.SetWindowTextA((LPCTSTR)info->time);
			}
		}
		else // READ_CMD
		{

		}
	}

	if(m_hEventMasterFinished)
	{
		CloseHandle(m_hEventMasterFinished);
		m_hEventMasterFinished = NULL;
	}

	if(parent->m_popupInfo.self)
		::SendMessageA(parent->m_popupInfo.self, ARV300_USER_MESSAGE_POPUPEXIT, NULL, NULL);  

	return 0;
}

void CARV300_SN_WriterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// Send Message 

	CDialogEx::OnTimer(nIDEvent);
}

void CARV300_SN_WriterDlg::OnEventOpen(BOOL bSuccess)
{
	CString str;
	if(bSuccess)
	{
//		str=m_strPortName+" open successfully";

//		bPortOpened=TRUE;
//		m_btnOpen.SetWindowText("Close");

	}
	else
	{
//		str=m_strPortName+" open failed";
	}
//	m_staticInfo.SetWindowText(str);

	SetEvent(m_hPortOpen);
}

void CARV300_SN_WriterDlg::OnEventClose(BOOL bSuccess)
{
	CString str;
	if(bSuccess)
	{
//		str=m_strPortName+" close successfully";
//		bPortOpened=FALSE;
//		m_btnOpen.SetWindowText("Open");

	}
	else
	{
//		str=m_strPortName+" close failed";
	}
//	m_staticInfo.SetWindowText(str);
}

void CARV300_SN_WriterDlg::OnEventRead(char *inPacket,int inLength)
{
	
//	m_listboxRead.AddString(inPacket);

//	CString str;
//	str.Format("%d bytes read",inLength);

//	m_staticInfo.SetWindowText(str);
	
}
void CARV300_SN_WriterDlg::OnEventWrite(int nWritten)
{
	if(nWritten>0)
	{
		CString str;
		str.Format("%d bytes written",nWritten);
//		m_staticInfo.SetWindowText(str);
	}
	else
	{
//		m_staticInfo.SetWindowText("Write failed");
	}

}


BOOL CARV300_SN_WriterDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (!FreeConsole())
		AfxMessageBox("Could not free the console!");

	if(m_dlgPopup)
	{
		m_dlgPopup->DestroyWindow();
		delete m_dlgPopup;
	}
	return CDialogEx::DestroyWindow();
}


void CARV300_SN_WriterDlg::OnWindLogwindow()
{
	if (!AllocConsole())
		AfxMessageBox("Failed to create the console!", MB_ICONEXCLAMATION);
	else 
	{
		if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler,TRUE)==FALSE)
		{
    // unable to install handler... 
    // display message to the user
				printf("Unable to install handler!\n");
				return;
		}

//		AttachConsole( GetCurrentProcessId() ) ;
	}
	_cprintf("Test Message\n");

/*
HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
 
::SetConsoleTitle("���");
HWND hwnd = ::FindWindow(NULL, "���");
RemoveMenu(::GetSystemMenu(hwnd, FALSE),  SC_CLOSE, MF_BYCOMMAND);
*/
}


BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
    char mesg[128];

    switch(CEvent)
    {
    case CTRL_C_EVENT:
//        MessageBox(NULL,
//            "CTRL+C received!","CEvent",MB_OK);
//        break;
    case CTRL_BREAK_EVENT:
 //       MessageBox(NULL,
 //           "CTRL+BREAK received!","CEvent",MB_OK);
 //       break;
    case CTRL_CLOSE_EVENT:
//		FreeConsole();
//		ExitProcess(1);
 //       MessageBox(NULL,
 //           "Program being closed!","CEvent",MB_OK);
//		break;
    case CTRL_LOGOFF_EVENT:
 //       MessageBox(NULL,
 //           "User is logging off!","CEvent",MB_OK);
 //       break;
    case CTRL_SHUTDOWN_EVENT:
		break;

    }
    return TRUE;
}