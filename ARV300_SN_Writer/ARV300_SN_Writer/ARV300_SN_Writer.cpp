// ARV300_SN_Writer.cpp : �⺻ ������Ʈ �����Դϴ�.

#include "stdafx.h"
#include "ARV300_MainForm.h"

using namespace ARV300_SN_Writer;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// ��Ʈ���� ��������� ���� Windows XP �ð� ȿ���� Ȱ��ȭ�մϴ�.
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// �� â�� ����� �����մϴ�.
	Application::Run(gcnew ARV300_MainForm());
	return 0;
}
