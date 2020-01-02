#pragma once

#include <winsock2.h>
#include <stdio.h>


static const char* IP_ADDRESS = "127.0.0.1";
static int PORT = 8080;


static void create_socket()
{

	printf("\nInitializing Winsock...");
	
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("  Fail, Error Code : %d", WSAGetLastError());
		return;
	}

	printf("  Success\n");

	// Create a socket
	SOCKET s;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");


	sockaddr_in server_socket;
	server_socket.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	server_socket.sin_family = AF_INET;
	server_socket.sin_port = htons(PORT);

	//Bind
	if (bind(s, (sockaddr*) &server_socket, sizeof(server_socket)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		return;
	}

	printf("  Bind done\n");

	//Listen to incoming connections
	listen(s, 3);

	//Accept an incoming connection
	printf("Waiting for incoming connections...\n");

	sockaddr_in client_socket;
	int c = sizeof(sockaddr_in);
	SOCKET new_socket = accept(s, (sockaddr*) &client_socket, &c);
	if (new_socket == INVALID_SOCKET)
	{
		printf("Accept failed with error code : %d", WSAGetLastError());
	}

	printf("Connection accepted\n");

	//Reply to client
	const char* message = "Hello from the server\n";
	send(new_socket, message, strlen(message), 0);

	closesocket(s);
	WSACleanup();
}