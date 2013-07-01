
// ARV300_SN_WriterDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300_SN_WriterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.
#define PORT_STATUS_NOT_OPEN    "Not Open"

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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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

CString CARV300_SN_WriterDlg::GetExcelDriver(void)
{
	TCHAR szBuf[2048]={0,};
	WORD cbBufMax = 2047;
	WORD cbBufOut;
	LPCTSTR pszBuf = szBuf;
	CString strExcelDriver;

	//��ġ�� ����̹��� �̸��� ���� ("odbcinst.h"�� ���ǵ�)
	if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut))
		return _T("");

	//����̹��� ����
	do
	{
		if( _tcsstr(pszBuf, _T("Excel") ) != 0 )
		{
			//ã��
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

		strSql.Format(_T("SELECT * FROM [%s$A1:IV65535]"), strSheet);//A1������ IV:65535���� ����
//		recset.Open(CRecordset::forwardOnly,strSql,CRecordset::none);

		if(recset.Open(AFX_DB_USE_DEFAULT_TYPE, strSql, CRecordset::useExtendedFetch) == FALSE)
			return FALSE;


		int nFieldCount = recset.GetODBCFieldCount();//�ʵ�(��)�� ����
		int nRowCount =0;
		int nIndex = 0;
		CDBVariant m_DBVariant;
		
		recset.MoveFirst();
		
		//������ ���� �����͸� �а� ����Ʈ��Ʈ�ѿ� ���
		while( !recset.IsEOF() )
		{
			CString strFieldSum; //strFieldSum�� ������ �ִ��� Ȯ���ϱ� ���� ����ϴ� �ӽ� �ڷ��� ��
			CString strSN, strMasterInfo, strSlaveInfo;
			
			recset.GetFieldValue((short)0, strSN);
			recset.GetFieldValue((short)1, strMasterInfo);
			recset.GetFieldValue((short)2, strSlaveInfo);

			
			if(strSN.Right(2) == _T(".0"))//�� ���ڸ��� .0���� ������ ��� �� �������� �������� �Ǽ������� �о���̴� ���
				strSN = strSN.Left(strSN.GetLength() - 2);//.0�� ������ ���������� ��ȯ(3.0 -> 3)

			if(strMasterInfo.Right(2) == _T(".0"))//�� ���ڸ��� .0���� ������ ��� �� �������� �������� �Ǽ������� �о���̴� ���
				strMasterInfo = strMasterInfo.Left(strMasterInfo.GetLength() - 2);//.0�� ������ ���������� ��ȯ(3.0 -> 3)

			if(strSlaveInfo.Right(2) == _T(".0"))//�� ���ڸ��� .0���� ������ ��� �� �������� �������� �Ǽ������� �о���̴� ���
				strSlaveInfo = strSlaveInfo.Left(strSlaveInfo.GetLength() - 2);//.0�� ������ ���������� ��ȯ(3.0 -> 3)

			InsertRow(strSN, strMasterInfo,strSlaveInfo);

			recset.MoveNext();
		}

	}
	CATCH(CDBException, e)
	{
		database.Close();
		//���� �߻�
		AfxMessageBox(_T("Database error: ")+e->m_strError);
		return FALSE;
	}
	END_CATCH;

	database.Close();
	return TRUE;
}

void CARV300_SN_WriterDlg::OnFileOpenSn()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
			lplvcd->clrTextBk = RGB(204, 255, 255); //üũ�ڽ��� ���õ� ��� ����� ��Ʈ
        else
			lplvcd->clrTextBk = RGB(255, 255, 255);  //�ƴѰ��� ����� ���  
        
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

	strSql.Format(_T("SELECT * FROM [%s$A1:IV65535]"), strSheet);//A1������ IV:65535���� ����
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
