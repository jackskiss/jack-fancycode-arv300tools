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
	m_statusMPort = FALSE;
	m_statusSPort = FALSE;
	

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}


void CARV300_COMPort::comportlist(void)
{
     HKEY h_CommKey;
     LONG Reg_Ret;
     DWORD Size = MAX_PATH;
     char i_str[MAX_PATH];

     Reg_Ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_READ | KEY_QUERY_VALUE, &h_CommKey);   
        //������Ʈ��..

     if(Reg_Ret == ERROR_SUCCESS)
     {
          for(int i = 0; Reg_Ret == ERROR_SUCCESS; i++)
          {
               Reg_Ret = RegEnumValue(h_CommKey, i, i_str, &Size, NULL, NULL, NULL, NULL);
               if(Reg_Ret == ERROR_SUCCESS)
               {
                    DWORD dwType, dwSize = MAX_PATH;
                    char szBuffer[MAX_PATH];
    
                    RegQueryValueEx(h_CommKey, i_str, 0, &dwType, (LPBYTE)szBuffer, &dwSize);

                    m_comboMPort.AddString(szBuffer);  // ����Ʈ �ڽ��� ������Ʈ�� ���� �߰�(���⼭�� COM PORT)
                    m_comboSPort.AddString(szBuffer);  // ����Ʈ �ڽ��� ������Ʈ�� ���� �߰�(���⼭�� COM PORT)
                            //���� ������Ʈ�� ���� ���� �� ����. 
               }
               Size = MAX_PATH;
          }
     }
     RegCloseKey(h_CommKey); 
}


void CARV300_COMPort::OnCbnSelchangeComboMport()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_comboMPort.GetWindowTextA(m_strMPort);
	m_statusMPort = (m_comboMPort.GetCurSel() != (m_comboMPort.GetCount() - 1))?TRUE:FALSE;
}


void CARV300_COMPort::OnCbnSelchangeComboSport()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_comboSPort.GetWindowTextA(m_strSPort);
	m_statusSPort = (m_comboSPort.GetCurSel() != (m_comboSPort.GetCount() - 1))?TRUE:FALSE;
}
