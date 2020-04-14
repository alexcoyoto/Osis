// Lab3.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Lab3.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
void				InitControls(HWND);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAB3, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB3));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB3));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(3);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 820, 420, NULL, NULL, hInstance, NULL);

   InitControls(hWnd);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#define BUTTON_DRAW_ID 1
#define BUTTON_CLEAR_ID 2

void InitControls(HWND hWnd) {
	HWND buttonAdd = CreateWindow(L"BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
		20, 320, 60, 60, hWnd, (HMENU)BUTTON_DRAW_ID, NULL, NULL);
	HWND buttonClear = CreateWindow(L"BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
		90, 320, 60, 60, hWnd, (HMENU)BUTTON_CLEAR_ID, NULL, NULL);
}

bool pictureVisible = false;

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, i;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case BUTTON_DRAW_ID:
			pictureVisible = true;
			InvalidateRect(hWnd, NULL, true);
			break;
		case BUTTON_CLEAR_ID:
			pictureVisible = false;
			InvalidateRect(hWnd, NULL, true);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DRAWITEM:
	{
		DRAWITEMSTRUCT *draw = (DRAWITEMSTRUCT*)lParam;
		HBRUSH brush;
		HPEN pen;

		switch (draw->CtlID)
		{
			case BUTTON_DRAW_ID:
				{
					HDC hdc = draw->hDC;

					if (draw->itemAction == ODA_SELECT) {
						brush = CreateSolidBrush(RGB(0, 0, 255));
					}
					else {
						brush = CreateSolidBrush(RGB(0, 255, 0));
					}

					
					pen = CreatePen(BS_SOLID, 2, RGB(0, 0, 0));

					SelectObject(hdc, brush);
					SelectObject(hdc, pen);

					Ellipse(hdc, 0, 0, 60, 60);
					MoveToEx(hdc, 10, 30, NULL);
					LineTo(hdc, 50, 30);
					MoveToEx(hdc, 30, 10, NULL);
					LineTo(hdc, 30, 50);
				}
				break;
			case BUTTON_CLEAR_ID:
				{
					HDC hdc = draw->hDC;

					if (draw->itemAction == ODA_SELECT) {
						brush = CreateSolidBrush(RGB(0, 0, 255));
					}
					else {
						brush = CreateSolidBrush(RGB(255, 0, 0));
					}
					pen = CreatePen(BS_SOLID, 2, RGB(0, 0, 0));

					SelectObject(hdc, brush);
					SelectObject(hdc, pen);

					Ellipse(hdc, 0, 0, 60, 60);
					MoveToEx(hdc, 20, 20, NULL);
					LineTo(hdc, 40, 40);
					MoveToEx(hdc, 40, 20, NULL);
					LineTo(hdc, 20, 40);
				}
				break;
		default:
			break;
		}

	}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...

		HBRUSH brush;
		HPEN pen;
		if (pictureVisible)
		{
			brush = CreateSolidBrush(RGB(50, 50, 50));
			SelectObject(hdc, brush);

			Ellipse(hdc, 600, 10, 620, 40);
			Ellipse(hdc, 620, 51, 640, 40);
			Ellipse(hdc, 640, 15, 660, 40);
			Ellipse(hdc, 660, 68, 680, 40);
			Ellipse(hdc, 582, 10, 620, 40);
			
			brush = CreateSolidBrush(RGB(10, 0, 50));
			SelectObject(hdc, brush);
			pen = CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
			SelectObject(hdc, pen);

			POINT p[18] = { 35, 120, 241, 120, 140, 140, 432, 140,
				240, 120, 640, 120, 432, 240, 800, 280, 120, 405, 356, 64, 264, 362 };
			Polygon(hdc, p, 18);

			pen = CreatePen(BS_SOLID, 1, RGB(0, 0, 0));
			SelectObject(hdc, pen);
			brush = CreateSolidBrush(RGB(0, 354, 86));
			SelectObject(hdc, brush);

			Rectangle(hdc, 10, 23, 60, 30);
			POINT p2[3] = { 640, 120, 720, 180, 640, 180 };
			Polygon(hdc, p2, 2);

			pen = CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
			SelectObject(hdc, pen);

			MoveToEx(hdc, 0, 40, NULL);
			LineTo(hdc, 620, 40);

			pen = CreatePen(BS_SOLID, 5, RGB(0, 356, 621));
			SelectObject(hdc, pen);

			MoveToEx(hdc, 200, 40, NULL);
			LineTo(hdc, 60, 52);
			LineTo(hdc, 2, 14);

			MoveToEx(hdc, 0, 300, NULL);
			LineTo(hdc, 820, 300);

			pen = CreatePen(BS_SOLID, 7, RGB(255, 255, 255));
			SelectObject(hdc, pen);

			Arc(hdc,241, 200, 532, 525, 420, 280, 20, 200);
			Arc(hdc, 360, 230, 234, 453, 560, 230, 360, 200);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}