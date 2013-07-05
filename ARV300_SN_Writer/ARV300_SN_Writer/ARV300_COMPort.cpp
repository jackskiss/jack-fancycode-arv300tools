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

	m_strMPort = (LPCTSTR)strCOM;
	m_strSPort = (LPCTSTR)strCOM;

	m_comboMPort.SetCurSel(m_comboMPort.GetCount()-1);
	m_comboSPort.SetCurSel(m_comboMPort.GetCount()-1);

	

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


void CARV300_COMPort::comportlist(void)
{

    CString m_strComPort;
 
    HKEY hKey;
    LONG Result;
    LPCTSTR lpszSubKey="HARDWARE\\DEVICEMAP\\SERIALCOMM";
 
    char lpValue[10];
    DWORD dwType, dwByte;
    
    Result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,lpszSubKey,0,KEY_ALL_ACCESS,&hKey);
 
    if(Result == ERROR_SUCCESS)
    {
        int i = 0;
        while (!Result)
        {
            CString strSubKey;
            strSubKey.Format("\\device\\serial%d", i);
 
            Result = RegQueryValueEx(hKey, strSubKey, NULL, &dwType, (unsigned char*)lpValue, &dwByte);
            if( Result == ERROR_SUCCESS )
            {
                m_strComPort = lpValue;
 				m_comboMPort.AddString((LPCTSTR)m_strComPort);
				m_comboSPort.AddString((LPCTSTR)m_strComPort);               
            }
            else
            {
                m_strComPort = "";
            }
            i++;
        }
    }
    RegCloseKey(hKey);
}


void CARV300_COMPort::OnCbnSelchangeComboMport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_comboMPort.GetWindowTextA(m_strMPort);
}


void CARV300_COMPort::OnCbnSelchangeComboSport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_comboSPort.GetWindowTextA(m_strSPort);
}
