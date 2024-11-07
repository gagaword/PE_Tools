#include "InitListControl.h"

// 对进程List添加列信息
BOOL InitProcessListControl(HWND hWnd)
{
	// 初始化 List Control
	HWND hListProcess = GetDlgItem(hWnd, IDC_LIST_PROCESS);

	// 设置 List Control 样式
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	// 插入列
	LVCOLUMN column;
	column.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	column.pszText = LPWSTR(L"进程名称");
	column.cx = 130;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 0, (LPARAM)&column);

	column.pszText = LPWSTR(L"PID");
	column.cx = 120;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 1, (LPARAM)&column);

	column.pszText = LPWSTR(L"基地址");
	column.cx = 120;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 2, (LPARAM)&column);

	column.pszText = LPWSTR(L"镜像大小");
	column.cx = 120;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 3, (LPARAM)&column);

	AddProcessInfo(hListProcess);

	return TRUE;
}

// 对模块List添加信息
BOOL InitMoudleListControl(HWND hWnd)
{
	// 初始化 List Control
	hListMoudles = GetDlgItem(hWnd, IDC_LIST_MOUDLE);

	// 设置 List Control 样式
	SendMessage(hListMoudles, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	// 插入列
	LVCOLUMN column;
	column.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	column.pszText = LPWSTR(L"模块名称");
	column.cx = 200;
	SendMessage(hListMoudles, LVM_INSERTCOLUMN, 0, (LPARAM)&column);

	column.pszText = LPWSTR(L"模块地址");
	column.cx = 290;
	SendMessage(hListMoudles, LVM_INSERTCOLUMN, 1, (LPARAM)&column);

	return TRUE;
}

// 初始化区段
BOOL InitSectionListControl(HWND hSectionListControl)
{
	// 初始化 List Control
	hListSection = GetDlgItem(hSectionListControl, IDC_LIST_QUDUAN);

	// 设置 List Control 样式
	SendMessage(hListSection, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	// 插入列
	LVCOLUMN column;
	column.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	column.pszText = LPWSTR(L"名称");
	column.cx = 100;
	SendMessage(hListSection, LVM_INSERTCOLUMN, 0, (LPARAM)&column);

	column.pszText = LPWSTR(L"V.内存中偏移");
	column.cx = 100;
	SendMessage(hListSection, LVM_INSERTCOLUMN, 1, (LPARAM)&column);

	column.pszText = LPWSTR(L"V.内存中大小");
	column.cx = 100;
	SendMessage(hListSection, LVM_INSERTCOLUMN, 2, (LPARAM)&column);

	column.pszText = LPWSTR(L"R.文件中偏移");
	column.cx = 100;
	SendMessage(hListSection, LVM_INSERTCOLUMN, 3, (LPARAM)&column);

	column.pszText = LPWSTR(L"R.文件中大小");
	column.cx = 100;
	SendMessage(hListSection, LVM_INSERTCOLUMN, 4, (LPARAM)&column);

	column.pszText = LPWSTR(L"标志");
	column.cx = 113;
	SendMessage(hListSection, LVM_INSERTCOLUMN, 5, (LPARAM)&column);

	return TRUE;
}

BOOL InitImportList(HWND hImportList)
{
	hListImport = GetDlgItem(hImportList, IDC_LIST_IMPORT_LI);

	// 设置 List Control 样式
	SendMessage(hListImport, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	// 插入列
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
	// 初始化 List Control
	hListImportInfo = GetDlgItem(hImportInfoList, IDC_LIST_IMPORT_INFO);

	// 设置 List Control 样式
	SendMessage(hListImportInfo, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	// 插入列
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
