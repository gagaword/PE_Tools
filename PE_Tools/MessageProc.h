#pragma once
#include "framework.h"

extern HINSTANCE PEhInst;

// ���Ի���
BOOL CALLBACK DiaMainProc(HWND hWnd, UINT message, WPARAM wParm, LPARAM lParam);

// PE���ζԻ���
BOOL CALLBACK PeSectionProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ���ڶԻ�����
BOOL CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// PE��Ϣ�鿴�Ի���
BOOL CALLBACK PeFuncProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


BOOL CALLBACK idm_time_proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ����Ŀ¼�Ի���
BOOL CALLBACK PeDataProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK SubSytemProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK ImportInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK TimeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ���ļ��Ի���
BOOL OpenFileDialog(HWND hWnd, TCHAR* szFile, DWORD size);

// PE��Ϣ����
BOOL PeInfo(HWND hDlg, TCHAR* filePath);

// DATA��Ϣ����
BOOL AddDataInfo(HWND hwnd);


