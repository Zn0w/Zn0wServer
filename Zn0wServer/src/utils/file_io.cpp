#include "file_io.h"


unsigned int read_file(const char* filepath, char** buffer)
{
	FILE* f = 0;
	if (filepath[0] == '/')
		f = fopen(filepath + 1, "rb");
	else
		f = fopen(filepath, "rb");

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

unsigned int read_image_file(const char* filepath, char** buffer)
{
	FILE* f = 0;
	if (filepath[0] == '/')
		f = fopen(filepath + 1, "rb");
	else
		f = fopen(filepath, "rb");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

		/* Get the image data offset */
		unsigned int offset;
		fseek(f, 10, SEEK_SET);
		fgets((char*)&offset, 4, f);

		/* Go the the position where the image data is stored */
		fseek(f, offset, SEEK_SET);

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