#include "pch.h"
#include <iostream>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		int32 errCode = WSAGetLastError();
		cout << "socket error: " << errCode << endl;
		return 0;
	}

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // 구식 deprecated
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);      // 이걸 쓰자
	serverAddr.sin_port = htons(7777); // host to network short

	if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32 errCode = WSAGetLastError();
		cout << "connect error: " << errCode << endl;
		return 0;
	}

	cout << "Connected to server" << endl;

	while (true)
	{
		char sendBuffer[100] = "Hello World!";

		for (int32 i = 0; i < 10; i++)
		{
			int resultCode = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
			if (resultCode == SOCKET_ERROR)
			{
				int32 errCode = WSAGetLastError();
				cout << "send error: " << errCode << endl;
				return 0;
			}
		}
		cout << "Send Data! Length: " << sizeof(sendBuffer) << endl;
		
		/*char recvBuffer[1024] = {};
		int recvLen = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (recvLen <= 0)
		{
			int32 errCode = WSAGetLastError();
			cout << "recv error: " << errCode << endl;
			return 0;
		}

		cout << recvBuffer << endl;
		cout << "Recv Data! Length: " << recvLen << endl;*/

		this_thread::sleep_for(1s);
	}

	// 소켓 반환
	closesocket(clientSocket);
	// 윈속 종료
	WSACleanup();
}