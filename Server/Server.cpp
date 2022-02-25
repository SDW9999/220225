#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{
	// 1. Winsock 초기화
	WSAData wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Winsock Error : " << GetLastError() << endl;
		exit(-1);
	}


	// 2. Socket 생성
	SOCKET ServerSocket;

	ServerSocket = socket(
		AF_INET,
		SOCK_STREAM,
		0
	);

	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "socket Error : " << GetLastError() << endl;
		exit(-1);
	}


	// 3. IP와 PORT를 할당하여 소켓과 연결
	SOCKADDR_IN ServerAddr;

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(50000);

	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		cout << "bind Error : " << GetLastError() << endl;
		exit(-1);
	}
	

	// 4. 클라이언트 연결 대기
	if (listen(ServerSocket, 0) == SOCKET_ERROR)
	{
		cout << "listen Error : " << GetLastError() << endl;
		exit(-1);
	}

	while (1)
	{
		SOCKADDR_IN ClientAddr;
		int ClientAddrSize = sizeof(ClientAddr);
		SOCKET ClientSocket;
		ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClientAddrSize);

		if (ClientSocket == SOCKET_ERROR)
		{
			cout << "accept Error : " << GetLastError() << endl;
			exit(-1);
		}

		cout << "connect ip : " << inet_ntoa(ClientAddr.sin_addr) << endl;
		cout << "connect port : " << ntohs(ClientAddr.sin_port) << endl;

		// 5. 서버 -> 클라이언트 자료 전송
		char Message[1024] = { 0, };
		recv(ClientSocket, Message, 1024 - 1, 0);

		cout << "client sended : " << Message << endl;
		send(ClientSocket, Message, strlen(Message) + 1, 0);

		// 6. 클라이언트 연결 종료
		closesocket(ClientSocket);
	}

	// 7. 서버 소켓 종료
	closesocket(ServerSocket);

	// 8. 초기화한 Winsock 종료
	WSACleanup();

	return 0;
}