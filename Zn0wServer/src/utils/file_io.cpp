#include "file_io.h"


unsigned int read_file(const char* filepath, char** buffer)
{
	FILE* f = 0;
	if (filepath[0] == '/')
		f = fopen(filepath + 1, "r");
	else
		f = fopen(filepath, "r");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

		*buffer = (char*)malloc(fsize);
		if (*buffer)
			fread(*buffer, sizeof(char), fsize, f);
		else
			fsize = 0;

		fclose(f);

		return fsize;
	}
	else
		return 0;
}