#pragma once

#include <string.h>


struct HeaderItem
{
	const char* attribute;
	const char* value;
};

struct HTTP_Request
{
	const char* method;
	const char* url;
	const char* protocol;

	HeaderItem* header_items;
};

struct HTTP_Response
{};


void parse_http_request(HTTP_Request* http_request, char* message, unsigned int length);
//void parse_http_response(HTTP_Response* http_response);