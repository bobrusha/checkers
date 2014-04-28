#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"

LRESULT CALLBACK MyFunc(HWND, UINT, WPARAM, LPARAM);

void drawBoard(HWND, int);

char szWinName[] = "MyWin";

//int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode)

int main()
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wcl;
	memset(&wcl, 0, sizeof(WNDCLASS));

	HWND hButton1, hButton2;

	wcl.lpszClassName = "MyWindow";
	wcl.lpfnWndProc = MyFunc;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);

	wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

	if(!RegisterClass(&wcl)) return 0;

	// MSDN.microsoft.com/ 
	hWnd = CreateWindow(
		"MyWindow",
		"Checkers", 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		600 /*CW_USEDEFAULT*/,
		500 /*CW_USEDEFAULT*/,
		HWND_DESKTOP, NULL, 
		NULL/*hThisInst*/, NULL);

	ShowWindow(hWnd, SW_SHOW /*nWinMode*/);
	UpdateWindow(hWnd);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK MyFunc(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		drawBoard(hwnd, 400);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void drawBoard(HWND hwnd, int x) // рисует доску размером х*х
{
	const int h = x / 8;
	const int d = 10; // отступ от верхнего левого угла
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	RECT r; //объявляем экзмепляр структуры RECT - координаты прямоугольника.
	
	for ( int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (!(i%2 && j%2 || !(i%2) && !(j%2)))
			{
				r.left = d + i * h;
				r.top = d + h * j; 
				r.right = d + h * (i + 1) ; 
				r.bottom = d + h * (j + 1) ;
				FillRect(hdc, &r, (HBRUSH)CreateSolidBrush(RGB(0,0,0)));
			}
		}
	}
	return;
}
