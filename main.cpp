#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>

#include "game.h"

#define ID_EDIT 1001
#define ID_BUTTON 1002

LRESULT CALLBACK MyFunc(HWND, UINT, WPARAM, LPARAM);
void showConsole();
void drawGreen (const int , const int, HWND );
void drawBoard(HWND, int, const bool, int &, int & );

int fromCoordinateToPosx ( const int );
int fromCoordinateToPosy ( const int );

char szWinName[] = "MyWin";

int H;
vector < vector < int > > checkers;
int condition;
int cond;

int main()
{
	showConsole();
	printf("I began \n");
		
	checkers.resize(8);
	for ( int i = 0; i < 8; ++i){
		checkers[i].resize(8);
	}

	HWND hWnd;
	MSG msg;
	WNDCLASS wcl;
	memset ( &wcl, 0, sizeof(WNDCLASS));

	HWND hButton1, hButton2;

	wcl.lpszClassName = "MyWindow";
	wcl.lpfnWndProc = MyFunc;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);

	wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

	if(!RegisterClass(&wcl)) return 0;

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
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE );

	WSADATA ws;
 	int error;

 	WORD wVersionRequeested = MAKEWORD(2,2);
 	if (FAILED ( WSAStartup ( wVersionRequeested, &ws) ) ) 
   	{
   		printf("WSAStartup faild with error:%d\n",error);
   		error = WSAGetLastError();
   	}

   	SOCKET s;
   	s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
   	if ( s == INVALID_SOCKET )
   	{
		printf( "Error at socket():%ld\n", WSAGetLastError() );
		WSACleanup();
		return 1;
	}

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;			//IP adress family
	clientService.sin_port = htons(27016);		//port
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");//IP

   // Дальше выполняем соединение:
	if (SOCKET_ERROR == ( connect (s, (sockaddr *) &clientService, sizeof (clientService) ) ) )
	{
		printf("Failed to connect.\n");
		closesocket(s);
		error = WSAGetLastError();
		WSACleanup();
		return 2;
	}
	printf("Connect to server \n");

	int iResult;
	char color[1];

	iResult = recv( s, color, 1, 0);
	while (true){
		if ( iResult > 0){
			if ( color[0] == 1 ){
				startGame ( true, checkers, condition);
				cout<<"My color is white"<<endl;
			}
			else {
				startGame ( false, checkers, condition);
				cout<<"My color is black"<<endl;
			}
			break;
		}
	}	
	
	const int buflen = 64;
	
	char recvbuf [buflen];
	char sendbuf [buflen];
	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage ( &msg );
		DispatchMessage ( &msg );
		/*
		if (cond == 2){
			iResult = recv( s, recvbuf, buflen, 0);
			if ( iResult > 0 ){
				printf("Bytes received:%d\n",iResult);
				printf("Result: %d\n",recvbuf[0]);
			}
			else if ( iResult == 0 )
				printf("Connection closed\n");
			else
            	printf("recv failed: %d\n", WSAGetLastError());
    	}
    	*/
	}
	
	system("pause");
	return msg.wParam;
}

int pos_x = -1, pos_y = -1; 
bool is_choose = false;

LRESULT CALLBACK MyFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch(message) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
	{
		if ( condition > 0){
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			pos_x = fromCoordinateToPosx(x);
			if ( pos_x < 0 ) 
				break;
			pos_y = fromCoordinateToPosy(y);
			if ( pos_y < 0 ) 
				break;
			if ( is_choose )
			{
				makeStep ( pos_x, pos_y, checkers );
				is_choose = false;
				InvalidateRect ( hwnd, NULL, 1);
				break;
			}
			is_choose = chooseChecker ( pos_x, pos_y, checkers);
			InvalidateRect ( hwnd, NULL, 1);
			break;
		}
	}
	case WM_PAINT:
		//if ( condition > 0 ){
			drawBoard(hwnd, 400, is_choose, pos_x, pos_y);
		//}
		/*else if( condition == 0 )
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			TextOut ( hdc, 200, 190, "Write ip: ", 10);
			EndPaint ( hwnd, &ps );			
		}
		*/
		break;
	} 

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void drawGreen (const int x, const int y, HWND hwnd){
	cout<< "green"<<endl;
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	RECT r; 
	const int d = 10;
	const int H = 50;

	r.left = d + x * H;
	r.top = d + (7-y) * H; 
	r.right = d + H * ( x + 1) ; 
	r.bottom = d + H * ( 7 - y + 1) ;
	cout<<r.left<<" "<<r.top<<" "<<r.right<<" "<<r.bottom<<endl;

	FillRect ( hdc, &r, (HBRUSH)CreateSolidBrush( RGB ( 0, 255, 0 ) ) );
	return;
}
void drawBoard(HWND hwnd, int a, const bool is_choose, int& pos_x, int& pos_y) 			// рисует доску размером х*х
{
	H = a / 8;
	const int d = 10;						// отступ от верхнего левого угла

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	RECT r; 								//объявляем экзмепляр структуры RECT - координаты прямоугольника.
	
	TextOut ( hdc, 10, 410, "a", 1);
	TextOut ( hdc, 60, 410, "b", 1);
	TextOut ( hdc, 110, 410, "c", 1);
	TextOut ( hdc, 160, 410, "d", 1);
	TextOut ( hdc, 210, 410, "e", 1);
	TextOut ( hdc, 260, 410, "f", 1);
	TextOut ( hdc, 310, 410, "g", 1);
	TextOut ( hdc, 360, 410, "h", 1);

	TextOut ( hdc, 410, 370, "1", 1);
	TextOut ( hdc, 410, 320, "2", 1);
	TextOut ( hdc, 410, 270, "3", 1);
	TextOut ( hdc, 410, 220, "4", 1);
	TextOut ( hdc, 410, 170, "5", 1);
	TextOut ( hdc, 410, 120, "6", 1);
	TextOut ( hdc, 410, 70, "7", 1);
	TextOut ( hdc, 410, 20, "8", 1);

	for ( int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (pos_x == i && pos_y == j ){
				if ( is_choose ){
					r.left = d + (i) * H;
					r.top = d + H * (j); 
					r.right = d + H * (i + 1) ; 
					r.bottom = d + H * (j+1) ;
					FillRect(hdc, &r, (HBRUSH)CreateSolidBrush( RGB ( 0, 250, 0 ) ) );
					pos_x = pos_y = -1;
					continue;
				}
			}

			if (!(i%2 && j%2 || !(i%2) && !(j%2)))
			{
				r.left = d + (i) * H;
				r.top = d + H * (j); 
				r.right = d + H * (i + 1) ; 
				r.bottom = d + H * ( j + 1) ;
				FillRect(hdc, &r, (HBRUSH)CreateSolidBrush( RGB ( 50, 50, 50 ) ) );
			
			}
		}	
	}

	HBRUSH brush_b, brush_w;
	brush_w = CreateSolidBrush( RGB(255, 255, 255) ) ;
	brush_b = CreateSolidBrush( RGB( 0, 0, 0) );

	int x, y;


	for ( int i = 0 ; i < 8 ; ++i )
	{
		for (int j = 0; j < 8; ++j ) 
		{
			if ( checkers[i][j] == 1 ) 
			{
				x =  (j)*H + d;
				y =  (7-i)*H + d;

				SelectObject (hdc, brush_w);
				Ellipse ( hdc, x, y, x+H, y+H); 
			}
			else if( checkers[i][j] == 2 )
			{
				x =  (j)*H + d;
				y =  (7-i)*H + d;
				
				SelectObject(hdc, brush_b);
				Ellipse ( hdc, x, y, x+H, y+H); 
			}
		}
	}
	EndPaint ( hwnd, &ps );

	return;
}

int fromCoordinateToPosx ( const int x )						// возвращает -1 если вне доски
{
	int pos = -1;
	for ( int i = 0; i < 8; ++i){
		if ( x <= 10 + H * (i + 1) && x >= 10 + H * i ){
			pos = i;
			break;
		}
	}
	return pos;
}

int fromCoordinateToPosy ( const int x )						// возвращает -1 если вне доски
{
	int pos = -1;
	for ( int i = 0; i < 8; ++i){
		if ( x <= 10 + H * (7 - i + 1) && x >= 10 + H * (7-i) ){
			pos =  7-i;
			break;
		}
	}
	return pos;
}

void showConsole(){
	BOOL f = AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

}