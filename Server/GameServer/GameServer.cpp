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

const int32 BUFSIZE = 1024;

struct Session
{
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = { };
	int32 recvBytes = 0;
	int32 sendBytes = 0;
};

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	// Blocking 소켓
	// accept -> 접속한 클라가 있을 때
	// connect -> 서버 접속 성공했을 때
	// send, sendto -> 요청한 데이터를 송신 버퍼에 복사했을 때
	// recv, recvfrom -> 수신 버퍼에 데이터가 도착했을 때


	// Non-Blocking 소켓

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		HandleError("socket");
		return 0;
	}

	u_long on = 1;
	if (ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;
	
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(7777);
	
	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		HandleError("bind");
		return 0;
	}

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		HandleError("listen");
		return 0;
	}

	// WSAEventSelect
	// 소켓과 관련된 네트워크 이벤트를 이벤트객체를 통해 감지
	// 생성 : WSACreateEvent
	// 삭제 : WSACloseEvent
	// 신호 상태 감지 : WSAWaitForMultipleEvents
	// 구체적인 이벤트 감지 : WSAEnumNetworkEvents

	// 소켓 <-> 이벤트 객체 연결 : WSAEventSelect(socket, event, networkEvents)
	// networkEvents : FD_READ, FD_WRITE, FD_ACCEPT, FD_CONNECT, FD_CLOSE
	// WSAEventSelect 함수를 호출하면 해당 소켓은 자동으로 비동기 소켓(논블로킹 소켓)이 된다.

	// WSAEnumNetworkEvents 함수를 호출하면 이벤트 객체에 발생한 네트워크 이벤트를 확인할 수 있다.

	vector<WSAEVENT> wsaEvents;
	vector<Session> sessions;
	sessions.reserve(100);

	WSAEVENT listenEvent = WSACreateEvent();
	wsaEvents.push_back(listenEvent);
	sessions.push_back(Session{ listenSocket });
	if (WSAEventSelect(listenSocket, listenEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
	{
		HandleError("WSAEventSelect");
		return 0;
	}

	while (true)
	{
		int32 Idx = WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, WSA_INFINITE, FALSE);
		
		Idx -= WSA_WAIT_EVENT_0;

		WSANETWORKEVENTS networkEvents;
		WSAEnumNetworkEvents(sessions[Idx].socket, wsaEvents[Idx], &networkEvents);
	
		// listenSocket check
		if (networkEvents.lNetworkEvents & FD_ACCEPT)
		{
			if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				continue;

			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);

			SOCKET clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "client connect" << endl;

				WSAEVENT clientEvent = WSACreateEvent();
				wsaEvents.push_back(clientEvent);
				sessions.push_back(Session{ clientSocket });
				if (WSAEventSelect(clientSocket, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
					return 0;
			}
		}
		
		// client session socket check
		if (networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_WRITE)
		{
			if ((networkEvents.lNetworkEvents & FD_READ) && (networkEvents.iErrorCode[FD_READ_BIT] != 0))
				continue;
			if ((networkEvents.lNetworkEvents & FD_WRITE) && (networkEvents.iErrorCode[FD_WRITE_BIT] != 0))
				continue;

			Session& s = sessions[Idx];

			// Read
			if (s.recvBytes == 0)
			{
				int32 recvLen = recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
				{
					// remove session
					continue;
				}

				s.recvBytes = recvLen;

			}

			if (s.recvBytes > s.sendBytes)
			{
				int32 senLen = send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (senLen == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
				{
					// remove session
					continue;
				}

				s.sendBytes += senLen;
				if(s.recvBytes == s.sendBytes)
					s.recvBytes = s.sendBytes = 0;

			}
		}

		if (networkEvents.lNetworkEvents & FD_CLOSE)
		{
			// remove session
		}
	}

	

	closesocket(listenSocket);
	WSACleanup();
}