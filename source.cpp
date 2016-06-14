#include <sdkddkver.h>
// Macro used to reduce namespace pollution
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//#include<rope.h>
//#include<rope>
#define _CRT_SECURE_NO_WARNINGS
#include<string.h>
#include<stdlib.h>
#include <Windows.h>
#include <commctrl.h>
#include"resource.h"
#include <tchar.h>
#include<stdio.h>
//#include "functions1.h"
#include<commdlg.h>


char fisierOpen[256];
char fisierSave[256];

HWND hEdit;
RECT dim;
HANDLE hOpenFile, hSaveFile;
OPENFILENAME ofn,sfn;

LRESULT CALLBACK MessageProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{

	switch (Msg)
	{

	case WM_CREATE:{

		GetClientRect(hWnd, &dim);
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_HSCROLL | WS_VSCROLL,
			dim.left, dim.top, dim.right, dim.bottom, hWnd, NULL, GetModuleHandle(NULL), NULL);

		//creeare font 
		HFONT hf;
		HDC hdc;
		long lfHeight;
		hdc = GetDC(NULL);
		lfHeight = -MulDiv(13, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		ReleaseDC(NULL, hdc);
		hf = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
		//HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hf, TRUE);

		break;
	}
	case WM_SIZE:
		GetClientRect(hWnd, &dim);
		MoveWindow(hEdit, dim.left, dim.top, abs(dim.right - dim.left), abs(dim.top - dim.bottom), TRUE);

	case WM_COMMAND:

		switch (LOWORD(wParam)){


		case ID_DESPRE:
			TCHAR mesajAbout[500];
			LoadString(NULL, IDS_STRING102, mesajAbout, sizeof(TCHAR) * 500);
			MessageBox(NULL, mesajAbout, "Despre", MB_OK);
			break;

		case ID_FISIER_NOU:{

			SetWindowText(hEdit, NULL);
			if (hSaveFile != NULL) CloseHandle(hSaveFile);
			if (hOpenFile != NULL) CloseHandle(hOpenFile);
			break;
		}

		case ID_FISIER_SALVEAZA_CA:{
ETICHETA:			
			sfn.lStructSize = sizeof(sfn);
			sfn.lpstrFile = fisierSave;
			sfn.lpstrFile[0] = '\0';
			sfn.hwndOwner = hWnd;
			sfn.nMaxFile = sizeof(fisierSave);
			sfn.lpstrFilter = TEXT("txt Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
			sfn.nFilterIndex = 1;
			sfn.lpstrInitialDir = NULL;
			sfn.lpstrFileTitle = NULL;
			sfn.lpstrDefExt = "txt";
			sfn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			
			GetSaveFileName(&sfn);
			
			DWORD aux = 0;
			int length = GetWindowTextLength(hEdit);
			char* buffer = new char[length + 1];
			GetWindowText(hEdit, buffer, length+1);

			hSaveFile = CreateFile(sfn.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			WriteFile(hSaveFile, buffer, length, &aux, NULL);
            

			if (hOpenFile != NULL) CloseHandle(hSaveFile);

			break; 
		}
		case ID_FISIER_SALVEAZA:{

			DWORD aux = 0;
			int length = GetWindowTextLength(hEdit);
			char* buffer = new char[length + 1];
			GetWindowText(hEdit, buffer, length + 1);
			
			if (hOpenFile!=NULL) {

				CloseHandle(hOpenFile);
				hOpenFile = CreateFile(fisierOpen, GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				WriteFile(hOpenFile, buffer, length, &aux, NULL);

			}
			else if (hSaveFile != NULL){

				CloseHandle(hSaveFile);
				hSaveFile = CreateFile(fisierSave, GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				WriteFile(hSaveFile, buffer, length, &aux, NULL);

			}
			else goto ETICHETA;
			break;
		}
		case ID_FISIER_DESCHIDE:{
			
			DWORD szOpen;
			DWORD aux = 0;

			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = fisierOpen;
			ofn.lpstrFile[0] = '\0';
			ofn.hwndOwner = hWnd;
			ofn.nMaxFile = sizeof(fisierOpen);
			ofn.lpstrFilter = TEXT("txt Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrInitialDir = NULL;
			ofn.lpstrFileTitle = NULL;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn)){

				hOpenFile = CreateFile(fisierOpen, GENERIC_ALL, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				szOpen = GetFileSize(hOpenFile, NULL);
				char *buffer = new char[szOpen + 1];
				buffer[szOpen] = 0;
				ReadFile(hOpenFile, buffer, szOpen, &aux, NULL);
				SetWindowText(hEdit, buffer);

			}
			break;
		}
		case ID_INCHIDERE:
			PostQuitMessage(WM_QUIT);
			return 0;
			break;


		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		//break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{

	const size_t MAX_LOADSTRING = 100;
	TCHAR className[MAX_LOADSTRING];
	TCHAR title[MAX_LOADSTRING];

	HICON icon, iconSmall;
	

	LoadString(hInstance, IDS_MYAPP_CLASS, className, sizeof(TCHAR)*MAX_LOADSTRING);
	LoadString(hInstance, IDS_MYAPP_TITLE, title, sizeof(TCHAR)*MAX_LOADSTRING);

	icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	iconSmall = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	// Setup window class attributes.
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(wcex); // WNDCLASSEX size in bytes
	wcex.style = CS_HREDRAW | CS_VREDRAW;      // Window class styles
	wcex.lpszClassName = className;   // Window class name
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);   // Window background brush color.
	wcex.hCursor = NULL; // Window cursor
	wcex.lpfnWndProc = MessageProcedure;     // Window procedure associated to this window class.
	wcex.hInstance = hInstance;    // The application instance.
	wcex.hIcon = icon;
	wcex.hIconSm = iconSmall;
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);


	if (!RegisterClassEx(&wcex))
		return 1;

	// Setup window initialization attributes.
	CREATESTRUCT cs;
	ZeroMemory(&cs, sizeof(cs));

	cs.x = 350;    // Window X position
	cs.y = 150;    // Window Y position
	cs.cx = 640;  // Window width
	cs.cy = 480;  // Window height
	cs.hInstance = hInstance; // Window instance.
	cs.lpszClass = wcex.lpszClassName;       // Window class name
	cs.lpszName = title;  // Window title
	cs.style = WS_OVERLAPPEDWINDOW;      // Window style

	HWND hWnd = CreateWindowEx(cs.dwExStyle, cs.lpszClass, cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy, cs.hwndParent,
		cs.hMenu, cs.hInstance, cs.lpCreateParams);

	if (!hWnd)
		return 1;


	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);


	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClass(wcex.lpszClassName, hInstance);

	return msg.wParam;
}
