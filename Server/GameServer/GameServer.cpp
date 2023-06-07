#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		int32 errCode = WSAGetLastError();
		cout << "socket error: " << errCode << endl;
		return 0;
	}

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // 니가 알아서 해줘
	serverAddr.sin_port = htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32 errCode = WSAGetLastError();
		cout << "bind error: " << errCode << endl;
		return 0;
	}

	if (listen(listenSocket, 10) == SOCKET_ERROR)
	{
		int32 errCode = WSAGetLastError();
		cout << "listen error: " << errCode << endl;
		return 0;
	}

	cout << "Waiting for client..." << endl;

	while (true)
	{
		SOCKADDR_IN clientAddr;
		memset(&clientAddr, 0, sizeof(clientAddr));
		int32 addrLen = sizeof(clientAddr);

		SOCKET clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			int32 errCode = WSAGetLastError();
			cout << "accept error: " << errCode << endl;
			return 0;
		}

		char ipAddress[16];
		inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
		cout << "Client Connected IP : " << ipAddress << endl;
		
		while (true)
		{
			char recvBuffer[100];

			this_thread::sleep_for(1s);
				
			int recvLen = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen <= 0)
			{
				int32 errCode = WSAGetLastError();
				cout << "recv error: " << errCode << endl;
				return 0;
			}

			cout << recvBuffer << endl;
			cout << "Recv Data! Length: " << recvLen << endl;
			
			/*int resultCode = send(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (resultCode == SOCKET_ERROR)
			{
				int32 errCode = WSAGetLastError();
				cout << "send error: " << errCode << endl;
				return 0;
			}*/
		}
	
	}


	closesocket(listenSocket);
	WSACleanup();
}