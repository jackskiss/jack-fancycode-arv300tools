// ARV300_COMPort.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ARV300_SN_Writer.h"
#include "ARV300_COMPort.h"
#include "afxdialogex.h"


// CARV300_COMPort ��ȭ �����Դϴ�.

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


// CARV300_COMPort �޽��� ó�����Դϴ�.
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

	

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_comboMPort.GetWindowTextA(m_strMPort);
}


void CARV300_COMPort::OnCbnSelchangeComboSport()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_comboSPort.GetWindowTextA(m_strSPort);
}
