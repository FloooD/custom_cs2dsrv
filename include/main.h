/*
 * Published under GPLv3.
 * For more information take a look at the Readme
 * Copyright (c) by the authors of this file
 *
 * Author/s of this file: Jermuk
 */

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NS_PER_S 1000000000
#include <stdarg.h> //log.h
#include <math.h> //aim
#define PI 3.14159265

#ifdef _WIN32
#include <winsock.h>
#include <io.h>
#else
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <sys/select.h> //for select()
#endif

/* Self-made includes */
#include "log.h"
#include "settings.h"
#include "structs.h"
#include "weapons.h"

#include "cross_layer.h"
#include "functions.h"
#include "sendto_functions.h"
#include "respond_messages.h"
#include "packets.h"
#include "script-functions.h"
#include "map.h"
#include "lua_env.h"
#include "hashmap.h"
#include "parse.h"
#include "parse_cmds.h"

#include <signal.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#ifdef USE_ZLIB
#include <zlib.h>
#endif

int main();
void just(byte*, int);
extern int sock;

#endif // MAIN_H_INCLUDED
