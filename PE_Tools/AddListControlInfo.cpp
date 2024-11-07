#include "AddListControlInfo.h"

// ��������----��ȡ���̵�����ģ�鼰�����ַ
VOID EnmuDoubleLitControl(DWORD processID);

VOID AddProcessInfo(HWND hListProcesInfo) {
	// ����б�ؼ�
	ListView_DeleteAllItems(hListProcesInfo);

	// ������ǰϵͳ����
	DWORD processes[1024], processCount, cbNeeded;
	if (!EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
		return;
	}

	processCount = cbNeeded / sizeof(DWORD);

	// ��List Control Process ��ӽ�����Ϣ
	for (DWORD i = 0; i < processCount; i++) {
		if (processes[i] == 0) {
			continue;
		}

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
		if (hProcess) {
			TCHAR processName[MAX_PATH] = L"<Unknown>";
			HMODULE hModule;
			DWORD cbNeeded;

			// ��ȡģ�������
			if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded)) {
				GetModuleBaseName(hProcess, hModule, processName, sizeof(processName) / sizeof(TCHAR));
			}

			// ��ȡ������Ϣ���� PID ����ַ������ַ�;����С
			MODULEINFO modInfo;
			if (GetModuleInformation(hProcess, hModule, &modInfo, sizeof(modInfo))) {
				wchar_t baseAddress[32];
				wchar_t imageSize[32];
				wchar_t pid[16];

				wsprintf(pid, L"%lu", processes[i]);
				wsprintf(baseAddress, L"0x%p", modInfo.lpBaseOfDll);
				wsprintf(imageSize, L"%X", modInfo.SizeOfImage);

				LVITEM item;
				item.mask = LVIF_TEXT;
				item.iItem = ListView_GetItemCount(hListProcesInfo);

				item.iSubItem = 0;
				item.pszText = (LPWSTR)processName;
				SendMessage(hListProcesInfo, LVM_INSERTITEM, 0, (LPARAM)&item);

				item.iSubItem = 1;
				item.pszText = pid;
				SendMessage(hListProcesInfo, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

				item.iSubItem = 2;
				item.pszText = baseAddress;
				SendMessage(hListProcesInfo, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

				item.iSubItem = 3;
				item.pszText = imageSize;
				SendMessage(hListProcesInfo, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);
			}
			CloseHandle(hProcess);
		}
	}
}

VOID AddModulesInfo(HWND hListModuleInfo)
{	
	// ��� ListView
	ListView_DeleteAllItems(hListMoudles);

	DWORD dwRowld;
	TCHAR szPid[0x20];
	LVITEM lv;

	memset(&lv, 0, sizeof(LVITEM));
	memset(szPid, 0, sizeof(szPid));

	dwRowld = SendMessage(hListModuleInfo, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	if (dwRowld == -1)
	{
		MessageBoxW(hListModuleInfo, L"��ѡ�����", L"����", MB_OK);
		return;
	}

	// ���� LVITEM �Ի�ȡ�ı�
	lv.iItem = dwRowld;   // �趨Ϊѡ��������
	lv.iSubItem = 1;      // ���� PID �ڵڶ��У�����1��
	lv.pszText = szPid;   // ָ�򻺳���
	lv.cchTextMax = sizeof(szPid) / sizeof(TCHAR); // ��������С

	SendMessage(hListModuleInfo, LVM_GETITEMTEXT, dwRowld, (LPARAM)&lv);

	EnmuDoubleLitControl(_tcstoul(szPid, nullptr, 10));
}

VOID AddSectionListControl(HWND hListSectionInfo)
{
	// ��� ListView
	ListView_DeleteAllItems(hListSection);
	
	//��ȡPE��Ϣ
	dos_header = (PIMAGE_DOS_HEADER)fileBuffer;
	nt_header = (PIMAGE_NT_HEADERS32)(DWORD)((BYTE*)fileBuffer + dos_header->e_lfanew);
	file_header = (PIMAGE_FILE_HEADER)&nt_header->FileHeader;
	option_header = (PIMAGE_OPTIONAL_HEADER)&nt_header->OptionalHeader;
	sectcion_header = IMAGE_FIRST_SECTION(nt_header);

	DWORD setconNumber = file_header->NumberOfSections;
	for (size_t i = 0; i < setconNumber; i++)
	{
		wchar_t sectionName[9];
		wchar_t sectionAddress[0x16];
		wchar_t sectonSize[0x16];
		wchar_t fileAddresss[0x16];
		wchar_t fileSize[0x16];
		wchar_t Characteristics[0x16];

		MultiByteToWideChar(CP_UTF8, 0, (LPCCH)sectcion_header[i].Name, -1, sectionName, sizeof(sectionName) / sizeof(wchar_t) - 1);
		sectionName[8] = '\0';

		wsprintf(sectionAddress, L"%08X\n", sectcion_header[i].VirtualAddress);
		wsprintf(sectonSize, L"%08X\n", sectcion_header[i].Misc.VirtualSize);
		wsprintf(fileAddresss, L"%08X\n", sectcion_header[i].PointerToRawData);
		wsprintf(fileSize, L"%08X\n", sectcion_header[i].SizeOfRawData);
		wsprintf(Characteristics, L"%08X\n",sectcion_header[i].Characteristics);

		LVITEM item;
		item.mask = LVIF_TEXT;
		item.iItem = ListView_GetItemCount(hListSection);

		item.iSubItem = 0;
		item.pszText = sectionName;
		SendMessage(hListSection, LVM_INSERTITEM, 0, (LPARAM)&item);

		item.iSubItem = 1;
		item.pszText = sectionAddress;
		SendMessage(hListSection, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

		item.iSubItem = 2;
		item.pszText = sectonSize;
		SendMessage(hListSection, LVM_SETITEMTEXT,item.iItem, (LPARAM)&item);

		item.iSubItem = 3;
		item.pszText = fileAddresss;
		SendMessage(hListSection, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

		item.iSubItem = 4;
		item.pszText = fileSize;
		SendMessage(hListSection, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

		item.iSubItem = 5;
		item.pszText = Characteristics;
		SendMessage(hListSection, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);
	}
	wchar_t sectionNumber[0x20];
	wsprintf(sectionNumber, L"%d\n", setconNumber);
	SetDlgItemText(hListSectionInfo, IDC_STATIC_SECTION_NUMBER, sectionNumber);
}

VOID EnmuDoubleLitControl(DWORD processID)
{
	// ��Ŀ�����
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	if (hProcess == nullptr) {
		MessageBoxW(hListMoudles, L"�޷��򿪽���", L"����", MB_OK);
		return;
	}

	// ö��ģ��
	HMODULE hModules[1024];
	DWORD cbNeeded;
	if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded)) {
		DWORD moduleCount = cbNeeded / sizeof(HMODULE);

		for (DWORD i = 0; i < moduleCount; i++)
		{
			TCHAR moduleName[MAX_PATH];
			if (GetModuleFileNameEx(hProcess, hModules[i], moduleName, sizeof(moduleName) / sizeof(TCHAR)))
			{
				wchar_t moduleBase[0x100];

				// ��ȡģ�����ַ
				MODULEINFO modInfo;
				if (GetModuleInformation(hProcess, hModules[i], &modInfo, sizeof(modInfo)))
				{

					swprintf(moduleBase, sizeof(moduleBase) / sizeof(wchar_t), L"0x%p", modInfo.lpBaseOfDll);
				}
				else
				{
					// ��ȡ�������
					DWORD error = GetLastError();
					wchar_t errorMessage[0x100];
					swprintf(errorMessage, sizeof(errorMessage) / sizeof(wchar_t), L"GetModuleInformation ʧ��,�������: %lu", error);
					MessageBox(hListMoudles, errorMessage, TEXT("����"), MB_OK);
				}

				// ��ȡģ������
				wchar_t moduleName_temp[MAX_PATH];
				wchar_t* nameStart = wcsrchr(moduleName, L'\\');

				if (nameStart) {
					wcscpy_s(moduleName_temp, nameStart + 1);
				}
				else {
					wcscpy_s(moduleName_temp, moduleName);
				}

				LVITEM item;
				item.mask = LVIF_TEXT;
				item.iItem = ListView_GetItemCount(hListMoudles);

				item.iSubItem = 0;
				item.pszText = (LPWSTR)moduleName_temp;
				SendMessage(hListMoudles, LVM_INSERTITEM, 0, (LPARAM)&item);

				item.iSubItem = 1;
				item.pszText = moduleBase;
				SendMessage(hListMoudles, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);


			}
			else {
				MessageBoxW(hListMoudles, TEXT("����"), TEXT("TIP"), MB_OK);
			}
		}
	}
	else {
		// ��ȡ�������
		DWORD error = GetLastError();
		wchar_t errorMessage[0x100];
		swprintf(errorMessage, sizeof(errorMessage) / sizeof(wchar_t), L"GetModuleInformation ʧ��,�������: %lu", error);
		MessageBox(hListMoudles, errorMessage, TEXT("����"), MB_OK);
	}

	// �رս��̾��
	CloseHandle(hProcess);
}

VOID AddImportList(HWND hListImoprtControl)
{
	// ��� ListView
	ListView_DeleteAllItems(hListImport);

	//��ȡPE��Ϣ
	dos_header = (PIMAGE_DOS_HEADER)fileBuffer;
	nt_header = (PIMAGE_NT_HEADERS32)(DWORD)((BYTE*)fileBuffer + dos_header->e_lfanew);
	file_header = (PIMAGE_FILE_HEADER)&nt_header->FileHeader;
	option_header = (PIMAGE_OPTIONAL_HEADER)&nt_header->OptionalHeader;
	sectcion_header = IMAGE_FIRST_SECTION(nt_header);

	// ������ַ
	DWORD import_fov = RvaToFov(nt_header, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	import_section = (PIMAGE_IMPORT_DESCRIPTOR)(DWORD)((BYTE*)fileBuffer + import_fov);

	for (size_t i = 0; import_section[i].Characteristics != 0; i++)
	{
		wchar_t DllName[0x20];
		BYTE* name = (BYTE*)fileBuffer + RvaToFov(nt_header, import_section[i].Name);
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, DllName, sizeof(DllName) / sizeof(wchar_t), (const char*)name, _TRUNCATE);

		wchar_t original_thunk[0x20];
		wchar_t Time[0x20];
		wchar_t forwarder[0x20];
		wchar_t nameRva[0x20];
		wchar_t firsttunk[0x20];

		wsprintf(original_thunk, L"%08X\n", import_section[i].OriginalFirstThunk);
		wsprintf(Time, L"%08X\n", import_section[i].TimeDateStamp);
		wsprintf(forwarder, L"%08X\n", import_section[i].ForwarderChain);
		wsprintf(nameRva, L"%08X\n", import_section[i].Name);
		wsprintf(firsttunk, L"%08X\n", import_section[i].FirstThunk);

		LVITEM item;
		item.mask = LVIF_TEXT;
		item.iItem = ListView_GetItemCount(hListImport);

		item.iSubItem = 0;
		item.pszText = DllName;
		SendMessage(hListImport, LVM_INSERTITEM, 0, (LPARAM)&item);

		item.iSubItem = 1;
		item.pszText = original_thunk;
		SendMessage(hListImport, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

		item.iSubItem = 2;
		item.pszText = Time;
		SendMessage(hListImport, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

		item.iSubItem = 3;
		item.pszText = forwarder;
		SendMessage(hListImport, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

		item.iSubItem = 4;
		item.pszText = nameRva;
		SendMessage(hListImport, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

		item.iSubItem = 5;
		item.pszText = firsttunk;
		SendMessage(hListImport, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

	}
}

VOID AddImportInfo(HWND HListImportInfo, HWND hDlg)
{
	// ��� ListView
	ListView_DeleteAllItems(hListImportInfo);
	DWORD dwRowld;
	TCHAR OriginalFirstThunk[0x20];
	LVITEM lv;

	memset(&lv, 0, sizeof(LVITEM));
	memset(OriginalFirstThunk, 0, sizeof(OriginalFirstThunk));

	dwRowld = SendMessage(hListImport, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
	
	// ��ȡѡ���ж�Ӧ�� DLL ����
	wchar_t selectedDllName[0x20];

	LVITEM lvItem;
	memset(&lvItem, 0, sizeof(LVITEM));
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = dwRowld;
	lvItem.iSubItem = 0; // DLL ������
	lvItem.pszText = selectedDllName;
	lvItem.cchTextMax = sizeof(selectedDllName);
	SendMessage(hListImport, LVM_GETITEMTEXT, dwRowld, (LPARAM)&lvItem);

	// �������������ѡ���ж�Ӧ�ĵ�����
	DWORD import_fov = RvaToFov(nt_header, option_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	auto import_section = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)fileBuffer + import_fov);
	
	int index = 0;
	int thunkNumber = 0;

	while (import_section[index].OriginalFirstThunk != 0)
	{
		// ��ȡ��ǰ DLL ����
		wchar_t dllName[0x20];
		BYTE* name = (BYTE*)fileBuffer + RvaToFov(nt_header, import_section[index].Name);
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, dllName, sizeof(dllName) / sizeof(wchar_t), (const char*)name, _TRUNCATE);

		if (wcscmp(selectedDllName, dllName) == 0) {
			// �ҵ���ѡ�е� DLL�����������Ϣ
			auto* FunctionAddress = (DWORD*)((BYTE*)fileBuffer + RvaToFov(nt_header, import_section[index].OriginalFirstThunk));
			while (*FunctionAddress != 0) {
				thunkNumber++;
				wchar_t thunk_rva[0x20];
				wchar_t thunk_offset[0x20];
				wchar_t thunk_value[0x20];
				wchar_t hint[0x20] = L"-";
				wchar_t apiName[0x20];

				auto currentRVA = (DWORD)((BYTE*)FunctionAddress - (BYTE*)fileBuffer);
				wsprintf(thunk_rva, L"%08X\n", currentRVA);
				wsprintf(thunk_offset, L"%08X\n", RvaToFov(nt_header, currentRVA));
				wsprintf(thunk_value, L"%08X\n", *FunctionAddress);

				if ((*FunctionAddress & IMAGE_ORDINAL_FLAG32)) {
					wsprintf(apiName, L"�������: %d", (*FunctionAddress & ~IMAGE_ORDINAL_FLAG32));
				}
				else {
					auto importByName = (PIMAGE_IMPORT_BY_NAME)((BYTE*)fileBuffer + RvaToFov(nt_header, *FunctionAddress));
					wsprintf(hint, L"%04X\n", importByName->Hint);
					MultiByteToWideChar(CP_ACP, 0, importByName->Name, -1, apiName, sizeof(apiName) / sizeof(wchar_t));
				}

				LVITEM item;
				item.mask = LVIF_TEXT;
				item.iItem = ListView_GetItemCount(hListImportInfo);

				item.iSubItem = 0;
				item.pszText = thunk_rva;
				SendMessage(hListImportInfo, LVM_INSERTITEM, 0, (LPARAM)&item);

				item.iSubItem = 1;
				item.pszText = thunk_offset;
				SendMessage(hListImportInfo, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

				item.iSubItem = 2;
				item.pszText = thunk_value;
				SendMessage(hListImportInfo, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

				item.iSubItem = 3;
				item.pszText = hint;
				SendMessage(hListImportInfo, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

				item.iSubItem = 4;
				item.pszText = apiName;
				SendMessage(hListImportInfo, LVM_SETITEMTEXT, item.iItem, (LPARAM)&item);

				FunctionAddress++;
			}
			break;
		}
		index++;
	}
	wchar_t ThunkNumber[0x20];
	wsprintf(ThunkNumber, L"%d\n", thunkNumber);
	SetDlgItemText(hDlg, IDC_STATIC_THUNK_NUMBER, ThunkNumber);
} 


VOID AddTeZheng(HWND hDlg, HWND hFaterHdlg)
{
	char zz[] = {
			0x0001 ,
	0x0002,
	0x0004 ,
	0x0008 ,
	0x0010 ,
	0x0020 ,
	0x0080 ,
	0x0100 ,
	0x0200 ,
	0x0400 ,
	0x0800 ,
	0x1000 ,
	0x2000 ,
	0x4000 ,
	0x8000,
	};
	wchar_t num[0x20];
	HWND TEZHNEG = GetDlgItem(hFaterHdlg, IDC_EDIT_TEZHNEG);
	GetWindowText(TEZHNEG, num, 0x20);

	unsigned int  featureValue = 0;
	swscanf_s(num, L"%x", &featureValue);
	for (int i = 0; i < 14; ++i)
	{
		if (featureValue & zz[i])
		{
			HWND hCheckBox = GetDlgItem(hDlg, IDC_CHECK1 + i);
			SendMessage(hCheckBox, BM_SETCHECK, BST_CHECKED, 0);
		}
	}
}