#pragma once

#include <winsock2.h>
#include <stdio.h>
#include <thread>

#include "http_parser.h"
#include "file_io.h"


static const char* IP_ADDRESS = "127.0.0.1";
static int PORT = 8080;
#define MAX_CONN 3 // maximum number of pending connections that can be queued up before connections are refused

static bool running = false;


void start_server();