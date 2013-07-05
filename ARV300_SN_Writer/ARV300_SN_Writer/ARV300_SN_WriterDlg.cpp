
// ARV300_SN_WriterDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300_SN_WriterDlg.h"
#include "afxdialogex.h"
#include "cspreadsheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
#define PORT_STATUS_NOT_OPEN    "Not Open"
#define ARV300_SPREEDSHEET_NAME "Sheet1"

#define ROWLENGHT_FROM_FIELDNAME_TO_FIRSTROW	2

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

			
		if(strSN.Right(2) == _T(".0"))//끝 두자리가 .0으로 끝나는 경우 즉 엑셀에서 정수값을 실수형으로 읽어들이는 경우
			strSN = strSN.Left(strSN.GetLength() - 2);//.0을 삭제한 정수형으로 변환(3.0 -> 3)

		if(strMasterInfo.Right(2) == _T(".0"))//끝 두자리가 .0으로 끝나는 경우 즉 엑셀에서 정수값을 실수형으로 읽어들이는 경우
			strMasterInfo = strMasterInfo.Left(strMasterInfo.GetLength() - 2);//.0을 삭제한 정수형으로 변환(3.0 -> 3)

		if(strSlaveInfo.Right(2) == _T(".0"))//끝 두자리가 .0으로 끝나는 경우 즉 엑셀에서 정수값을 실수형으로 읽어들이는 경우
			strSlaveInfo = strSlaveInfo.Left(strSlaveInfo.GetLength() - 2);//.0을 삭제한 정수형으로 변환(3.0 -> 3)

		InsertRow(strSN, strMasterInfo,strSlaveInfo);
	}

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
//			lplvcd->clrTextBk = RGB(204, 255, 255); //체크박스가 선택된 경우 배경이 민트
//        else
//			lplvcd->clrTextBk = RGB(255, 255, 255);  //아닌경우는 배경이 흰색  
		
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다
	m_WType = (WR_Type)m_comboWType.GetCurSel();
}


void CARV300_SN_WriterDlg::OnCbnSelchangeComboRtype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_dlgAbout.DoModal();
}
