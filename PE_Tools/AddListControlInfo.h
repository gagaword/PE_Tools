#pragma once

#include "framework.h"
#include "PE_tools.h"


DWORD RvaToFov(PIMAGE_NT_HEADERS ntHeaders, DWORD rva);

// 向进程添加信息
VOID AddProcessInfo(HWND hListProcesInfo);

// 向模块添加信息
VOID AddModulesInfo(HWND hListModuleInfo);

// 向区段添加信息
VOID AddSectionListControl(HWND hListSectionInfo);

VOID AddImportList(HWND hListImoprtControl);

VOID AddImportInfo(HWND HListImportInfo, HWND hDlg);


