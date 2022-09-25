#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include "Render.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


int __cdecl main(int argc, char** argv) {
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	addrinfo* result = NULL, * ptr = NULL, hints;
	const char* sendbuf = "Client Connected";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	//Check CommandLine Args
	if (argc != 2) {
		printf("usage: %s server-name\n", argv[0]);
		return 1;
	}

	//Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSA Startup failed with error: %d\n", iResult);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//resiolve server address and port
	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	//attempt to connect
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %d\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		//connect to server
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	//send the initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("Send failed with error: %d\n", iResult);
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes sent: %ld\n", iResult);

	FILE* recvfile;
	if (fopen_s(&recvfile, "client.scene", "w")) {
		std::cout << "failed to open file";
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	//recieve until the peer closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes recieved: %d\n", iResult);
			fwrite(recvbuf, sizeof(char), iResult, recvfile);
			//copy data to file
			
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else printf("recv failed with error: %d\n", WSAGetLastError());
	} while (iResult > 0);

	//When done copying to file
	//interpret file as data
	//render
	//send back framebuffer

	//cleanup
	fclose(recvfile);
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}
