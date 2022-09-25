#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

#define DATASIZE 22
#define ELEMENTCOUNT 2
#define ELEMENTSIZE (DATASIZE/ELEMENTCOUNT)
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#include <string>
#include <iostream>
#include "Window.h"


int __cdecl main(int argc, char** argv){
    if (argc < 2) {
        std::cout << "enter an 'out' fileName" << std::endl;
        return -1;
    }

    auto aspect_ratio = 16.0 / 9.0;
    unsigned int image_width = 2560;
    unsigned int image_height = static_cast<int>(image_width / aspect_ratio);
    unsigned int samples = 10;
    unsigned int max_depth = 50;
   
    
    hittable_list w = one_sphere();

    // Camera
	glm::vec3 lookfrom(13,2,3);
	glm::vec3 lookat(0,0,0);
	glm::vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0, 1.0);
   

    //framebuffer
    FrameBuffer* frame = new FrameBuffer(image_width, image_height);

    RenderSettings settings(image_width, image_height, samples, max_depth, cam, &w, frame);
	
	settings.toFile("server.scene");

	//setup server
	WSADATA wsadata;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	addrinfo* result = NULL, hints;

	int iSendResult;
	char sendbuf[DEFAULT_BUFLEN];

	char recvbuf[DEFAULT_BUFLEN];
	int buflen = DEFAULT_BUFLEN;

	//intialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (iResult != 0) {
		printf("WSA startup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d", iResult);
		WSACleanup();
		return 1;
	}

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %d", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", iResult);
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("socket failed with error: %d\n", iResult);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed wtih error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	//no longer need server socket
	closesocket(ListenSocket);


	FILE* sceneFile;
	if (fopen_s(&sceneFile, "server.scene", "r")) {
		std::cout << "Failed to open file" << std::endl;
		return 1;
	}

	//receive until peer shits down connection
	do {
		iResult = recv(ClientSocket, recvbuf, buflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			int readSize;
			while ((readSize = fread_s(sendbuf, buflen * sizeof(char), sizeof(char), buflen, sceneFile)) > 0) {
				printf("read: %d bytes ", (int)readSize);
				iSendResult = send(ClientSocket, sendbuf, readSize, 0);
				if (iSendResult == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(ClientSocket);
					WSACleanup();
					return 1;
				}
				printf("Bytes sent: %d\n", iSendResult);
			}
		}
		else if (iResult == 0) {
			printf("Connection closing...\n");
		}
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	} while (iResult > 0);

	//shutdown since connection closed 

	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
	}

	closesocket(ClientSocket);
	WSACleanup();

	fclose(sceneFile);

    Window window(settings, argv[1]);
    if (window.createWindow()) {
        std::cout << "Failed To Initialize Window" << std::endl;
        return -1;
    }
    window.begin();


    return 0;
}

//Client connects to server
//Server sends RenderSettingsPacket to Client
//Client does work
//Client sends Framebuffer data back to the server
//Server pieces data into its framebuffer and updates display
