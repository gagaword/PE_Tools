#include "MessageProc.h"

// ��������
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
		// ����ͼ��
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
			// PE�鿴��ť
			case IDC_BUTTON_PE:
			{
				HWND buttonPeHwnd = GetDlgItem(hWnd, IDC_BUTTON_PE);
				TCHAR szFile[260]; // ���ڴ洢�ļ�·��

				if (OpenFileDialog(hWnd, szFile, sizeof(szFile) / sizeof(TCHAR)))
				{
					// ����򿪵��ļ��������ȡ����
					//MessageBox(hWnd, szFile, L"�ļ�·��", MB_OK);
					DialogBoxParam(PEhInst, MAKEINTRESOURCE(IDD_DIALOG_PE), buttonPeHwnd, PeFuncProc, (LPARAM)szFile);
				}
				break;
			}
			// ���ڰ�ť
			case IDC_BUTTON_ABOUT:
			{
				DialogBox(PEhInst, MAKEINTRESOURCE(IDD_ABOUTBOX), nullptr, AboutProc);
				break;
			}
			// �˳���ť
			case IDC_BUTTON_EXIT:
			{
				EndDialog(hWnd, 0);
				return TRUE;
			}
			// ����
			case IDM_ABOUT:
			{
				DialogBox(PEhInst, MAKEINTRESOURCE(IDD_ABOUTBOX), nullptr, AboutProc);
				break;
			}
			// �˳�
			case IDM_EXIT:
			{
				EndDialog(hWnd, 0);
				return TRUE;
			}
			case IDM_OPEN:
			{
				TCHAR szFile[260]; // ���ڴ洢�ļ�·��

				if (OpenFileDialog(hWnd, szFile, sizeof(szFile) / sizeof(TCHAR)))
				{
					// ����򿪵��ļ��������ȡ����
					MessageBox(hWnd, szFile, L"�ļ�·��", MB_OK);
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
		// ��PE�ļ�������PE�ļ���Ϣ
		PeInfo(hDlg, (TCHAR*)lParam);

		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			// �˳�
			case IDC_BUTTON_PE_EXIT:
			{
				EndDialog(hDlg, 0);

				return TRUE;
			}
			// ����Ŀ¼
			case IDC_BUTTON_SECTION:
			{
				HWND buttonPeSectionHwnd = GetDlgItem(hDlg, IDC_BUTTON_SECTION);
				DialogBox(PEhInst, MAKEINTRESOURCE(IDD_DIALOG_QUDUAN), buttonPeSectionHwnd, PeSectionProc);

				break;
			}
			// Ŀ¼����
			case IDC_BUTTON_MULU:
			{
				HWND buttonPeDataHwnd = GetDlgItem(hDlg, IDC_BUTTON_MULU);
				DialogBox(PEhInst, MAKEINTRESOURCE(IDD_DIALOG_DATAMULU), buttonPeDataHwnd, PeDataProc);

				break;
			}

			// ��ϵͳ������Ϣ
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
			// ʱ���
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
			int intValue = static_cast<int>(lParam); // ת�� lParam
	
			for (auto & radioMapping : radioMappings) {
				if (radioMapping.subsystemValue == intValue) {
					wchar_t buffer[32];
					swprintf(buffer, 32, L"%04X", radioMapping.subsystemValue);
					SetDlgItemText(hDlg, IDC_STATIC_SUBSYTEM_NUM, buffer);

					// ��RADIO�ؼ���ֵ
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
	OPENFILENAME ofn;       // �ṹ�壬���ڴ洢���ļ��Ի������Ϣ

	// ��սṹ��
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = size;
	ofn.lpstrFilter = L"�����ļ� (*.*)\0*.*\0PE�ļ�(*.exe)\0*.exe\0DLL�ļ�(*.dll)\0*.dll"; // �ļ�������
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// �������ļ��Ի���
	return GetOpenFileName(&ofn);
}

BOOL PeInfo(HWND hDlg, TCHAR* filePath)
{
	TCHAR filePathTemp[260];
	wsprintf(filePathTemp, L"%s\n", filePath);
	//MessageBox(hDlg, filePathTemp, L"�ļ�·��", MB_OK);

	// ���ļ�
	FILE* file = nullptr;
	errno_t openResult = _wfopen_s(&file, filePath, L"rb");
	if (openResult != 0 || file == nullptr)
	{
		DWORD error = GetLastError();
		wchar_t errorMessage[0x100];
		swprintf(errorMessage, sizeof(errorMessage) / sizeof(wchar_t), L"ʧ��,�������: %d", error);
		MessageBox(hDlg, errorMessage, TEXT("PE�������"), MB_OK);
		return FALSE;
	}

	// ��ȡ�ļ���С
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	// ��ȡ�ļ�����
	fileBuffer = (DWORD*)malloc(fileSize);
	size_t readResult = fread_s(fileBuffer, fileSize, 1, fileSize, file);

	if (readResult != fileSize)
	{
		DWORD error = GetLastError();
		wchar_t errorMessage[0x100];
		swprintf(errorMessage, sizeof(errorMessage) / sizeof(wchar_t), L"ʧ��,�������: %d", error);
		MessageBox(hDlg, errorMessage, TEXT("PE�������"), MB_OK);
		return FALSE;
	}

	if (peInfoChuLi(hDlg, fileBuffer))
	{
		fclose(file);
		return TRUE;
	}

	return FALSE;
}

// ��������
DWORD RvaToFov(PIMAGE_NT_HEADERS ntHeaders, DWORD rva) {
	
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);
	for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++, section++) {
		DWORD sectionStartRva = section->VirtualAddress;
		DWORD sectionSize = section->Misc.VirtualSize;
		if (rva >= sectionStartRva && rva < sectionStartRva + sectionSize) {
			return (rva - sectionStartRva) + section->PointerToRawData;
		}
	}

	// ���û���ҵ�ƥ��Ľڣ����� 0 ��ʾ��Чƫ��
	return 0;
}

BOOL AddDataInfo(HWND hwnd)
{
	PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)fileBuffer;
	PIMAGE_NT_HEADERS32 nt_header = (PIMAGE_NT_HEADERS32)(DWORD)((BYTE*)fileBuffer + dos_header->e_lfanew);
	PIMAGE_FILE_HEADER file_header = (PIMAGE_FILE_HEADER)&nt_header->FileHeader;
	PIMAGE_OPTIONAL_HEADER option_header = (PIMAGE_OPTIONAL_HEADER)&nt_header->OptionalHeader;

	// ������ַ
	DWORD import_fov = RvaToFov(nt_header, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	PIMAGE_IMPORT_DESCRIPTOR import_section = (PIMAGE_IMPORT_DESCRIPTOR)(DWORD)((BYTE*)fileBuffer + import_fov);

	// �����
	SetHexDialogText(hwnd, IDC_EDIT_IMPORT_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_IMPORT_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size);

	// ������
	SetHexDialogText(hwnd, IDC_EDIT_EXPORT_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_EXPORT_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size);

	// ��Դ
	SetHexDialogText(hwnd, IDC_EDIT_SRECOR_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_SRECOR_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size);

	// �쳣
	SetHexDialogText(hwnd, IDC_EDIT_TRY_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_TRY_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size);

	// ��ȫ֤��
	SetHexDialogText(hwnd, IDC_EDIT_ANQUAN_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_ANQUAN_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size);

	// �ض�λ��
	SetHexDialogText(hwnd, IDC_EDIT_CHONG_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_CHONG_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);

	// ������Ϣ
	SetHexDialogText(hwnd, IDC_EDIT_DEBUG_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_DEBUG_SZIE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size);

	// ��Ȩ����
	SetHexDialogText(hwnd, IDC_EDIT_BAN_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_BAN_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size);

	// ȫ��ָ��
	SetHexDialogText(hwnd, IDC_EDIT_POINTRE_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_POINTRE_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size);

	// TLS��
	SetHexDialogText(hwnd, IDC_EDIT_TLS_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_TLS_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size);

	// ��������
	SetHexDialogText(hwnd, IDC_EDIT_JIA_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_JIA_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size);

	// �󶨵���
	SetHexDialogText(hwnd, IDC_EDIT_BANGDING_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_BANGDING_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size);

	// IAT��
	SetHexDialogText(hwnd, IDC_EDIT_IAT_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_IAT_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size);

	// �ӳٵ���
	SetHexDialogText(hwnd, IDC_EDIT_YANCHI_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_YANCHI_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size);

	// COM
	SetHexDialogText(hwnd, IDC_EDIT_COM_RVA, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress);
	SetHexDialogText(hwnd, IDC_EDIT_COM_SIZE, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size);

	// ����
	SetHexDialogText(hwnd, IDC_EDIT_BAOLIU_RVA, 0x00000000);
	SetHexDialogText(hwnd, IDC_EDIT_BAOLIU_SIZE, 0x00000000);

	return FALSE;

}

// ��������
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

	// �������༭����и�ֵ
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
	// ���þ�̬�ؼ����ı�
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
						SetWindowText(hDlg, L"ʱ���->����ʱ��");
						EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), FALSE);
						EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), FALSE);
						EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), TRUE);
					}
					break;
				case IDC_RADIO3:
					if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
					{
						SetWindowText(hDlg, L"����ʱ��->ʱ���");
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
					// ��ȡ��ǰϵͳʱ��
					SYSTEMTIME systemTime;
					GetLocalTime(&systemTime); // ��ȡ��ǰ����ʱ��

					// �� SYSTEMTIME ���õ����� Date Time Picker �ؼ�
					DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), GDT_VALID, &systemTime);
					DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), GDT_VALID, &systemTime);

					// �� SYSTEMTIME ת��Ϊ FILETIME
					FILETIME fileTime;
					SystemTimeToFileTime(&systemTime, &fileTime);

					// �� FILETIME ת��Ϊ Unix ʱ���
					ULARGE_INTEGER ull;
					ull.LowPart = fileTime.dwLowDateTime;
					ull.HighPart = fileTime.dwHighDateTime;

					// FILETIME �Ǵ� 1601 �� 1 �� 1 �տ�ʼ�� 100 ���������ת��Ϊ Unix ʱ���
					const __int64 EPOCH_DIFF = 11644473600LL; // Unix ʱ���� Windows FILETIME ��ʱ���룩
					time_t timestamp = static_cast<time_t>((ull.QuadPart / 10000000ULL) - EPOCH_DIFF);

					// ��ʾ�����ʱ���ֵ���ɸ��������Ҫ��ʾ��
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
	// ���һ����ť�������Ҫ��
	CheckDlgButton(hDlg, IDC_RADIO2, BST_CHECKED);

	if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
	{
		SetWindowText(hDlg, L"ʱ���->����ʱ��");
		EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), FALSE);
	}

	wchar_t Time[0x10];
	HWND button_subsytem = GetDlgItem(hDlg, IDC_EDIT_TIME);
	GetDlgItemText(hParentWnd, IDC_EDIT_TIME, Time, 0x10);
	long value = wcstol(Time, nullptr, 16);
	int intValue = (int)value;

	// �� Unix ʱ���ת��Ϊ FILETIME
	__int64 timeStamp = static_cast<__int64>(intValue) + 11644473600LL; // �� Unix ʱ���ת��Ϊ Windows FILETIME
	timeStamp *= 10000000LL; // ת��Ϊ 100 ���뵥λ

	FILETIME fileTime;
	fileTime.dwLowDateTime = static_cast<DWORD>(timeStamp & 0xFFFFFFFF);
	fileTime.dwHighDateTime = static_cast<DWORD>(timeStamp >> 32);

	// ת��Ϊ SYSTEMTIME
	SYSTEMTIME systemTime;
	FileTimeToSystemTime(&fileTime, &systemTime);

	// �� SYSTEMTIME ���õ����� Date Time Picker �ؼ�
	DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), GDT_VALID, &systemTime);
	DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), GDT_VALID, &systemTime);

	// ��ʾʱ���ֵ�������Ҫ��
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

		// �� Unix ʱ���ת��Ϊ FILETIME
		__int64 timeStamp = static_cast<__int64>(intValue) + 11644473600LL; // �� Unix ʱ���ת��Ϊ Windows FILETIME
		timeStamp *= 10000000LL; // ת��Ϊ 100 ���뵥λ

		FILETIME fileTime;
		fileTime.dwLowDateTime = static_cast<DWORD>(timeStamp & 0xFFFFFFFF);
		fileTime.dwHighDateTime = static_cast<DWORD>(timeStamp >> 32);

		// ת��Ϊ SYSTEMTIME
		SYSTEMTIME systemTime;
		FileTimeToSystemTime(&fileTime, &systemTime);

		// �� SYSTEMTIME ���õ����� Date Time Picker �ؼ�
		DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), GDT_VALID, &systemTime);
		DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), GDT_VALID, &systemTime);

		// ��ʾʱ���ֵ�������Ҫ��
		wchar_t time_value[0x20];
		wsprintf(time_value, L"%d", intValue);
		SetDlgItemText(hDlg, IDC_EDIT1, time_value);
	}
	else if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
	{
		// ��ȡ Date Time Picker 1 �� 2 �� SYSTEMTIME
		SYSTEMTIME st1, st2;
		DateTime_GetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), &st1);
		DateTime_GetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), &st2);

		// �� SYSTEMTIME ת��Ϊ FILETIME
		FILETIME ft1, ft2;
		SystemTimeToFileTime(&st1, &ft1);
		SystemTimeToFileTime(&st2, &ft2);

		// �� FILETIME ת��Ϊ Unix ʱ���
		ULARGE_INTEGER ull1, ull2;
		ull1.LowPart = ft1.dwLowDateTime;
		ull1.HighPart = ft1.dwHighDateTime;
		ull2.LowPart = ft2.dwLowDateTime;
		ull2.HighPart = ft2.dwHighDateTime;

		// FILETIME �Ǵ� 1601 �� 1 �� 1 �տ�ʼ�� 100 ���������ת��Ϊ Unix ʱ���
		const __int64 EPOCH_DIFF = 11644473600LL; // Unix ʱ���� Windows FILETIME ��ʱ���룩
		time_t timestamp1 = static_cast<time_t>((ull1.QuadPart / 10000000ULL) - EPOCH_DIFF);
		time_t timestamp2 = static_cast<time_t>((ull2.QuadPart / 10000000ULL) - EPOCH_DIFF);

		// ���ת��������������������ʾ��ʹ�ã�
		wchar_t buffer[256];
		swprintf_s(buffer, L"%lld\n", timestamp1);
		SetDlgItemText(hDlg, IDC_EDIT1, buffer);
	}
}

void NowTime(HWND hDlg)
{
	// ��ȡ��ǰϵͳʱ��
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime); // ��ȡ��ǰ����ʱ��

	// �� SYSTEMTIME ���õ����� Date Time Picker �ؼ�
	DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER1), GDT_VALID, &systemTime);
	DateTime_SetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER2), GDT_VALID, &systemTime);

	// �� SYSTEMTIME ת��Ϊ FILETIME
	FILETIME fileTime;
	SystemTimeToFileTime(&systemTime, &fileTime);

	// �� FILETIME ת��Ϊ Unix ʱ���
	ULARGE_INTEGER ull;
	ull.LowPart = fileTime.dwLowDateTime;
	ull.HighPart = fileTime.dwHighDateTime;

	// FILETIME �Ǵ� 1601 �� 1 �� 1 �տ�ʼ�� 100 ���������ת��Ϊ Unix ʱ���
	const __int64 EPOCH_DIFF = 11644473600LL; // Unix ʱ���� Windows FILETIME ��ʱ���룩
	time_t timestamp = static_cast<time_t>((ull.QuadPart / 10000000ULL) - EPOCH_DIFF);

	// ��ʾ�����ʱ���ֵ���ɸ��������Ҫ��ʾ��
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
			SetWindowText(hDlg, L"ʱ���->����ʱ��");
			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_RADIO2:
				{
					SetWindowText(hDlg, L"ʱ���->����ʱ��");
					break;
				}
				case IDC_RADIO3:
				{
					SetWindowText(hDlg, L"����ʱ��->ʱ���");
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