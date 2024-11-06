#include "MessageProc.h"

// 函数声明
void SetHexDialogText(HWND hDlg, int controlID, DWORD value);
void ImportNumber(HWND hDlg);
void InitTime(HWND hDlg, HWND hParentWnd);
void TimeJiSuan(HWND hDlg);
void NowTime(HWND hDlg);
void JiSuan(HWND hDlg);

using RadioMapping = struct {
	int radioID;
	int subsystemValue;
};

RadioMapping radioMappings[] = {
	{IDC_RADIO2, IMAGE_SUBSYSTEM_UNKNOWN},
	{IDC_RADIO3, IMAGE_SUBSYSTEM_NATIVE},
	{IDC_RADIO4, IMAGE_SUBSYSTEM_WINDOWS_GUI},
	{IDC_RADIO5, IMAGE_SUBSYSTEM_WINDOWS_CUI},
	{IDC_RADIO6, IMAGE_SUBSYSTEM_OS2_CUI},
	{IDC_RADIO7, IMAGE_SUBSYSTEM_POSIX_CUI},
	{IDC_RADIO8, IMAGE_SUBSYSTEM_NATIVE_WINDOWS},
	{IDC_RADIO9, IMAGE_SUBSYSTEM_WINDOWS_CE_GUI},
	{IDC_RADIO10, IMAGE_SUBSYSTEM_EFI_APPLICATION},
	{IDC_RADIO11, IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER},
	{IDC_RADIO12, IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER},
	{IDC_RADIO13, IMAGE_SUBSYSTEM_EFI_ROM},
	{IDC_RADIO14, IMAGE_SUBSYSTEM_XBOX},
};

BOOL peInfoChuLi(HWND hDlg, DWORD* fileBuffer);

BOOL CALLBACK DiaMainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
	{
		EndDialog(hWnd, 0);

		return TRUE;
	}
	case WM_INITDIALOG:
	{
		// 加载图标
		HICON hBigIcon = LoadIcon(PEhInst, MAKEINTRESOURCE(IDI_PETOOLS));
		HICON hSmallIcon = LoadIcon(PEhInst, MAKEINTRESOURCE(IDI_SMALL));
		SendMessage(hWnd, WM_SETICON, ICON_BIG, (DWORD)hBigIcon);
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (DWORD)hSmallIcon);

		HMENU hMenu = LoadMenu(PEhInst, MAKEINTRESOURCE(IDC_PETOOLS));
		SetMenu(hWnd, hMenu);

		InitProcessListControl(hWnd);
		InitMoudleListControl(hWnd);

		return TRUE;
	}
	case WM_NOTIFY:
	{
		NMHDR* pNmhde = (NMHDR*)lParam;
		if (wParam == IDC_LIST_PROCESS && pNmhde->code == NM_CLICK)
		{
			AddModulesInfo(GetDlgItem(hWnd, IDC_LIST_PROCESS));
			return TRUE;
		}
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			// PE查看按钮
			case IDC_BUTTON_PE:
			{
				HWND buttonPeHwnd = GetDlgItem(hWnd, IDC_BUTTON_PE);
				TCHAR szFile[260]; // 用于存储文件路径

				if (OpenFileDialog(hWnd, szFile, sizeof(szFile) / sizeof(TCHAR)))
				{
					// 处理打开的文件，例如读取内容
					//MessageBox(hWnd, szFile, L"文件路径", MB_OK);
					DialogBoxParam(PEhInst, MAKEINTRESOURCE(IDD_DIALOG_PE), buttonPeHwnd, PeFuncProc, (LPARAM)szFile);
				}
				break;
			}
			// 关于按钮
			case IDC_BUTTON_ABOUT:
			{
				DialogBox(PEhInst, MAKEINTRESOURCE(IDD_ABOUTBOX), nullptr, AboutProc);
				break;
			}
			// 退出按钮
			case IDC_BUTTON_EXIT:
			{
				EndDialog(hWnd, 0);
				return TRUE;
			}
			// 关于
			case IDM_ABOUT:
			{
				DialogBox(PEhInst, MAKEINTRESOURCE(IDD_ABOUTBOX), nullptr, AboutProc);
				break;
			}
			// 退出
			case IDM_EXIT:
			{
				EndDialog(hWnd, 0);
				return TRUE;
			}
			case IDM_OPEN:
			{
				TCHAR szFile[260]; // 用于存储文件路径

				if (OpenFileDialog(hWnd, szFile, sizeof(szFile) / sizeof(TCHAR)))
				{
					// 处理打开的文件，例如读取内容
					MessageBox(hWnd, szFile, L"文件路径", MB_OK);
				}
				break;
			}
			case IDM_TIME:
			{
				HWND button_time_info = GetDlgItem(hWnd, IDD_DIALOG_TIME_TO_RILI);
				DialogBox(PEhInst, MAKEINTRESOURCE(IDD_DIALOG_TIME_TO_RILI), button_time_info, idm_time_proc);
				break;
			}
		}
		break;
	}
	}
	return FALSE;
}

BOOL CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return TRUE;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	}
	return FALSE;
}

BOOL CALLBACK PeFuncProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
	{
		EndDialog(hDlg, 0);

		return TRUE;
	}
	case WM_INITDIALOG:
	{
		// 打开PE文件，处理PE文件信息
		PeInfo(hDlg, (TCHAR*)lParam);

		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			// 退出
			case IDC_BUTTON_PE_EXIT:
			{
				EndDialog(hDlg, 0);

				return TRUE;
			}
			// 区段目录
			case IDC_BUTTON_SECTION:
			{
				HWND buttonPeSectionHwnd = GetDlgItem(hDlg, IDC_BUTTON_SECTION);
				DialogBox(PEhInst, MAKEINTRESOURCE(IDD_DIALOG_QUDUAN), buttonPeSectionHwnd, PeSectionProc);

				break;
			}
			// 目录数据
			case IDC_BUTTON_MULU:
			{
				HWND buttonPeDataHwnd = GetDlgItem(hDlg, IDC_BUTTON_MULU);
				DialogBox(PEhInst, MAKEINTRESOURCE(IDD_DIALOG_DATAMULU), buttonPeDataHwnd, PeDataProc);

				break;
			}

			// 子系统更多信息
			case IDC_BUTTON_ZIXITONG_INFO:
			{
				wchar_t subsytem[0x10];
				HWND button_subsytem = GetDlgItem(hDlg, IDC_EDIT_ZIXITONG);
				GetDlgItemText(hDlg, IDC_EDIT_ZIXITONG, subsytem, 0x10);
				long value = wcstol(subsytem, nullptr, 16);
				int intValue = (int)value;

				HWND buttonSubSytem_Info = GetDlgItem(hDlg, IDC_BUTTON_ZIXITONG_INFO);
				DialogBoxParam(PEhInst, MAKEINTRESOURCE(IDD_DIALOG_SUBSYTEM_INFO), buttonSubSytem_Info, SubSytemProc, (LPARAM)intValue);		
				break;

			}
			// 时间戳
			case IDC_BUTTON_TIME_INFO:
			{
				HWND button_time_info = GetDlgItem(hDlg, IDD_DIALOG_TIME_TO_RILI);
				DialogBoxParam(PEhInst, MAKEINTRESOURCE(IDD_DIALOG_TIME_TO_RILI), button_time_info, TimeProc, (LPARAM)hDlg);
				break;

			}
			break;
		}
	}
	break;
	}
	return FALSE;
}

BOOL CALLBACK PeSectionProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
		{
			EndDialog(hDlg, 0);
			return TRUE;
		}
		case WM_INITDIALOG:
		{
			InitSectionListControl(hDlg);	
			AddSectionListControl(hDlg);
			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_BUTTON_SECTION_EXIT:
				{
					EndDialog(hDlg, 0);
					return TRUE;
				}
				break;
			}
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK PeDataProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
		{
			EndDialog(hDlg, 0);

			return TRUE;
		}
		case WM_INITDIALOG:
		{
			AddDataInfo(hDlg);
			
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_BUTTON_IMPORT_INFO:
				{
					HWND button_import_info = GetDlgItem(hDlg, IDC_BUTTON_IMPORT_INFO);
					DialogBox(PEhInst, MAKEINTRESOURCE(IDD_DIALOG_IMPORT), button_import_info, ImportInfo);
					break;
				}
				break;
			}
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK SubSytemProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
		{
			EndDialog(hDlg, 0);
			return TRUE;
		}
		case WM_INITDIALOG:
		{
			int intValue = static_cast<int>(lParam); // 转换 lParam
	
			for (auto & radioMapping : radioMappings) {
				if (radioMapping.subsystemValue == intValue) {
					wchar_t buffer[32];
					swprintf(buffer, 32, L"%04X", radioMapping.subsystemValue);
					SetDlgItemText(hDlg, IDC_STATIC_SUBSYTEM_NUM, buffer);

					// 给RADIO控件赋值
					CheckDlgButton(hDlg, radioMapping.radioID, BST_CHECKED);
					break;
				}
			}

			break;
		}
		case WM_COMMAND:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				int controlID = LOWORD(wParam);
				for (int i = 0; i < sizeof(radioMappings) / sizeof(radioMappings[0]); i++) {
					if (radioMappings[i].radioID == controlID) {
						wchar_t buffer[32];
						swprintf(buffer, 32, L"%04X", radioMappings[i].subsystemValue);
						SetDlgItemText(hDlg, IDC_STATIC_SUBSYTEM_NUM, buffer);
						break;
					}
				}
			}
			switch (LOWORD(wParam))
			{
				case IDC_BUTTON_SUBSYTEM_EXIT:
				{
					EndDialog(hDlg, 0);
					return TRUE;
				}
				
				break;
			}
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK ImportInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
		{
			EndDialog(hDlg, 0);
			return TRUE;
		}
		case WM_INITDIALOG:
		{
			InitImportList(hDlg);
			InitImportIfoList(hDlg);

			AddImportList(hDlg);

			ImportNumber(hDlg);
		}
		case WM_NOTIFY:
		{
			NMHDR* pNmhde = (NMHDR*)lParam;
			if (wParam == IDC_LIST_IMPORT_LI && pNmhde->code == NM_CLICK)
			{
				AddImportInfo(GetDlgItem(hDlg, IDC_LIST_IMPORT_LI), hDlg);
				return TRUE;
			}
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{

				case IDC_BUTTON_IMPORT_EXIT:
				{
					EndDialog(hDlg, 0);
					return TRUE;
				}

			}
		}
		break;
	}
	return FALSE;
}
BOOL OpenFileDialog(HWND hWnd, TCHAR* szFile, DWORD size)
{
	OPENFILENAME ofn;       // 结构体，用于存储打开文件对话框的信息

	// 清空结构体
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = size;
	ofn.lpstrFilter = L"所有文件 (*.*)\0*.*\0PE文件(*.exe)\0*.exe\0DLL文件(*.dll)\0*.dll"; // 文件过滤器
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// 弹出打开文件对话框
	return GetOpenFileName(&ofn);
}

BOOL PeInfo(HWND hDlg, TCHAR* filePath)
{
	TCHAR filePathTemp[260];
	wsprintf(filePathTemp, L"%s\n", filePath);
	//MessageBox(hDlg, filePathTemp, L"文件路径", MB_OK);

	// 打开文件
	FILE* file = nullptr;
	errno_t openResult = _wfopen_s(&file, filePath, L"rb");
	if (openResult != 0 || file == nullptr)
	{
		DWORD error = GetLastError();
		wchar_t errorMessage[0x100];
		swprintf(errorMessage, sizeof(errorMessage) / sizeof(wchar_t), L"失败,错误代码: %d", error);
		MessageBox(hDlg, errorMessage, TEXT("PE处理错误"), MB_OK);
		return FALSE;
	}

	// 获取文件大小
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	// 读取文件数据
	fileBuffer = (DWORD*)malloc(fileSize);
	size_t readResult = fread_s(fileBuffer, fileSize, 1, fileSize, file);

	if (readResult != fileSize)
	{
		DWORD error = GetLastError();
		wchar_t errorMessage[0x100];
		swprintf(errorMessage, sizeof(errorMessage) / sizeof(wchar_t), L"失败,错误代码: %d", error);
		MessageBox(hDlg, errorMessage, TEXT("PE处理错误"), MB_OK);
		return FALSE;
	}

	if (peInfoChuLi(hDlg, fileBuffer))
	{
		fclose(file);
		return TRUE;
	}

	return FALSE;
}

// 辅助函数
DWORD RvaToFov(PIMAGE_NT_HEADERS ntHeaders, DWORD rva) {
	
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);
	for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++, section++) {
		DWORD sectionStartRva = section->VirtualAddress;
		DWORD sectionSize = section->Misc.VirtualSize;
		if (rva >= sectionStartRva && rva < sectionStartRva + sectionSize) {
			return (rva - sectionStartRva) + section->PointerToRawData;
		}
	}

	// 如果没有找到匹配的节，返回 0 表示无效偏移
	return 0;
}

BOOL AddDataInfo(HWND hwnd)
{
	PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)fileBuffer;
	PIMAGE_NT_HEADERS32 nt_header = (PIMAGE_NT_HEADERS32)(DWORD)((BYTE*)fileBuffer + dos_header->e_lfanew);
	PIMAGE_FILE_HEADER file_header = (PIMAGE_FILE_HEADER)&nt_header->FileHeader;
	PIMAGE_OPTIONAL_HEADER option_header = (PIMAGE_OPTIONAL_HEADER)&nt_header->OptionalHeader;

	// 导入表地址
	DWORD import_fov = RvaToFov(nt_header, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	PIMAGE_IMPORT_DESCRIPTOR import_section = (PIMAGE_IMPORT_DESCRIPTOR)(DWORD)((BYTE*)fileBuffer + import_fov);

	// 导入表
	SetHexDialogText(hwnd, IDC_EDIT_IMPORT_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_IMPORT_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size);

	// 导出表
	SetHexDialogText(hwnd, IDC_EDIT_EXPORT_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_EXPORT_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size);

	// 资源
	SetHexDialogText(hwnd, IDC_EDIT_SRECOR_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_SRECOR_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size);

	// 异常
	SetHexDialogText(hwnd, IDC_EDIT_TRY_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_TRY_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size);

	// 安全证书
	SetHexDialogText(hwnd, IDC_EDIT_ANQUAN_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_ANQUAN_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size);

	// 重定位表
	SetHexDialogText(hwnd, IDC_EDIT_CHONG_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_CHONG_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);

	// 调试信息
	SetHexDialogText(hwnd, IDC_EDIT_DEBUG_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_DEBUG_SZIE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size);

	// 版权所有
	SetHexDialogText(hwnd, IDC_EDIT_BAN_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_BAN_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size);

	// 全局指针
	SetHexDialogText(hwnd, IDC_EDIT_POINTRE_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_POINTRE_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size);

	// TLS表
	SetHexDialogText(hwnd, IDC_EDIT_TLS_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_TLS_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size);

	// 加载配置
	SetHexDialogText(hwnd, IDC_EDIT_JIA_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_JIA_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size);

	// 绑定导入
	SetHexDialogText(hwnd, IDC_EDIT_BANGDING_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_BANGDING_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size);

	// IAT表
	SetHexDialogText(hwnd, IDC_EDIT_IAT_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_IAT_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size);

	// 延迟导入
	SetHexDialogText(hwnd, IDC_EDIT_YANCHI_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_YANCHI_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size);

	// COM
	SetHexDialogText(hwnd, IDC_EDIT_COM_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_COM_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size);

	// 保留
	SetHexDialogText(hwnd, IDC_EDIT_BAOLIU_RVA, 0x00000000);
	SetHexDialogText(hwnd, IDC_EDIT_BAOLIU_SIZE, 0x00000000);

	return FALSE;

}

// 辅助函数
void SetHexDialogText(HWND hDlg, int controlID, DWORD value) {
	wchar_t buffer[16];
	swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t), L"%08X", value);
	SendDlgItemMessage(hDlg, controlID, WM_SETTEXT, 0, (LPARAM)buffer);
}

BOOL peInfoChuLi(HWND hDlg, DWORD* fileBuffer)
{
	PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)fileBuffer;
	PIMAGE_NT_HEADERS32 nt_header = (PIMAGE_NT_HEADERS32)(DWORD)((BYTE*)fileBuffer + dos_header->e_lfanew);
	PIMAGE_FILE_HEADER file_header = (PIMAGE_FILE_HEADER)&nt_header->FileHeader;
	PIMAGE_OPTIONAL_HEADER option_header = (PIMAGE_OPTIONAL_HEADER)&nt_header->OptionalHeader;

	// 给各个编辑框进行赋值
	SetHexDialogText(hDlg, IDC_EDIT_ENCRYPTION, option_header->AddressOfEntryPoint);
	SetHexDialogText(hDlg, IDC_EDIT_ZIXITONG, option_header->Subsystem);
	SetHexDialogText(hDlg, IDC_EDIT_IMAGEBASE, option_header->ImageBase);
	SetHexDialogText(hDlg, IDC_EDIT_IMAGESIZE, option_header->SizeOfImage);
	SetHexDialogText(hDlg, IDC_EDIT_SECTIONNUMBER, file_header->NumberOfSections);
	SetHexDialogText(hDlg, IDC_EDIT_TIME, file_header->TimeDateStamp);
	SetHexDialogText(hDlg, IDC_EDIT_CODEBASE, option_header->BaseOfCode);
	SetHexDialogText(hDlg, IDC_EDIT_PESIZE, option_header->SizeOfHeaders);
	SetHexDialogText(hDlg, IDC_EDIT_DATABASE, option_header->BaseOfData);
	SetHexDialogText(hDlg, IDC_EDIT_TEZHNEG, file_header->Characteristics);
	SetHexDialogText(hDlg, IDC_EDIT_SEALIGNMET, option_header->SectionAlignment);
	SetHexDialogText(hDlg, IDC_EDIT_FILEALIGNMET, option_header->FileAlignment);
	SetHexDialogText(hDlg, IDC_EDIT_JIAOYANHE, option_header->CheckSum);
	SetHexDialogText(hDlg, IDC_EDIT_PEHEADER, file_header->SizeOfOptionalHeader);
	SetHexDialogText(hDlg, IDC_EDIT_ISFLAG, dos_header->e_magic);
	SetHexDialogText(hDlg, IDC_EDIT_MULU, option_header->NumberOfRvaAndSizes);

	return TRUE;
}

void ImportNumber(HWND hDlg)
{
	DWORD import_fov = RvaToFov(nt_header, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	auto import_section = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)fileBuffer + import_fov);

	int importNumber = 0;
	while (import_section[importNumber].OriginalFirstThunk != 0)
	{
		importNumber++;
	}
	wchar_t buffer[32];
	swprintf(buffer, 32, L"%d", importNumber);
	// 设置静态控件的文本
	SetDlgItemText(hDlg, IDC_STATIC_IMPORT_NUMBER, buffer);	
}


BOOL CALLBACK TimeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hParentWnd = NULL;

	switch (message)
	{
		case WM_CLOSE:
		{
			EndDialog(hDlg, 0);
			return TRUE;
		}
		case WM_INITDIALOG:
		{
			CheckDlgButton(hDlg, IDC_RADIO2, BST_CHECKED);
			hParentWnd = (HWND)lParam;
			InitTime(hDlg, hParentWnd);
			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_BUTTON_TIME_EXIT:
				{
					EndDialog(hDlg, 0);
					return TRUE;
				}
				case IDC_RADIO2:
					if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
					{
						SetWindowText(hDlg, L"时间戳->日历时间");
						EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), FALSE);
						EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), FALSE);
						EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), TRUE);
					}
					break;
				case IDC_RADIO3:
					if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
					{
						SetWindowText(hDlg, L"日历时间->时间戳");
						EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), TRUE);
						EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), TRUE);
						EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), FALSE);
					}
					break;
				case IDC_BUTTON_TIME_JISUAN:
				{
					hParentWnd = (HWND)lParam;
					TimeJiSuan(hDlg);
					break;

				}
				case IDC_BUTTON_TIME_NOW:
				{
					//NowTime(hDlg);
					// 获取当前系统时间
					SYSTEMTIME systemTime;
					GetLocalTime(&systemTime); // 获取当前本地时间

					// 将 SYSTEMTIME 设置到两个 Date Time Picker 控件
					DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), GDT_VALID, &systemTime);
					DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), GDT_VALID, &systemTime);

					// 将 SYSTEMTIME 转换为 FILETIME
					FILETIME fileTime;
					SystemTimeToFileTime(&systemTime, &fileTime);

					// 将 FILETIME 转换为 Unix 时间戳
					ULARGE_INTEGER ull;
					ull.LowPart = fileTime.dwLowDateTime;
					ull.HighPart = fileTime.dwHighDateTime;

					// FILETIME 是从 1601 年 1 月 1 日开始的 100 纳秒计数，转换为 Unix 时间戳
					const __int64 EPOCH_DIFF = 11644473600LL; // Unix 时间与 Windows FILETIME 的时间差（秒）
					time_t timestamp = static_cast<time_t>((ull.QuadPart / 10000000ULL) - EPOCH_DIFF);

					// 显示计算的时间戳值（可根据你的需要显示）
					wchar_t buffer[0x20];
					swprintf_s(buffer, L"%lld", timestamp);
					SetDlgItemText(hDlg, IDC_EDIT1, buffer);
					break;

				}
				break;
			}
			break;
		}
		break;
	}
	return FALSE;
}

void InitTime(HWND hDlg, HWND hParentWnd)
{
	// 检查一个按钮（如果需要）
	CheckDlgButton(hDlg, IDC_RADIO2, BST_CHECKED);

	if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
	{
		SetWindowText(hDlg, L"时间戳->日历时间");
		EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), FALSE);
	}

	wchar_t Time[0x10];
	HWND button_subsytem = GetDlgItem(hDlg, IDC_EDIT_TIME);
	GetDlgItemText(hParentWnd, IDC_EDIT_TIME, Time, 0x10);
	long value = wcstol(Time, nullptr, 16);
	int intValue = (int)value;

	// 将 Unix 时间戳转换为 FILETIME
	__int64 timeStamp = static_cast<__int64>(intValue) + 11644473600LL; // 将 Unix 时间戳转换为 Windows FILETIME
	timeStamp *= 10000000LL; // 转换为 100 纳秒单位

	FILETIME fileTime;
	fileTime.dwLowDateTime = static_cast<DWORD>(timeStamp & 0xFFFFFFFF);
	fileTime.dwHighDateTime = static_cast<DWORD>(timeStamp >> 32);

	// 转换为 SYSTEMTIME
	SYSTEMTIME systemTime;
	FileTimeToSystemTime(&fileTime, &systemTime);

	// 将 SYSTEMTIME 设置到两个 Date Time Picker 控件
	DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), GDT_VALID, &systemTime);
	DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), GDT_VALID, &systemTime);

	// 显示时间戳值（如果需要）
	wchar_t time_value[0x20];
	wsprintf(time_value, L"%d", intValue);
	SetDlgItemText(hDlg, IDC_EDIT1, time_value);	
}

void TimeJiSuan(HWND hDlg)
{
	if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
	{		
		wchar_t Time[0x20];
		HWND button_subsytem = GetDlgItem(hDlg, IDC_EDIT1);
		GetDlgItemText(hDlg, IDC_EDIT1, Time, 0x20);
		unsigned long long value = wcstoull(Time, nullptr, 10);

		auto intValue = value;

		// 将 Unix 时间戳转换为 FILETIME
		__int64 timeStamp = static_cast<__int64>(intValue) + 11644473600LL; // 将 Unix 时间戳转换为 Windows FILETIME
		timeStamp *= 10000000LL; // 转换为 100 纳秒单位

		FILETIME fileTime;
		fileTime.dwLowDateTime = static_cast<DWORD>(timeStamp & 0xFFFFFFFF);
		fileTime.dwHighDateTime = static_cast<DWORD>(timeStamp >> 32);

		// 转换为 SYSTEMTIME
		SYSTEMTIME systemTime;
		FileTimeToSystemTime(&fileTime, &systemTime);

		// 将 SYSTEMTIME 设置到两个 Date Time Picker 控件
		DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), GDT_VALID, &systemTime);
		DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), GDT_VALID, &systemTime);

		// 显示时间戳值（如果需要）
		wchar_t time_value[0x20];
		wsprintf(time_value, L"%d", intValue);
		SetDlgItemText(hDlg, IDC_EDIT1, time_value);
	}
	else if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
	{
		// 获取 Date Time Picker 1 和 2 的 SYSTEMTIME
		SYSTEMTIME st1, st2;
		DateTime_GetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), &st1);
		DateTime_GetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), &st2);

		// 将 SYSTEMTIME 转换为 FILETIME
		FILETIME ft1, ft2;
		SystemTimeToFileTime(&st1, &ft1);
		SystemTimeToFileTime(&st2, &ft2);

		// 将 FILETIME 转换为 Unix 时间戳
		ULARGE_INTEGER ull1, ull2;
		ull1.LowPart = ft1.dwLowDateTime;
		ull1.HighPart = ft1.dwHighDateTime;
		ull2.LowPart = ft2.dwLowDateTime;
		ull2.HighPart = ft2.dwHighDateTime;

		// FILETIME 是从 1601 年 1 月 1 日开始的 100 纳秒计数，转换为 Unix 时间戳
		const __int64 EPOCH_DIFF = 11644473600LL; // Unix 时间与 Windows FILETIME 的时间差（秒）
		time_t timestamp1 = static_cast<time_t>((ull1.QuadPart / 10000000ULL) - EPOCH_DIFF);
		time_t timestamp2 = static_cast<time_t>((ull2.QuadPart / 10000000ULL) - EPOCH_DIFF);

		// 输出转换结果（根据你的需求显示或使用）
		wchar_t buffer[256];
		swprintf_s(buffer, L"%lld\n", timestamp1);
		SetDlgItemText(hDlg, IDC_EDIT1, buffer);
	}
}

void NowTime(HWND hDlg)
{
	// 获取当前系统时间
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime); // 获取当前本地时间

	// 将 SYSTEMTIME 设置到两个 Date Time Picker 控件
	DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), GDT_VALID, &systemTime);
	DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), GDT_VALID, &systemTime);

	// 将 SYSTEMTIME 转换为 FILETIME
	FILETIME fileTime;
	SystemTimeToFileTime(&systemTime, &fileTime);

	// 将 FILETIME 转换为 Unix 时间戳
	ULARGE_INTEGER ull;
	ull.LowPart = fileTime.dwLowDateTime;
	ull.HighPart = fileTime.dwHighDateTime;

	// FILETIME 是从 1601 年 1 月 1 日开始的 100 纳秒计数，转换为 Unix 时间戳
	const __int64 EPOCH_DIFF = 11644473600LL; // Unix 时间与 Windows FILETIME 的时间差（秒）
	time_t timestamp = static_cast<time_t>((ull.QuadPart / 10000000ULL) - EPOCH_DIFF);

	// 显示计算的时间戳值（可根据你的需要显示）
	wchar_t buffer[0x20];
	swprintf_s(buffer, L"%lld", timestamp);
	SetDlgItemText(hDlg, IDC_EDIT1, buffer);
}


BOOL CALLBACK idm_time_proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
		case WM_CLOSE: 
		{
			EndDialog(hDlg, 0);
			return TRUE;
		}
		case WM_INITDIALOG:
		{
			CheckDlgButton(hDlg, IDC_RADIO2, BST_CHECKED);
			SetWindowText(hDlg, L"时间戳->日历时间");
			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_RADIO2:
				{
					SetWindowText(hDlg, L"时间戳->日历时间");
					break;
				}
				case IDC_RADIO3:
				{
					SetWindowText(hDlg, L"日历时间->时间戳");
					break;
				}
				case IDC_BUTTON_TIME_JISUAN:
				{
					//JiSuan(hDlg);
					TimeJiSuan(hDlg);
					break;
				}
				case IDC_BUTTON_TIME_NOW:
				{
					NowTime(hDlg);
					break;
				}
				case IDC_BUTTON_TIME_EXIT:
				{
					EndDialog(hDlg, 0);
					return TRUE;
				}
				break;
			}
			break;
		}
		break;
	}
	return FALSE;
}