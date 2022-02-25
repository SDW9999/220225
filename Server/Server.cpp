#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{
	// 1. Winsock �ʱ�ȭ
	WSAData wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Winsock Error : " << GetLastError() << endl;
		exit(-1);
	}


	// 2. Socket ����
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


	// 3. IP�� PORT�� �Ҵ��Ͽ� ���ϰ� ����
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
	

	// 4. Ŭ���̾�Ʈ ���� ���
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

		// 5. ���� -> Ŭ���̾�Ʈ �ڷ� ����
		char Message[1024] = { 0, };
		recv(ClientSocket, Message, 1024 - 1, 0);

		cout << "client sended : " << Message << endl;
		send(ClientSocket, Message, strlen(Message) + 1, 0);

		// 6. Ŭ���̾�Ʈ ���� ����
		closesocket(ClientSocket);
	}

	// 7. ���� ���� ����
	closesocket(ServerSocket);

	// 8. �ʱ�ȭ�� Winsock ����
	WSACleanup();

	return 0;
}