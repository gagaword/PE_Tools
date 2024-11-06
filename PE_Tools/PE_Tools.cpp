// PE_tools.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "PE_tools.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    MessageBox(0,0,0,0);
	PEhInst = hInstance;
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), nullptr, DiaMainProc);
      
}




