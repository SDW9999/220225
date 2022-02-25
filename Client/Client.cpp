#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{
	// 1. Winsock 초기화
	WSAData WsaData;

	if (WSAStartup(MAKEWORD(2, 2), &WsaData))
	{
		cout << "Winsock Error : " << GetLastError() << endl;
	}


	// 2. 소켓 생성
	SOCKET ServerSocket;

	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "Socket Error : " << GetLastError() << endl;
		exit(-1);
	}


	// 3. 주소 세팅
	SOCKADDR_IN ServerAddr;

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_port = htons(50000);
	ServerAddr.sin_family = PF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


	// 4. 서버와 연결
	if (connect(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		cout << "connect Error : " << GetLastError() << endl;
		exit(-1);
	}


	// 5. 서버에서 받기
	char Buffer[1024] = { 0, };
	cin.getline(Buffer, 1023);
	send(ServerSocket, Buffer, strlen(Buffer) + 1, 0);

	int RecvLength = recv(ServerSocket, Buffer, 1024, 0);

	if (RecvLength == 0)
	{
		cout << "close : " << GetLastError() << endl;
		exit(-1);
	}

	else if (RecvLength < 0)
	{
		cout << "Error : " << GetLastError() << endl;
		exit(-1);
	}

	cout << "Server Sended : " << Buffer << endl;


	// 6. 서버 종료
	closesocket(ServerSocket);

	// 7. 초기화한 Winsock 종료
	WSACleanup();

	return 0;
}