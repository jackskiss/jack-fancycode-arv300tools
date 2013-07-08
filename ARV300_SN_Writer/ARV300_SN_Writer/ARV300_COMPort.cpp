// ARV300_COMPort.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300_COMPort.h"
#include "afxdialogex.h"


// CARV300_COMPort 대화 상자입니다.

IMPLEMENT_DYNAMIC(CARV300_COMPort, CDialogEx)

CARV300_COMPort::CARV300_COMPort(CWnd* pParent /*=NULL*/)
	: CDialogEx(CARV300_COMPort::IDD, pParent)
{
	m_strMPort = _T("");
	m_strSPort = _T("");
	m_statusMPort = FALSE;
	m_statusSPort = FALSE;
}

CARV300_COMPort::~CARV300_COMPort()
{

}

void CARV300_COMPort::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MPORT, m_comboMPort);
	DDX_Control(pDX, IDC_COMBO_SPORT, m_comboSPort);
}


BEGIN_MESSAGE_MAP(CARV300_COMPort, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_MPORT, &CARV300_COMPort::OnCbnSelchangeComboMport)
	ON_CBN_SELCHANGE(IDC_COMBO_SPORT, &CARV300_COMPort::OnCbnSelchangeComboSport)
END_MESSAGE_MAP()


// CARV300_COMPort 메시지 처리기입니다.
BOOL CARV300_COMPort::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strCOM;
/*
	for(int i=1; i <256; i++)
	{
		strCOM.Format("COM%d",i);
		m_comboMPort.AddString((LPCTSTR)strCOM);
		m_comboSPort.AddString((LPCTSTR)strCOM);
	}
*/
	comportlist();

	strCOM.LoadStringA(IDS_NOT_SELECTED_PORT);
	m_comboMPort.AddString((LPCTSTR)strCOM);
	m_comboSPort.AddString((LPCTSTR)strCOM);

	int curidx = m_comboMPort.FindString(0,(LPCTSTR)m_strMPort);

	if(m_strMPort.IsEmpty() || (curidx < 0))
	{
		m_strMPort = (LPCTSTR)strCOM;
		m_comboMPort.SetCurSel(m_comboMPort.GetCount()-1);
		m_statusMPort = FALSE;
	}
	else
	{
		m_comboMPort.SetCurSel(curidx);
		m_statusMPort = TRUE;
	}
	
	curidx = m_comboSPort.FindString(0,(LPCTSTR)m_strSPort);

	if(m_strSPort.IsEmpty() || (curidx < 0))
	{
		m_strSPort = (LPCTSTR)strCOM;
		m_comboSPort.SetCurSel(m_comboSPort.GetCount()-1);
		m_statusSPort = FALSE;
	}
	else
	{
		m_comboSPort.SetCurSel(curidx);
		m_statusSPort = TRUE;
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


void CARV300_COMPort::comportlist(void)
{
    HKEY  hSerialCom;
    TCHAR buffer[_MAX_PATH], data[_MAX_PATH];
    DWORD len, type, dataSize;
    long  i;
    
    if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                       _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 
                       0, 
//                       KEY_ALL_ACCESS, 
                       KEY_QUERY_VALUE, 
                       &hSerialCom) == ERROR_SUCCESS)
    {
        for (i=0, len=dataSize=_MAX_PATH; 
            ::RegEnumValue(hSerialCom, 
                           i, 
                           buffer, 
                           &len, 
                           NULL, 
                           &type, 
                           (unsigned char*)data,
                           &dataSize)==ERROR_SUCCESS; i++, len=dataSize=_MAX_PATH)
        {
                data[dataSize-1] = NULL;
                if (strncmp(data, "COM", 3) == 0)
				{
					m_comboMPort.AddString((LPCTSTR)data);
					m_comboSPort.AddString((LPCTSTR)data);
				}
                    
        }
		
        ::RegCloseKey(hSerialCom);
    }
}


void CARV300_COMPort::OnCbnSelchangeComboMport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_comboMPort.GetCurSel() < (m_comboMPort.GetCount()-1))
	{
		m_comboMPort.GetWindowTextA(m_strMPort);
		m_statusMPort = TRUE;
	}
	else
		m_statusMPort = FALSE;
}


void CARV300_COMPort::OnCbnSelchangeComboSport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_comboMPort.GetCurSel() < (m_comboMPort.GetCount()-1))
	{
		m_comboSPort.GetWindowTextA(m_strSPort);
		m_statusSPort = TRUE;
	}
	else
		m_statusSPort = FALSE;
}
