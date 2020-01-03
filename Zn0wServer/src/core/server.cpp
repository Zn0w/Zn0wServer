#include "server.h"


void start_server()
{

	printf("\nInitializing Winsock...");

	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("  Fail, Error Code : %d\n", WSAGetLastError());
		return;
	}

	printf("  Success\n");

	// Create a socket
	SOCKET s;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d\n", WSAGetLastError());
		return;
	}

	printf("Socket created\n");


	sockaddr_in server_socket;
	server_socket.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	server_socket.sin_family = AF_INET;
	server_socket.sin_port = htons(PORT);

	//Bind
	if (bind(s, (sockaddr*)&server_socket, sizeof(server_socket)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d\n", WSAGetLastError());
		return;
	}

	printf("Bind done\n");

	//Listen to incoming connections
	listen(s, MAX_CONN);

	printf("Waiting for incoming connections...\n");
	
	//Accept an incoming connection

	sockaddr_in client_socket;
	SOCKET new_socket;
	int c = sizeof(sockaddr_in);
	new_socket = accept(s, (sockaddr*)&client_socket, &c);
	if (new_socket == INVALID_SOCKET)
	{
		printf("Accept failed with error code : %d", WSAGetLastError());
		return;
	}

	printf("Connection accepted\n");

	// get client http request
	char client_message[2048];
	memset(client_message, '\0', 2048);
	recv(new_socket, client_message, 2048, 0);
	printf("Message from client:\n%s", client_message);

	HTTP_Request http_request;
	parse_http_request(&http_request, client_message, 2048);

	//send server http response
	char http_response_string[2048];
	memset(http_response_string, '\0', 2048);

	char* resource = 0;
	unsigned int file_size;
	if ((file_size = read_file(http_request.url, &resource)))
	{
		HeaderItem content_length;
		memcpy((void*)content_length.attribute, "Content-Length", 15);
		sprintf(content_length.value, "%d", file_size);

		HeaderItem header_items[] = { { "Content-Type", "text/html;charset=UTF-8" },content_length };

		parse_http_response(http_response_string, "HTTP/1.1", "200", "OK", header_items, 2, resource);
		send(new_socket, http_response_string, strlen(http_response_string), 0);
	}
	else
	{
		file_size = read_file("test/error_404.html", &resource);
		HeaderItem content_length;
		memcpy((void*)content_length.attribute, "Content-Length", 15);
		sprintf(content_length.value, "%d", file_size);

		HeaderItem header_items[] = { { "Content-Type", "text/html;charset=UTF-8" }, content_length };

		parse_http_response(http_response_string, "HTTP/1.1", "404", "Not Found", header_items, 2, resource);
		send(new_socket, http_response_string, strlen(http_response_string), 0);
	}

	printf("Message sent to the client:\n%s", http_response_string);

	closesocket(s);
	WSACleanup();
}