
// ARV300_SN_WriterDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300_SN_WriterDlg.h"
#include "afxdialogex.h"

// For Database
#include <afxdb.h>         // MFC ODBC database classes
#include <odbcinst.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CARV300_SN_WriterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SN, m_SNListCtrl);
}

BEGIN_MESSAGE_MAP(CARV300_SN_WriterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, &CARV300_SN_WriterDlg::OnFileOpen)
	ON_BN_CLICKED(IDC_BUTTON1, &CARV300_SN_WriterDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBOREAD, &CARV300_SN_WriterDlg::OnCbnSelchangeComboread)
END_MESSAGE_MAP()


// CARV300_SN_WriterDlg �޽��� ó����

BOOL CARV300_SN_WriterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ���� ������ �־�� �մϴ�.
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
	m_SNListCtrl.SetExtendedStyle(
								m_SNListCtrl.GetExtendedStyle() & (~LVS_EX_HEADERDRAGDROP) | (LVS_EX_MULTIWORKAREAS | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES));

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



void CARV300_SN_WriterDlg::OnFileOpen()
{
	char szFilter[] = "Serial Number Data (*.xls, *.xlsx) | *.xls;*.xlsx; | All Files(*.*)|*.*||";
	
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if(IDOK == dlg.DoModal())
	{
			strSNFileName = dlg.GetPathName();

			if(!strSNFileName.IsEmpty())
				ExcelToListCtrl(strSNFileName);
	}
}


void CARV300_SN_WriterDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CARV300_SN_WriterDlg::OnCbnSelchangeComboread()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


int CARV300_SN_WriterDlg::ExcelToListCtrl(CString strExcelFilePath)
{
	CDatabase database;
	CString strSql;
	CString strField;
	CString strExcelDriver;
	CString strDsn;
	CString strSheet = "";
	
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

			
//			if(strFruitPrice.Right(2) == _T(".0"))//�� ���ڸ��� .0���� ������ ��� �� �������� �������� �Ǽ������� �о���̴� ���
//				strFruitPrice = strFruitPrice.Left(strFruitPrice.GetLength() - 2);//.0�� ������ ���������� ��ȯ(3.0 -> 3)

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
	return 0;
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