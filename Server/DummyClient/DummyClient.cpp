#include "pch.h"
#include <iostream>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* msg)
{
	int32 errCode = WSAGetLastError();
	cout << msg << " error: " << errCode << endl;
}

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		HandleError("socket");
		return 0;
	}

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // 구식 deprecated
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);      // 이걸 쓰자
	serverAddr.sin_port = htons(7777); // host to network short

	// Connected UDP
	connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	// send로 보냄

	// ----------------------------


	while (true)
	{
		/*for (int32 i = 0; i < 10; i++)
		{
			char sendBuffer[100] = "Hello World!";

			// 나의 IP주소 + 포트번호 설정
			int resultCode = sendto(clientSocket, sendBuffer, sizeof(sendBuffer), 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
			if (resultCode == SOCKET_ERROR)
			{
				int32 errCode = WSAGetLastError();
				cout << "send error: " << errCode << endl;
				return 0;
			}

			cout << "Send Data! Length: " << sizeof(sendBuffer) << endl;
		}*/
		
		char sendBuffer[100] = "Hello World!";

		// Unconnected UDP
		// int resultCode = sendto(clientSocket, sendBuffer, sizeof(sendBuffer), 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
		
		// Connected UDP
		int resultCode = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);

		SOCKADDR_IN recvAddr;
		memset(&recvAddr, 0, sizeof(recvAddr));
		int recvAddrSize = sizeof(recvAddr);


		char recvBuffer[1024] = {};
		//int32 recvLen = recvfrom(clientSocket, recvBuffer, sizeof(recvBuffer), 0, (SOCKADDR*)&recvAddr, &recvAddrSize);
		int32 recvLen = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (recvLen <= 0)
		{
			HandleError("RecvFrom");
			return 0;
		}

		cout << "Recv Data! Data: " << recvBuffer << endl;
		cout << "Recv Data! Length: " << recvLen << endl;

		this_thread::sleep_for(1s);
	}

	// 소켓 반환
	closesocket(clientSocket);
	// 윈속 종료
	WSACleanup();
}