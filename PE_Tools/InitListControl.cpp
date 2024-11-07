#include "InitListControl.h"

// �Խ���List�������Ϣ
BOOL InitProcessListControl(HWND hWnd)
{
	// ��ʼ�� List Control
	HWND hListProcess = GetDlgItem(hWnd, IDC_LIST_PROCESS);

	// ���� List Control ��ʽ
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	// ������
	LVCOLUMN column;
	column.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	column.pszText = LPWSTR(L"��������");
	column.cx = 130;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 0, (LPARAM)&column);

	column.pszText = LPWSTR(L"PID");
	column.cx = 120;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 1, (LPARAM)&column);

	column.pszText = LPWSTR(L"����ַ");
	column.cx = 120;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 2, (LPARAM)&column);

	column.pszText = LPWSTR(L"�����С");
	column.cx = 120;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 3, (LPARAM)&column);

	AddProcessInfo(hListProcess);

	return TRUE;
}

// ��ģ��List�����Ϣ
BOOL InitMoudleListControl(HWND hWnd)
{
	// ��ʼ�� List Control
	hListMoudles = GetDlgItem(hWnd, IDC_LIST_MOUDLE);

	// ���� List Control ��ʽ
	SendMessage(hListMoudles, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	// ������
	LVCOLUMN column;
	column.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	column.pszText = LPWSTR(L"ģ������");
	column.cx = 200;
	SendMessage(hListMoudles, LVM_INSERTCOLUMN, 0, (LPARAM)&column);

	column.pszText = LPWSTR(L"ģ���ַ");
	column.cx = 290;
	SendMessage(hListMoudles, LVM_INSERTCOLUMN, 1, (LPARAM)&column);

	return TRUE;
}

// ��ʼ������
BOOL InitSectionListControl(HWND hSectionListControl)
{
	// ��ʼ�� List Control
	hListSection = GetDlgItem(hSectionListControl, IDC_LIST_QUDUAN);

	// ���� List Control ��ʽ
	SendMessage(hListSection, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	// ������
	LVCOLUMN column;
	column.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	column.pszText = LPWSTR(L"����");
	column.cx = 100;
	SendMessage(hListSection, LVM_INSERTCOLUMN, 0, (LPARAM)&column);

	column.pszText = LPWSTR(L"V.�ڴ���ƫ��");
	column.cx = 100;
	SendMessage(hListSection, LVM_INSERTCOLUMN, 1, (LPARAM)&column);

	column.pszText = LPWSTR(L"V.�ڴ��д�С");
	column.cx = 100;
	SendMessage(hListSection, LVM_INSERTCOLUMN, 2, (LPARAM)&column);

	column.pszText = LPWSTR(L"R.�ļ���ƫ��");
	column.cx = 100;
	SendMessage(hListSection, LVM_INSERTCOLUMN, 3, (LPARAM)&column);

	column.pszText = LPWSTR(L"R.�ļ��д�С");
	column.cx = 100;
	SendMessage(hListSection, LVM_INSERTCOLUMN, 4, (LPARAM)&column);

	column.pszText = LPWSTR(L"��־");
	column.cx = 113;
	SendMessage(hListSection, LVM_INSERTCOLUMN, 5, (LPARAM)&column);

	return TRUE;
}

BOOL InitImportList(HWND hImportList)
{
	hListImport = GetDlgItem(hImportList, IDC_LIST_IMPORT_LI);

	// ���� List Control ��ʽ
	SendMessage(hListImport, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	// ������
	LVCOLUMN column;
	column.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	column.pszText = LPWSTR(L"DLL Name");
	column.cx = 180;
	SendMessage(hListImport, LVM_INSERTCOLUMN, 0, (LPARAM)&column);

	column.pszText = LPWSTR(L"Original First Thunk");
	column.cx = 105;
	SendMessage(hListImport, LVM_INSERTCOLUMN, 1, (LPARAM)&column);

	column.pszText = LPWSTR(L"Time/Data Stamp");
	column.cx = 105;
	SendMessage(hListImport, LVM_INSERTCOLUMN, 2, (LPARAM)&column);

	column.pszText = LPWSTR(L"ForWarder Chain");
	column.cx = 105;
	SendMessage(hListImport, LVM_INSERTCOLUMN, 3, (LPARAM)&column);

	column.pszText = LPWSTR(L"Name(RVA)");
	column.cx = 105;
	SendMessage(hListImport, LVM_INSERTCOLUMN, 4, (LPARAM)&column);

	column.pszText = LPWSTR(L"First Thunk");
	column.cx = 105;
	SendMessage(hListImport, LVM_INSERTCOLUMN, 5, (LPARAM)&column);

	return TRUE;
}

BOOL InitImportIfoList(HWND hImportInfoList)
{
	// ��ʼ�� List Control
	hListImportInfo = GetDlgItem(hImportInfoList, IDC_LIST_IMPORT_INFO);

	// ���� List Control ��ʽ
	SendMessage(hListImportInfo, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	// ������
	LVCOLUMN column;
	column.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	column.pszText = LPWSTR(L"Thunk RVA");
	column.cx = 130;
	SendMessage(hListImportInfo, LVM_INSERTCOLUMN, 0, (LPARAM)&column);

	column.pszText = LPWSTR(L"Thunk Offset");
	column.cx = 150;
	SendMessage(hListImportInfo, LVM_INSERTCOLUMN, 1, (LPARAM)&column);

	column.pszText = LPWSTR(L"Thunk Value");
	column.cx = 150;
	SendMessage(hListImportInfo, LVM_INSERTCOLUMN, 2, (LPARAM)&column);

	column.pszText = LPWSTR(L"Hint");
	column.cx = 110;
	SendMessage(hListImportInfo, LVM_INSERTCOLUMN, 3, (LPARAM)&column);

	column.pszText = LPWSTR(L"API Name");
	column.cx = 160;
	SendMessage(hListImportInfo, LVM_INSERTCOLUMN, 4, (LPARAM)&column);

	return TRUE;
}
