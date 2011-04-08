/*
 * Published under GPLv3.
 * For more information take a look at the Readme
 * Copyright (c) by the authors of this file
 *
 * Author/s of this file: Jermuk, GuccaGucca, FloooD
 */

#include "settings.h"

#include <stdarg.h>
#include <string.h>

char* cfg_file = NULL;
char* lua_file = NULL;
int no_usgn = 0;
int lua_strict = 0;
int lua_debug = 0;
int debug = 0;

unsigned char pre_authcode[] = "roflmfao?";
char startweapons[] = { 50 };
unsigned short lcbuffer[LC_BUFFER_SIZE][MAX_CLIENTS][2];
int fpsnow = 0;
int dropped_weapons_count = 0;
int uptime = 0;

unsigned char *sv_name = NULL;
unsigned char *sv_map = NULL;
unsigned char *sv_rcon = NULL;
unsigned int sv_hostport = 0;
unsigned short sv_maxplayers = 0;
unsigned short sv_fps = 0;
unsigned short sv_lcbuffer = 0;
unsigned short sv_fow = 0;
unsigned short sv_gamemode = 0;
unsigned short sv_friendlyfire = 0;
unsigned short sv_usgnonly = 0;
unsigned char *sv_password = 0;
unsigned short bot_count = 0;
unsigned short sv_lan = 0;

unsigned short mp_roundtime = 0;
unsigned short mp_freezetime = 0;
unsigned short mp_c4timer = 0;
unsigned short mp_infammo = 0;
unsigned short mp_respawndelay = 0;
unsigned short mp_specmode = 0;
unsigned short onlineplayer = 0;
unsigned short mp_startmoney = 0;
unsigned short mp_dmspawnmoney = 0;

struct setting{
	char *name;
	char *value;
};

// Internal structure to represent read configurations
typedef struct setting_closure{
	int entries;
	struct setting** settings;
} setting_closure;

int tointeger(char * c);
double todouble(char *c);
struct setting * settings[256];

int buf_is_in(char c, int n, ...){
	va_list ap;
	va_start(ap, n);
	int j;
	for(j=0; j<n; j++){
		char* i = (char*)(va_arg(ap, int)); // minimum is 4 bytes, so can't pop off chars
		if (c == *i) return 1;} //Requires the type to cast to. Increments ap to the next argument.
	va_end(ap);
	return 0;
}

//int is_in(char c, char* args){
//	int i, n = strlen(args);
//	for (i=0; i<n; i++)
//		if (args[i]==c) return 1;
//	return 0;
//}

setting_closure read_config(const char* cfg_file, struct setting** settings){ // rewritten, also renamed for C naming standard
	FILE * f;
	char *buf = (char*)malloc(1024);
	char *clone = buf;
	int setting_num = 0;
	f = fopen(cfg_file, "r");
	setting_closure c = {0, 0};
	if (!f) {perror("Error opening CFG");return c;}
	while (fgets(buf, 256, f)){
		int ptr = 0;
		while (buf_is_in (*buf++, 5, "\0", " ", "\t", "\n", "\r"));buf--; // trim
		// Get until one of space or \t or \n or \r if malformatted
		while (!buf_is_in(buf[ptr], 5, "\0", " ", "\t", "\n", "\r")) ptr++; // get header
		struct setting *v = (struct setting*) malloc(sizeof(struct setting));
		v->name = (char*)malloc(ptr+1);
		memcpy(v->name, buf, ptr);v->name[ptr] = '\0';
		buf+=ptr;ptr=0;
		while (buf_is_in (buf[ptr], 5, "\0", " ", "\t", "\n", "\r")){
			if (buf_is_in(buf[ptr], 3, "\0", "\n", "\r")) goto FAIL; ptr++; // OH NOEZ, GOTO, THE WORLD SHALL END
		} // trim, and make sure not malformed
		buf += ptr; ptr = 0;
		while (!buf_is_in(buf[ptr], 5, "\0", "\n", "\r")) ptr++; // get value
		v->value = (char*)malloc(ptr+1);
		memcpy(v->value, buf, ptr); v->value[ptr] = '\0';
		buf = clone;
		settings[setting_num++] = v;
		FAIL:
		(void) 0; // nop
	}
	c.entries = setting_num;
	c.settings = settings;
	return c;
}

char *GetValue(setting_closure clsr, char *sname, char *alternate){
	int i;
	for (i = 0; i < clsr.entries; i++){
		if (clsr.settings[i] != NULL){
			struct setting *var = clsr.settings[i];
			if (strcmp(var->name, sname) == 0){
				char *string = malloc(strlen(var->value) + 1);
				if (!string)
					error_exit("Memory error in GetValue()\n");
				strncpy(string, var->value, strlen(var->value));
				string[strlen(var->value)] = '\0';

				free(var->name);
				free(var->value);
				free(var); //also frees settings[i]
				clsr.settings[i] = NULL;
				return string;
			}
		}
	}
	//If not found
	char *string = malloc(strlen(alternate) + 1);
	if (string == NULL)
		error_exit("Memory error in GetValue()\n");
	strncpy(string, alternate, strlen(alternate));
	string[strlen(alternate)] = '\0';

	return string; // doesn't exist
}

void ReadServerCfg(const char* cfg)
{
	setting_closure clsr = read_config(cfg, settings);

	sv_name = sv_name ? sv_name : (unsigned char *) GetValue(clsr, "sv_name", "Alpha Custom CS2D Server");
	sv_map = (unsigned char *) GetValue(clsr, "sv_map", "de_cs2d");
	sv_hostport = tointeger(GetValue(clsr, "sv_hostport", "36963"));
	sv_maxplayers = tointeger(GetValue(clsr, "sv_maxplayers", "32"));
	sv_fps = tointeger(GetValue(clsr, "sv_fps", "500"));
	sv_lcbuffer = tointeger(GetValue(clsr, "sv_lcbuffer", "100"));
	sv_fow = tointeger(GetValue(clsr, "sv_fow", "0"));
	sv_gamemode = tointeger(GetValue(clsr, "sv_gamemode", "2"));
	sv_friendlyfire = tointeger(GetValue(clsr, "sv_friendlyfire", "0"));
	sv_usgnonly = tointeger(GetValue(clsr, "sv_usgnonly", "0"));
	sv_password = (unsigned char *) GetValue(clsr, "sv_password", "");
	sv_rcon = (unsigned char *) GetValue(clsr, "sv_rcon", "testing");
	sv_lan = tointeger(GetValue(clsr, "sv_lan", "0"));
	bot_count = tointeger(GetValue(clsr, "bot_count", "0"));

	mp_roundtime = tointeger(GetValue(clsr, "mp_roundtime", "5"));
	mp_freezetime = tointeger(GetValue(clsr, "mp_freezetime", "0"));
	mp_c4timer = tointeger(GetValue(clsr, "mp_c4timer", "35"));
	mp_infammo = tointeger(GetValue(clsr, "mp_infammo", "0"));
	mp_respawndelay = tointeger(GetValue(clsr, "mp_respawndelay", "0"));
	mp_specmode = tointeger(GetValue(clsr, "mp_specmode", "1"));
	mp_startmoney = tointeger(GetValue(clsr, "mp_startmoney", "1000"));
	mp_dmspawnmoney = tointeger(GetValue(clsr, "mp_dmspawnmoney", "32000"));
}

int tointeger(char *c){
	int buff = atoi(c);
	free(c);
	return buff;
}

double todouble(char *c){
	double buff = atof(c);
	free(c);
	return buff;
}
