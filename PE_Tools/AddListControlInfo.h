#pragma once

#include "framework.h"
#include "PE_tools.h"


DWORD RvaToFov(PIMAGE_NT_HEADERS ntHeaders, DWORD rva);

// ����������Ϣ
VOID AddProcessInfo(HWND hListProcesInfo);

// ��ģ�������Ϣ
VOID AddModulesInfo(HWND hListModuleInfo);

// �����������Ϣ
VOID AddSectionListControl(HWND hListSectionInfo);

VOID AddImportList(HWND hListImoprtControl);

VOID AddImportInfo(HWND HListImportInfo, HWND hDlg);


