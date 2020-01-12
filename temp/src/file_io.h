#pragma once

#include <stdio.h>
#include <stdlib.h>


unsigned int read_file(const char* filepath, char** buffer);

unsigned int read_image_file(const char* filepath, char** buffer);