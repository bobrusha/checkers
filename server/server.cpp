//#pragma comment(lib,"ws_32.lib")
#include <stdio.h>
#include <winsock2.h>
//#include <time.h>

#define DEFAULT_BUFLEN 512

using namespace std;


int main() 
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup ( wVersionRequested, &wsaData );
	if ( err != 0){
		printf("WSAStartup failed with error: %d\n",err);
		return 1;
	}

	SOCKET s;
	s = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	
	if ( s == INVALID_SOCKET ) {
		printf( "Error at socket(): %ld\n", WSAGetLastError() );
		WSACleanup();
		return 2;
	}
	
	sockaddr_in service;
	service.sin_family = AF_INET;
    service.sin_port = htons (27016);
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if ( bind ( s, (sockaddr *)&service, sizeof(service))) 
	{
		printf( "bind() failed.\n" );
		closesocket (s);
		WSACleanup();
		return 3;
	}

	if ( listen(s, SOMAXCONN) == SOCKET_ERROR)
	{
		printf ( "Error listening on socket.\n" );
		closesocket (s);
		WSACleanup();
		return 4;
	}

	SOCKET AcceptSocket1;
	SOCKET AcceptSocket2;

	int bytes = 1;
	char *sendbuf = new char[bytes]; 
	int iResult;
	bool first_client = true;
	bool second_client = true;

	while ( first_client ){
		printf("Waiting for first client to connect...\n");
		sockaddr_in cliaddr1;
		int length1 = sizeof(cliaddr1);
		AcceptSocket1 = accept( s, (sockaddr*) &cliaddr1, &length1);
			
		if ( AcceptSocket1 == INVALID_SOCKET ){
			printf ( "accept failed:%d\n", WSAGetLastError() );
			closesocket(s);
			WSACleanup();
			return 5;
		}
		else 
		{
			PHOSTENT phost1;
			printf ( "First client connect:\n" );
			phost1 = gethostbyaddr ( (char*) &cliaddr1.sin_addr, 4, AF_INET);		//отправляет цвет
			printf ( "%s %s Port:%d\n", phost1->h_name, inet_ntoa ( cliaddr1.sin_addr ), cliaddr1.sin_port );
			char color[1];
			color[0] = 1;
			iResult = send ( AcceptSocket1, color, 1, 0 );
			printf("bytes sent: %d\n", iResult);

			if ( iResult == SOCKET_ERROR){
				printf("send() failed  with error %d\n", WSAGetLastError());
			}
			first_client = false;
			iResult = 0;
		}
	}
	while ( second_client ){
		printf("Waiting for second client to connect...\n");

		sockaddr_in cliaddr2;
		int length2 = sizeof(cliaddr2);
		AcceptSocket2 = accept( s, (sockaddr*)&cliaddr2, &length2);
		
		if ( AcceptSocket2 == INVALID_SOCKET ){
			printf ( "accept faild:%d\n", WSAGetLastError() );
			closesocket(s);
			WSACleanup();
			return 5;
		}
		else 
		{
			PHOSTENT phost2;
			printf ( "Second client connect.\n" );
			phost2 = gethostbyaddr ( (char*) &cliaddr2.sin_addr, 4, AF_INET);
			printf ( "%s %s Port:%d\n", phost2->h_name, inet_ntoa ( cliaddr2.sin_addr ), cliaddr2.sin_port );

			char color[1];
			color[0] = 0;
			iResult = send ( AcceptSocket2, color, 1, 0 );
			printf("bytes sent: %d\n", iResult);

			if ( iResult == SOCKET_ERROR){
				printf("send() failed  with error %d\n", WSAGetLastError());
			}
			iResult = 0;
			second_client = false;
		}
	}
	const int buflen_recv = 64;
	char buf_recv[64];

	bool step_w = true; 	//ход белых
	while (true){
		printf("I'm in while \n");
		if ( step_w )
		{
			iResult = recv( AcceptSocket1, buf_recv, buflen_recv, 0);
			if ( iResult > 0){
				printf("Bytes received: %d \n", iResult);
				iResult = send ( AcceptSocket2, buf_recv, buflen_recv, 0 );
				if ( iResult == SOCKET_ERROR)
					printf("send() failed  with error %d\n", WSAGetLastError());
				else{
					step_w = false;
					printf ("Bytes Sent:%d\n", iResult);
				}
			}
		}
		else{
			iResult = recv ( AcceptSocket2, buf_recv, buflen_recv, 0);
			if (iResult > 0){
				iResult = send ( AcceptSocket1, buf_recv, buflen_recv, 0 );
				if ( iResult == SOCKET_ERROR)
					printf("send() failed  with error %d\n", WSAGetLastError());
				else{
					step_w = true;
					printf ("Bytes Sent:%d\n", iResult);
				}
			}
		}
	}
	iResult = shutdown ( AcceptSocket2, SD_SEND );
		
	if (iResult == SOCKET_ERROR){
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(AcceptSocket1);
		closesocket(AcceptSocket2);
		WSACleanup();
		return 6;	
	}
	closesocket(AcceptSocket1);
	closesocket(AcceptSocket2);
	
	closesocket(s);
	
	WSACleanup();

	system("pause");
	return 0;
}
