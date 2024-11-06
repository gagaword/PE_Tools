#pragma once
#include "framework.h"

extern HINSTANCE PEhInst;

// 主对话框
BOOL CALLBACK DiaMainProc(HWND hWnd, UINT message, WPARAM wParm, LPARAM lParam);

// PE区段对话框
BOOL CALLBACK PeSectionProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// 关于对话框函数
BOOL CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// PE信息查看对话框
BOOL CALLBACK PeFuncProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


BOOL CALLBACK idm_time_proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// 数据目录对话框
BOOL CALLBACK PeDataProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK SubSytemProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK ImportInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK TimeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// 打开文件对话框
BOOL OpenFileDialog(HWND hWnd, TCHAR* szFile, DWORD size);

// PE信息处理
BOOL PeInfo(HWND hDlg, TCHAR* filePath);

// DATA信息处理
BOOL AddDataInfo(HWND hwnd);


