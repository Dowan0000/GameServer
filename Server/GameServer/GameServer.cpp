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

	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		HandleError("socket");
		return 0;
	}

	// SO_Keeplive = 주기적으로 연결 상태 확인 (TCP Only)
	
	bool enable = true;
	setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&enable, sizeof(enable));
	
	// SO_Linger = 소켓 종료시 남아있는 데이터를 전송할 것인지
	// onoff = 0이면 바로 closesocket return, 아니면 linger초만큼 대기
	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 5;
	setsockopt(serverSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));

	// Half-close = shutdown()
	// SO_SEND = send만 막음
	// SO_RECV = recv만 막음
	// SO_BOTH = send, recv 모두 막음
	//::shutdown(serverSocket, SD_SEND);

	// SO_SNDBUF = 송신 버퍼 크기
	// SO_RCVBUF = 수신 버퍼 크기
	int32 sendBufSize;
	int32 optLen = sizeof(sendBufSize);
	getsockopt(serverSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufSize, &optLen);
	cout << "SO_SNDBUF: " << sendBufSize << endl;

	int32 recvBufSize;
	optLen = sizeof(recvBufSize);
	getsockopt(serverSocket, SOL_SOCKET, SO_RCVBUF, (char*)&recvBufSize, &optLen);
	cout << "SO_RCVBUF: " << recvBufSize << endl;

	// SO_REUSEADDR = IP주소, 포트 재사용
	{
		bool enable = true;
		setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(enable));
	}

	// IPPROTO_TCP = TCP
	// TCP_NODELAY = Nagle 알고리즘 작동 여부
	// 데이터가 충분히 크면 보내고, 그렇지않으면 쌓일때 까지 대기
	{
		bool enable = true;   // NO_DELAY = true : Nagle 알고리즘 끔
		setsockopt(serverSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&enable, sizeof(enable));
	}


	closesocket(serverSocket);
	WSACleanup();
}