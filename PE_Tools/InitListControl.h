#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <commctrl.h>
#include <commdlg.h >
#include <Psapi.h>
#include <stdio.h>
#include "PE_tools.h"
#include "AddListControlInfo.h"

extern HWND hListMoudles;
extern HWND hListSection;
extern HWND hListImport;
extern HWND hListImportInfo;
extern DWORD* fileBuffer;
extern PIMAGE_DOS_HEADER dos_header;
extern PIMAGE_NT_HEADERS32 nt_header;
extern PIMAGE_FILE_HEADER file_header;
extern PIMAGE_OPTIONAL_HEADER option_header;
extern PIMAGE_SECTION_HEADER sectcion_header;
extern PIMAGE_IMPORT_DESCRIPTOR import_section;

BOOL InitProcessListControl(HWND hWnd);
BOOL InitMoudleListControl(HWND hWnd);
BOOL InitSectionListControl(HWND hSectionListControl);
BOOL InitImportList(HWND hImportList);
BOOL InitImportIfoList(HWND hImportInfoList);
