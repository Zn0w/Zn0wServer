#include "server.h"


#define HTTP_REQUEST_MAX_LENGTH 4096
#define HTTP_RESPONSE_MAX_LENGTH 16384


static void handle_client(SOCKET client_socket)
{
	while (true)
	{
		// get client http request

		char client_message[HTTP_REQUEST_MAX_LENGTH];
		memset(client_message, '\0', HTTP_REQUEST_MAX_LENGTH);

		int recv_result = recv(client_socket, client_message, HTTP_REQUEST_MAX_LENGTH, 0);
		if (recv_result == 0)
		{
			printf("Connection has been closed\n");
			return;
		}
		else if (recv_result == SOCKET_ERROR)
		{
			printf("Socket error: %d\n", WSAGetLastError());
			return;
		}

		printf("Message from client:\n%s", client_message);

		HTTP_Request http_request;
		parse_http_request(&http_request, client_message, HTTP_REQUEST_MAX_LENGTH);


		//send server http response

		char http_response_string[HTTP_RESPONSE_MAX_LENGTH];
		memset(http_response_string, '\0', HTTP_RESPONSE_MAX_LENGTH);

		if (strcmp(http_request.url, "/") == 0)
		{
			memcpy((void*)http_request.url, "index.html", 11);
		}

		char* resource = 0;
		unsigned int file_size;
		if (strcmp(http_request.url, "/favicon.ico") == 0)
		{
			// handle the icon loading
			//file_size = read_file(http_request.url, &resource);
			file_size = read_image_file(http_request.url, &resource);

			HeaderItem content_length;
			memcpy((void*)content_length.attribute, "Content-Length", 15);
			sprintf(content_length.value, "%d", file_size);

			HeaderItem header_items[] = { { "Content-Type", "image/x-icon" },content_length };

			parse_http_response(http_response_string, "HTTP/1.1", "200", "OK", header_items, 2, resource);
			send(client_socket, http_response_string, strlen(http_response_string), 0);
		}
		else if ((file_size = read_file(http_request.url, &resource)))
		{
			HeaderItem content_length;
			memcpy((void*)content_length.attribute, "Content-Length", 15);
			sprintf(content_length.value, "%d", file_size);

			HeaderItem header_items[] = { { "Content-Type", "text/html;charset=UTF-8" },content_length };

			parse_http_response(http_response_string, "HTTP/1.1", "200", "OK", header_items, 2, resource);
			send(client_socket, http_response_string, strlen(http_response_string), 0);
		}
		else
		{
			file_size = read_file("errors/error_404.html", &resource);
			if (file_size == 0)
			{
				// If site-defined error pages don't exist

				HeaderItem header_items[] = { { "Content-Type", "text/plaint;charset=UTF-8" }, { "Content-Length", "25" } };

				parse_http_response(http_response_string, "HTTP/1.1", "404", "Not Found", header_items, 2, "This page does not exist");
				send(client_socket, http_response_string, strlen(http_response_string), 0);
			}
			else
			{
				HeaderItem content_length;
				memcpy((void*)content_length.attribute, "Content-Length", 15);
				sprintf(content_length.value, "%d", file_size);

				HeaderItem header_items[] = { { "Content-Type", "text/html;charset=UTF-8" }, content_length };

				parse_http_response(http_response_string, "HTTP/1.1", "404", "Not Found", header_items, 2, resource);
				send(client_socket, http_response_string, strlen(http_response_string), 0);
			}
		}

		printf("Message sent to the client:\n%s", http_response_string);
	}

	closesocket(client_socket);
}

void start_server()
{
	// Init Winsock2 library

	printf("\nInitializing Winsock...");
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("  Fail, Error Code : %d\n", WSAGetLastError());
		return;
	}
	printf("  Success\n");

	// Set up a tcp server

	// Create a socket that is bound to a specific transport service provider
	SOCKET s;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d\n", WSAGetLastError());
		return;
	}
	printf("Socket created\n");


	//Bind (associate a local address with a socket)
	sockaddr_in server_socket;
	server_socket.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	server_socket.sin_family = AF_INET;
	server_socket.sin_port = htons(PORT);

	if (bind(s, (sockaddr*)&server_socket, sizeof(server_socket)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d\n", WSAGetLastError());
		return;
	}
	printf("Bind done\n");

	// Start listening to incoming connections
	
	listen(s, MAX_CONN);
	running = true;

	printf("Waiting for incoming connections...\n");
	
	// Accept incoming connections

	while (running)
	{
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

		std::thread new_client_thread(handle_client, new_socket);
		new_client_thread.join();	// to be removed
	}

	// TODO : make sure all the clients are done

	closesocket(s);
	WSACleanup();
}