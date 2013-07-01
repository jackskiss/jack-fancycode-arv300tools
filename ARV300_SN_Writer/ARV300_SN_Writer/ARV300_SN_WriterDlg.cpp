
// ARV300_SN_WriterDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300_SN_WriterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
#define PORT_STATUS_NOT_OPEN    "Not Open"

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CARV300_SN_WriterDlg 대화 상자




CARV300_SN_WriterDlg::CARV300_SN_WriterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CARV300_SN_WriterDlg::IDD, pParent)
	, strSNFileName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
END_MESSAGE_MAP()


// CARV300_SN_WriterDlg 메시지 처리기

BOOL CARV300_SN_WriterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_SNListCtrl.InsertColumn(0, _T("S/N"));
	m_SNListCtrl.InsertColumn(1, _T("Master Data"));
	m_SNListCtrl.InsertColumn(2, _T("Slave Data"));
	m_SNListCtrl.SetColumnWidth(0, 150);
	m_SNListCtrl.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_SNListCtrl.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_SNListCtrl.SetExtendedStyle(
								m_SNListCtrl.GetExtendedStyle() & (~LVS_EX_HEADERDRAGDROP) | (LVS_EX_MULTIWORKAREAS | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES));


	/* Set Default Vaule */
	m_WType = MS_BOTH;
	m_RType = MS_BOTH;

    m_statusMPort = FALSE; // Master Port Status
	m_statusSPort = FALSE; // Slave Port Status

	m_staticMPORT.SetWindowTextW(_T(PORT_STATUS_NOT_OPEN));
	m_staticSPORT.SetWindowTextW(_T(PORT_STATUS_NOT_OPEN));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CARV300_SN_WriterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CARV300_SN_WriterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CARV300_SN_WriterDlg::GetExcelDriver(void)
{
	TCHAR szBuf[2048]={0,};
	WORD cbBufMax = 2047;
	WORD cbBufOut;
	LPCTSTR pszBuf = szBuf;
	CString strExcelDriver;

	//설치된 드라이버의 이름을 구함 ("odbcinst.h"에 정의됨)
	if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut))
		return _T("");

	//드라이버를 구함
	do
	{
		if( _tcsstr(pszBuf, _T("Excel") ) != 0 )
		{
			//찾음
			strExcelDriver = CString( pszBuf );
			break;
		}
		pszBuf = _tcschr( pszBuf, _T('\0') ) + 1;
	}
	while( pszBuf[1] != _T('\0') );

	return strExcelDriver;
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

	strSN.ReleaseBuffer(strSN.GetLength());//메모리 해제

	//Master Write Date
	lvitem.iItem = iActItem;
	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 1;
	lvitem.pszText = strMasterDate.GetBuffer(strMasterDate.GetLength());
	m_SNListCtrl.SetItem( &lvitem );
	strMasterDate.ReleaseBuffer(strMasterDate.GetLength());//메모리 해제

	//Slave Write Date
	lvitem.iItem = iActItem;
	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 2;
	lvitem.pszText = strSlaveDate.GetBuffer(strSlaveDate.GetLength());
	m_SNListCtrl.SetItem( &lvitem );
	strSlaveDate.ReleaseBuffer(strSlaveDate.GetLength());//메모리 해제

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}

int CARV300_SN_WriterDlg::ExcelToListCtrl(CString strExcelFilePath)
{
	CDatabase database;
	CString strSql;
	CString strField;
	CString strExcelDriver;
	CString strDsn;
	CString strSheet = _T("Sheet1");
	
	if(strExcelFilePath.IsEmpty())
	{
		AfxMessageBox(_T("No file path"));
		return FALSE;
	}

	strExcelDriver = GetExcelDriver();
	if( strExcelDriver.IsEmpty() )
	{
		AfxMessageBox(_T("Fail to find out excel driver"));
		return FALSE;
	}

	strDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"),strExcelDriver,strExcelFilePath);

	TRY
	{
		if(database.Open(NULL,false,false,strDsn) == FALSE)
			return FALSE;

		CRecordset recset( &database );

		int nEmptyRow = 0;

		strSql.Format(_T("SELECT * FROM [%s$A1:IV65535]"), strSheet);//A1셀부터 IV:65535까지 읽음
//		recset.Open(CRecordset::forwardOnly,strSql,CRecordset::none);

		if(recset.Open(AFX_DB_USE_DEFAULT_TYPE, strSql, CRecordset::useExtendedFetch) == FALSE)
			return FALSE;


		int nFieldCount = recset.GetODBCFieldCount();//필드(열)의 개수
		int nRowCount =0;
		int nIndex = 0;
		CDBVariant m_DBVariant;
		
		recset.MoveFirst();
		
		//엑셀로 부터 데이터를 읽고 리스트컨트롤에 출력
		while( !recset.IsEOF() )
		{
			CString strFieldSum; //strFieldSum는 빈행이 있는지 확인하기 위해 사용하는 임시 자료형 임
			CString strSN, strMasterInfo, strSlaveInfo;
			
			recset.GetFieldValue((short)0, strSN);
			recset.GetFieldValue((short)1, strMasterInfo);
			recset.GetFieldValue((short)2, strSlaveInfo);

			
			if(strSN.Right(2) == _T(".0"))//끝 두자리가 .0으로 끝나는 경우 즉 엑셀에서 정수값을 실수형으로 읽어들이는 경우
				strSN = strSN.Left(strSN.GetLength() - 2);//.0을 삭제한 정수형으로 변환(3.0 -> 3)

			if(strMasterInfo.Right(2) == _T(".0"))//끝 두자리가 .0으로 끝나는 경우 즉 엑셀에서 정수값을 실수형으로 읽어들이는 경우
				strMasterInfo = strMasterInfo.Left(strMasterInfo.GetLength() - 2);//.0을 삭제한 정수형으로 변환(3.0 -> 3)

			if(strSlaveInfo.Right(2) == _T(".0"))//끝 두자리가 .0으로 끝나는 경우 즉 엑셀에서 정수값을 실수형으로 읽어들이는 경우
				strSlaveInfo = strSlaveInfo.Left(strSlaveInfo.GetLength() - 2);//.0을 삭제한 정수형으로 변환(3.0 -> 3)

			InsertRow(strSN, strMasterInfo,strSlaveInfo);

			recset.MoveNext();
		}

	}
	CATCH(CDBException, e)
	{
		database.Close();
		//예외 발생
		AfxMessageBox(_T("Database error: ")+e->m_strError);
		return FALSE;
	}
	END_CATCH;

	database.Close();
	return TRUE;
}

void CARV300_SN_WriterDlg::OnFileOpenSn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	char szFilter[] = "Serial Number Data (*.xls, *.xlsx) | *.xls;*.xlsx; | All Files(*.*)|*.*||";
	CString szFilter = _T("Serial Number Data (*.xls, *.xlsx) | *.xls;*.xlsx; | All Files(*.*)|*.*||");

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, (LPCTSTR)szFilter);
	if(IDOK == dlg.DoModal())
	{
			strSNFileName = dlg.GetPathName();

			if(!strSNFileName.IsEmpty())
				ExcelToListCtrl(strSNFileName);
	}
}


void CARV300_SN_WriterDlg::OnNMCustomdrawListSn(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
    
	switch( lplvcd->nmcd.dwDrawStage )
    {
    case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM :               
		if(0x2000 == m_SNListCtrl.GetItemState(pNMCD->dwItemSpec, LVIS_STATEIMAGEMASK))
			lplvcd->clrTextBk = RGB(204, 255, 255); //체크박스가 선택된 경우 배경이 민트
        else
			lplvcd->clrTextBk = RGB(255, 255, 255);  //아닌경우는 배경이 흰색  
        
		*pResult = CDRF_NEWFONT;
        return;
          break;
	}

	*pResult = 0;
}

int CARV300_SN_WriterDlg::DataBaseConnection(CString strExcelFilePath)
{
	CDatabase database;
	CString strSql;
	CString strField;
	CString strExcelDriver;
	CString strDsn;
	CString strSheet = _T("Sheet1");
	
	if(strExcelFilePath.IsEmpty())
	{
		AfxMessageBox(_T("No file path"));
		return FALSE;
	}

	strExcelDriver = GetExcelDriver();

	if( strExcelDriver.IsEmpty() )
	{
		AfxMessageBox(_T("Fail to find out excel driver"));
		return FALSE;
	}

	strDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"),strExcelDriver,strExcelFilePath);

    if(m_SNDB.Open(NULL,false,false,strDsn) == FALSE)
		return FALSE;

	m_SNRS.m_pDatabase = &m_SNDB;

	strSql.Format(_T("SELECT * FROM [%s$A1:IV65535]"), strSheet);//A1셀부터 IV:65535까지 읽음
//		recset.Open(CRecordset::forwardOnly,strSql,CRecordset::none);

	if(m_SNRS.Open(AFX_DB_USE_DEFAULT_TYPE, strSql, CRecordset::useExtendedFetch) == FALSE)
		return FALSE;

	return TRUE;
}

int CARV300_SN_WriterDlg::DataBaseClose()
{
	if(m_SNRS.IsOpen())
	{
		m_SNRS.Close();
	
		if(m_SNDB.IsOpen())
		{
			m_SNDB.Close();

		}
	}

	return 0;
}


void CARV300_SN_WriterDlg::OnLvnItemchangedListSn(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


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
	m_editMSN.SetWindowTextW((LPCTSTR)m_strMSN);
	m_editSSN.SetWindowTextW((LPCTSTR)m_strSSN);
	m_staticMDATE.SetWindowTextW((LPCTSTR)m_strMDate);
	m_staticSDATE.SetWindowTextW((LPCTSTR)m_strSDate);

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!(m_statusMPort || m_statusSPort) )
	{
		AfxMessageBox(IDS_ERROR_NO_PORT_TO_WRITE ,NULL,IDOK);
		return;
	}

	if(m_statusMPort && (m_WType == MS_BOTH || m_WType == M_ONLY))
	{
		// Master Write Sequence
	}

	if(m_statusMPort && (m_WType == MS_BOTH || m_WType == S_ONLY))
	{
		// Slave Write Sequence
	}
}


void CARV300_SN_WriterDlg::OnBnClickedReadBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	if(type == MASTER)
	{
		// Write Comport 
		
	} else if (type == SLAVE) {
	
	} else {
		ret = -(ARV300_ERROR_NO_MS_TYPE);
	}
	
	//	Write Procedure
	/* Write Command */
	/* Wait for response */
	/* If success to write, change excel file */
	if(DataBaseConnection(strSNFileName))
	{
		m_SNRS.Edit();
		m_SNRS.LoadFields();

		m_SNRS.LoadFields(
			Update();
	}
	else
		ret = -(ARV300_ERROR_DB_CONNECTION_FAIL);

	return ret;
}


int CARV300_SN_WriterDlg::SNRead(MS_TYPE type)
{
	return 0;
}
