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

	




	// 소켓 반환
	closesocket(clientSocket);
	// 윈속 종료
	WSACleanup();
}