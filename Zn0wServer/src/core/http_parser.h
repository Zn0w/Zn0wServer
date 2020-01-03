#pragma once

#include <string.h>
#include <vector>


struct HeaderItem
{
	char attribute[256] = "";
	char value[256] = "";
};

struct HTTP_Request
{
	char method[256] = "";
	char url[256] = "";
	char protocol[256] = "";

	//HeaderItem* header_items;
	std::vector<HeaderItem> header_items;
};

/*struct HTTP_Response
{
	char protocol[256] = "";
	char status_code[256] = "";
	char status_message[256] = "";

	std::vector<HeaderItem> header_items;

	char body[1024] = "";
};*/


void parse_http_request(HTTP_Request* http_request, char* message, unsigned int length);
void parse_http_response(
	char* http_response_string, const char* protocol,
	const char* status_code, const char* status_message,
	HeaderItem* header_items, unsigned int header_length,
	const char* body
);