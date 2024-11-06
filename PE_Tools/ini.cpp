#include "framework.h"

HWND hListMoudles; 
HWND hListSection;
HWND hListImport;
HWND hListImportInfo;
HINSTANCE PEhInst;
DWORD* fileBuffer;

PIMAGE_DOS_HEADER dos_header;
PIMAGE_NT_HEADERS32 nt_header;
PIMAGE_FILE_HEADER file_header;
PIMAGE_OPTIONAL_HEADER option_header;
PIMAGE_SECTION_HEADER sectcion_header;
PIMAGE_IMPORT_DESCRIPTOR import_section;