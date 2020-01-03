#pragma once

#include <string.h>


struct HeaderItem
{
	const char* attribute;
	const char* value;
};

struct HTTP_Request
{
	char method[256] = "";
	char url[256] = "";
	char protocol[256] = "";

	HeaderItem* header_items;
};

struct HTTP_Response
{};


void parse_http_request(HTTP_Request* http_request, char* message, unsigned int length);
//void parse_http_response(HTTP_Response* http_response);