#include "http_parser.h"


void parse_http_request(HTTP_Request* http_request, char* message, unsigned int length)
{
	char word[256];
	memset(word, '\0', 256);
	int i = 0;
	int j;
	int start_cursor;

	// get method
	for (j = 0; message[j] != ' ' && j < length; j++)
	{
		word[i++] = message[j];
	}

	memcpy((void*)http_request->method, message, i);
	memset(word, '\0', 256);
	i = 0;
	start_cursor = j + 1;

	// get url
	for (j = start_cursor; message[j] != ' ' && j < length; j++)
	{
		word[i++] = message[j];
	}

	memcpy((void*)http_request->url, message + start_cursor, i);
	memset(word, '\0', 256);
	i = 0;
	start_cursor = j + 1;

	// get protocol version
	for (j = start_cursor; message[j] != '\r' && j < length; j++)
	{
		word[i++] = message[j];
	}

	memcpy((void*)http_request->protocol, message + start_cursor, i);
	memset(word, '\0', 256);
	i = 0;
	start_cursor = j + 2;	// skip both \r and \n (windows implementation)

	// get header
	j = start_cursor;
	while (message[j] != '\0' && message[j] != '\r' && j < length)
	{
		HeaderItem header_item;
		
		while (message[j] != ':')
			word[i++] = message[j++];

		memcpy((void*)header_item.attribute, message + start_cursor, i);
		memset(word, '\0', 256);
		i = 0;
		start_cursor = j + 2;	// skip both ':' and 'space'

		j = start_cursor;
		while (message[j] != '\r')
			word[i++] = message[j++];

		memcpy((void*)header_item.value, message + start_cursor, i);
		memset(word, '\0', 256);
		i = 0;
		start_cursor = j + 2;	// skip both \r and \n (windows implementation)

		http_request->header_items.push_back(header_item);
		
		j = start_cursor;
	}
}