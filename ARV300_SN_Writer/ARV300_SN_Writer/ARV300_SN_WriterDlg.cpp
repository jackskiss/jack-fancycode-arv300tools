
// ARV300_SN_WriterDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300_SN_WriterDlg.h"
#include "afxdialogex.h"
#include "cspreadsheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.
#define PORT_STATUS_NOT_OPEN    "Not Open"
#define ARV300_SPREEDSHEET_NAME "Sheet1"

#define ROWLENGHT_FROM_FIELDNAME_TO_FIRSTROW	2

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
	m_SNListCtrl.InsertColumn(1, _T("Master Date"));
	m_SNListCtrl.InsertColumn(2, _T("Slave Date"));
	m_SNListCtrl.SetColumnWidth(0, 100);
	m_SNListCtrl.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_SNListCtrl.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_SNListCtrl.SetExtendedStyle(
								m_SNListCtrl.GetExtendedStyle() & (~LVS_EX_HEADERDRAGDROP) | (LVS_EX_MULTIWORKAREAS | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES));


	/* Set Default Vaule */
	m_WType = MS_BOTH;
	m_RType = MS_BOTH;
	m_comboWType.SetCurSel(MS_BOTH);
	m_comboRType.SetCurSel(MS_BOTH);
//	m_comboWType.EnableWindow(FALSE);
//	m_comboRType.EnableWindow(FALSE);

    m_statusMPort = FALSE; // Master Port Status
	m_statusSPort = FALSE; // Slave Port Status
	ARV300_WRButtonEnable(FALSE);

	m_staticMPORT.SetWindowText(_T(PORT_STATUS_NOT_OPEN));
	m_staticSPORT.SetWindowText(_T(PORT_STATUS_NOT_OPEN));

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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//	char szFilter[] = "Serial Number Data (*.xls, *.xlsx) | *.xls;*.xlsx; | All Files(*.*)|*.*||";
	CString szFilter = _T("Serial Number Data (*.xls, *.xlsx) | *.xls;*.xlsx; | All Files(*.*)|*.*||");

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, (LPCTSTR)szFilter);
	if(IDOK == dlg.DoModal())
	{
			m_strSNFileName = dlg.GetPathName();

			if(!m_strSNFileName.IsEmpty())
				ExcelToListCtrl(m_strSNFileName);
	}
}


void CARV300_SN_WriterDlg::OnNMCustomdrawListSn(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
    
	switch( lplvcd->nmcd.dwDrawStage )
    {
    case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM :               
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

		*pResult = CDRF_NEWFONT;
        return;
          break;
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
	int ret;

	if(!(m_statusMPort || m_statusSPort) )
	{
		AfxMessageBox(IDS_ERROR_NO_PORT_TO_WRITE ,NULL,IDOK);
		return;
	}

	if(m_statusMPort && (m_WType == MS_BOTH || m_WType == M_ONLY))
	{
		if((ret = SNWrite(MASTER)) != ARV300_ERROR_NO_ERROR)
		{
			errstr.Format("Error: SNWrite %d", ret);
			AfxMessageBox(LPCTSTR(errstr));
		}
	}

	if(m_statusMPort && (m_WType == MS_BOTH || m_WType == S_ONLY))
	{
		if(SNWrite(SLAVE) != ARV300_ERROR_NO_ERROR)
		{
			errstr.Format("Error: SNWrite %d", ret);
			AfxMessageBox(LPCTSTR(errstr));
		}
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�
	m_WType = (WR_Type)m_comboWType.GetCurSel();
}


void CARV300_SN_WriterDlg::OnCbnSelchangeComboRtype()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_RType = (WR_Type)m_comboRType.GetCurSel();
}


void CARV300_SN_WriterDlg::OnFileOption()
{
	if(m_dlgARV300.DoModal())
	{
		// 
		m_statusMPort = TRUE;
		m_statusSPort = TRUE;
		ARV300_WRButtonEnable(m_statusMPort || m_statusSPort);
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_dlgAbout.DoModal();
}
