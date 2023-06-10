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

	// select 모델
	// 소켓 함수 호출이 성공할 시점을 미리 알 수 있다

	// socket set 읽기, 쓰기, 예외 관찰
	// select(readSet, writeSet, exceptSet, timeout) - 관찰 시작
	// 적어도 하나의 소켓이 준비되면 리턴 - 낙오자는 알아서 제거됨

	vector<Session> sessions;
	sessions.reserve(100);

	fd_set reads;
	fd_set writes;

	while (true)
	{
		FD_ZERO(&reads);
		FD_ZERO(&writes);

		FD_SET(listenSocket, &reads);

		for (Session& s : sessions)
		{
			if(s.recvBytes <= s.sendBytes)
				FD_SET(s.socket, &reads);
			else
				FD_SET(s.socket, &writes);
		}

		int32 retVal = select(0, &reads, &writes, nullptr, nullptr);
		if (retVal == SOCKET_ERROR)
		{
			HandleError("select");
			break;
		}

		// listner 소켓 체크
		if (FD_ISSET(listenSocket, &reads))
		{
			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);
			SOCKET clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "Client Connected" << endl;
				sessions.push_back(Session{ clientSocket });
			}
		}

		// 나머지 소켓 체크
		for (Session& s : sessions)
		{
			// read 체크
			if (FD_ISSET(s.socket, &reads))
			{
				int32 recvLen = recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen <= 0)
				{
					// sessions 에서 제거
					continue;
				}

				s.recvBytes = recvLen;
			}

			// write 체크
			if (FD_ISSET(s.socket, &writes))
			{
				// 블로킹 모드 -> 모든 데이터 다 보내
				// 논블로킹 모드 -> 일부만 보낼 수 있음
				int32 sendLen = send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (sendLen <= 0)
				{
					// sessions 에서 제거
					continue;
				}

				s.sendBytes += sendLen;
				if (s.sendBytes == s.recvBytes)
					s.sendBytes = s.recvBytes = 0;
			}
		}
	}

	

	closesocket(listenSocket);
	WSACleanup();
}