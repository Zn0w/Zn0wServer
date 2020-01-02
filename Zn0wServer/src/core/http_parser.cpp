#include "http_parser.h"


void parse_http_request(HTTP_Request* http_request, char* message, unsigned int length)
{
	char word[256];
	memset(word, '\0', 256);
	int i = 0;
	int j;
	
	// get method
	for (j = 0; message[j] != ' ', j < length; j++)
	{
		word[i++] = message[j];
	}

	memcpy((void*)http_request->method, message, i + 1);
}