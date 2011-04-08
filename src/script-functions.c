/*
 * Published under GPLv3.
 * For more information take a look at the Readme
 * Copyright (c) by the authors of this file
 *
 * Author/s of this file: Jermuk, FloooD
 */

#include "script-functions.h"

void init_hooks(){
	hook_map = hashmap_new();
	int i = 0;
#define add(type) /*struct ll *type##_value = malloc(2*sizeof(int));*/ \
	struct ll_c *type##_c = malloc(3*sizeof(int));\
	type##_c->typ = i++;\
	type##_c->name = #type;\
	type##_c->root = NULL;\
	hashmap_put(hook_map, #type, type##_c); \

	add(ms100);
	add(second);
	add(minute);
	add(always); // delegate to ms100
	add(startround);
	add(endround);
	add(mapchange);
	add(parse);
	add(trigger);
	add(triggerentity);
	add(break);
	add(projectile);
	add(log);
	add(rcon);
	add(clientdata);
	add(join);
	add(leave);
	add(team);
	add(spawn);
	add(name);
	add(serveraction);
	add(buy);
	add(walkover);
	add(collect);
	add(drop);
	add(select);
	add(reload);
	add(attack);
	add(attack2);
	add(move);
	add(movetile);
	add(hit);
	add(kill);
	add(die);
	add(use);
	add(usebutton);
	add(say);
	add(sayteam);
	add(radio);
	add(spray);
	add(vote);
	add(buildattempt);
	add(build);
	add(flagtake);
	add(flagcapture);
	add(dominate);
	add(bombplant);
	add(bombdefuse);
	add(bombexplode);
	add(hostagerescue);
	add(vipescape);
	add(menu);
	add(objectdamage);
	add(objectkill);
	add(objectupgrade);

	if (!lua_strict){ // nonstandard hooks
		add(start);
		add(exit);
		add(respawn);
	}



#undef add

	lua_pushcfunction(_G, addhook);
	lua_setfield(_G, LUA_GLOBALSINDEX, "addhook");
}

int addhook(lua_State* l){
	int n = lua_gettop(l); // we need at least 2
	if (n<2){
		lua_pushstring(l, "addhook() requires two strings.");
		lua_error(l);
	}
	const char* type = lua_tostring(l, 1);
	const char* data = lua_tostring(l, 2);

	struct ll_c* fn;
	int err = hashmap_get(hook_map, (char*)type, (void**)(&fn));
	if (err != MAP_OK){
		lua_pushfstring(l, "Invalid hook %s.", type);
		lua_error(l);
	}

	struct ll* root = fn->root;

	if(!root) root = (fn->root = (struct ll*)malloc(sizeof(struct ll)));
	else{
		while (root->next)
			root = root->next;
		root = (root->next = (struct ll*)malloc(sizeof(struct ll)));
	}

	root->data = (char*)data;
	root->next = NULL;

	return 0;
}

// Fmt - s i f b
int invoke_traverse(struct ll* list, char* fmt, ...){
	if (!list) return 0;

	va_list args;
	va_start(args, fmt);
	void* buffer[10];
	int i,ret = 0,nargs = strlen(fmt);
	for (i=0;i<nargs;i++)
		buffer[i] = va_arg(args, void*);
	va_end(args);

	while (list && list->data){
		char* data = malloc(0xff);
		sprintf(data, "return %s", list->data);

		int err = luaL_dostring(_G, data); // load the function into _G
		if (err) {printf("[Lua] Error while calling hook: %s\n",list->data);continue;}
		for(i=0;i<nargs;i++){
			switch(fmt[i]){
			case 's':
			case 'S':
				// String
				lua_pushstring(_G, buffer[i]);
				break;
			case 'i':
			case 'I':
				// Integer
				lua_pushinteger(_G, (int)buffer[i]);
				break;
			case 'f':
			case 'F':
			case 'n':
			case 'N':
				// Number
				lua_pushnumber(_G, *(double*)(buffer[i]));
				break;
			case 'b':
			case 'B':
				lua_pushboolean(_G, (int)buffer[i]);
				break;
			default:
				lua_pushnil(_G);
				printf("[Lua] Cannot parse the parameter type.");
			}
		}
		err = docall2(_G, nargs, 1);
		ret = lua_tonumber(_G, 1);
		lua_pop(_G, 1);
		list = list->next;
	}
	return ret;
}

struct ll* get_fn(char* type){
	struct ll_c* fn;
	int err = hashmap_get(hook_map, type, (void**)(&fn));
	if (err != MAP_OK){
		if (lua_debug)
			printf("[Lua] DEBUG: Cannot invoke hook %s\n", type);
		return NULL;
	}
	return fn->root;
}

#define INVOKE(type, args...) struct ll* root = get_fn(type); \
	if(!root) return 0; \
	return invoke_traverse(root, args); \

/*
 int OnJoin(int id)
 id - Who joins
 Return Values:
 0 - OK
 */
int OnJoin(int id){
	SendJoinMessage(id);
	printf("%s (#%d) has joined the game using ip %s:%d and usgn-id #%d!\n", player[id].name, id, inet_ntoa(player[id].ip), player[id].port, player[id].usgn);

	INVOKE("join", "i", id);
}

/*
 int OnLeave(int id)
 id - Who leaves
 Return Values:
 0 - OK
 */
int OnLeave(int id, int reason){
	SendLeaveMessage(id);
	printf("%s has left the game.\n", player[id].name);
	INVOKE("leave", "ii", id, reason);
}

/*
 int OnSpecmove(int id, int newx, int newy)
 Return Values:
 0 - OK
 1 - Don't save it
 */

int OnSpecmove(int id, int newx, int newy){
	INVOKE("specmove", "iii", id, newx, newy);
}

/*
 int OnServerstart()
 Return Values:
 0 - OK
 */
int OnServerStart(){
	time_t rawtime;
	time(&rawtime);
	printf("********** Server started **********\n");
	printf("Listening at port %d and using name '%s'\n", sv_hostport, sv_name);
	INVOKE("start", "");
}

/*
 int OnExit()
 Return Values:
 0 - OK
 */
int OnExit(){
	printf("********** Server Shutdown! **********\n");
	INVOKE("exit", "");
}

/*
 int OnRespawnRequest(int id)
 Return Values:
 0 - Respawn
 1 - Don't Respawn
 */
int OnRespawnRequest(int id){
	if (player[id].dead == 1){
		if (player[id].money + mp_dmspawnmoney > 65000)
			player[id].money = 65000;
		else
			player[id].money += mp_dmspawnmoney;

		INVOKE("respawn","i",id);
	}
	else
		return 1;
}

/*
 int OnRespawn(int id)
 Return Values:
 0 - OK
 */
int OnRespawn(int id){
	return 0;
}

/*
 int OnRespawn(int id)
 Return Values:
 0 - OK
 */
int OnWeaponChangeAttempt(int id, int wpnid)
{
	if (player[id].wpntable[wpnid].status <= 0)
		return 1;
	player[id].actualweapon = wpnid;
	if (weapons[wpnid].special == 4 || weapons[wpnid].special == 5)
		player[id].wpntable[wpnid].status = 1;
	player[id].reloading = 0;

	//printf("%s switched to %s\n", player[id].name, weapons[wpnid].name);
	INVOKE("select", "ii", id, wpnid);
}

/*
 int OnAdvancedFire(int id, int status)
 Return Values:
 0 - OK
 1 - Not OK
 */
int OnAdvancedFire(int id, int status)
{
	int wpnid = player[id].actualweapon;

	if (mtime() < player[id].zoomtimer)
	{
		printf("Zoomtimer error!\n");
		return 1;
	}
	player[id].zoomtimer = mtime() + 500;

	switch (weapons[wpnid].special)
	{
	case 0:
		printf("%s tried to hack!\n", player[id].name);
		return 1;
	case 1:
	case 2:
	{
		if (status <= 1)
		{
			player[id].wpntable[wpnid].status = status + 1;
			break;
		}
		return 1;
	}
	case 3:
	{
		player[id].wpntable[wpnid].status = 2;
		OnFire(id);
		player[id].wpntable[wpnid].status = 1;
		break;
	}
	case 4:
	{
		if (status <= 1)
		{
			player[id].wpntable[wpnid].status = status + 1;
			break;
		}
		return 1;
	}
	case 5:
	{
		if (status <= 2 && player[id].wpntable[wpnid].status == status)
		{
			player[id].wpntable[wpnid].status = status + 1;
			break;
		}
		else if (player[id].wpntable[wpnid].status == 3)
		{
			player[id].wpntable[wpnid].status = 1;
			break;
		}
		return 1;
	}
	default:
	{
		SendMessageToPlayer(id, "Not implemented yet!", 1);
		return 1;
	}

	}
	INVOKE("attack2", "ii", id, status)
	return 0;
}
/*
 int OnFire(int id)
 Return Values:
 0 - OK
 1 - Not OK
 */
int OnFire(int id)
{
	unsigned char wpn = player[id].actualweapon;

	if (weapons[wpn].slot == 0) return 0;

	short *ammo1 = &player[id].wpntable[wpn].ammo1;
	if (*ammo1 != -1) {
		if (*ammo1 <= 0) {
			printf("Not enough ammo!\n");
			return 1;
		} else
			--(*ammo1);
	}

	/*if (mtime() < player[id].firetimer) {
		printf("Firetimer error!%u %u\n", mtime(), player[id].firetimer);
		return 1;
	} else
		player[id].firetimer = mtime() + weapons[wpn].freq;*/
	
	short dmg;

	switch (weapons[wpn].special) { //lmao 76543210
	case 7: //TODO: grenades
		return 0;
	case 6: //shotgun
		dmg = weapons[wpn].weapondamage;
		simulate_bullet(id, wpn, dmg, player[id].rotation + (2 * (float)rand() / RAND_MAX - 1) * 20);
		simulate_bullet(id, wpn, dmg, player[id].rotation + (2 * (float)rand() / RAND_MAX - 1) * 20);
		simulate_bullet(id, wpn, dmg, player[id].rotation + (2 * (float)rand() / RAND_MAX - 1) * 20);
		simulate_bullet(id, wpn, dmg, player[id].rotation + (2 * (float)rand() / RAND_MAX - 1) * 20);
		simulate_bullet(id, wpn, dmg, player[id].rotation + (2 * (float)rand() / RAND_MAX - 1) * 20);
		return 0;
	case 5:
	case 4:
	case 3: //scoped or melee weapons
		if (player[id].wpntable[wpn].status == 2) {
			dmg = weapons[wpn].weapondamage_z1;
			break;
		} else if (player[id].wpntable[wpn].status == 3) {
			dmg = weapons[wpn].weapondamage_z2;
			break;
		}
	case 2: //burst weapon
		if (player[id].wpntable[wpn].status == 2) {
			dmg = 0.64 * weapons[wpn].weapondamage;
			simulate_bullet(id, wpn, dmg, player[id].rotation - 6 + 12 * (float)rand() / RAND_MAX);
			simulate_bullet(id, wpn, dmg, player[id].rotation + 6 + 8 * (float)rand() / RAND_MAX);
			simulate_bullet(id, wpn, dmg, player[id].rotation - 6 - 8 * (float)rand() / RAND_MAX);
			return 0;
		}
	case 1: //silencer does nothing
	case 0: //regular
		dmg = weapons[wpn].weapondamage;
		break;
	default:
		return 1;
	}

	simulate_bullet(id, wpn, dmg, player[id].rotation + (2 * (float)rand() / RAND_MAX - 1) * weapons[wpn].accuracy);
	INVOKE("attack", "i", id);
	return 0;
}
/*
 int OnHit(int hitter, int victim, unsigned char wpn, short dmg)
 Return Values:
 0 - OK
 */
int OnHit(int hitter, int victim, unsigned char wpn, short dmg)
{
	if (player[victim].health == 0 ) return 0;
	short newarmor = (short)player[victim].armor - dmg;
	if (newarmor < 0) newarmor = 0;
	short newhealth = (short)player[victim].health - dmg + ((short)player[victim].armor - newarmor) * 3 / 10;

	if (newhealth > 0) {
		player[victim].health = newhealth;
		player[victim].armor = newarmor;
		SendHitMessage(victim, hitter, player[victim].health, player[victim].armor);
	} else {
		OnKill(hitter, victim, wpn);
	}

	return 0;
}

int OnBuyAttempt(int id, int wpnid)
{
	//order of checks: buyzone, buytime, money, unbuyable, already have

	//buyzone check
	int b = 0;
	int tilex = *player[id].x / 32;
	int tiley = *player[id].y / 32;
	if (player[id].team == 1) {
		for (; b <= tspawncount; b++) {
			if (abs(tilex - tspawnx[b]) <= 2 && abs(tiley - tspawny[b]) <= 2) {
				b = 0;
				break;
			}
		}
	} else if (player[id].team == 2) {
		for (; b <= ctspawncount; b++) {
			if (abs(tilex - ctspawnx[b]) <= 2 && abs(tiley - ctspawny[b]) <= 2) {
				b = 0;
				break;
			}
		}
	}

	if (b > 0) {
		SendBuyFailedMessage(id, 255);
		return 1;
	}
	////
	
	//TODO: buytime check
	//will be implemented with standard gamemode.
	////

	//money check
	if (wpnid != 57 && wpnid != 58 && player[id].money < weapons[wpnid].price) {
		SendBuyFailedMessage(id, 253);
		return 1;
	}
	////

	//unbuyable or doesn't exist or wrong team check
	if (weapons[wpnid].name == NULL || weapons[wpnid].team == 3) {
		SendBuyFailedMessage(id, 244);
		return 1;
	}
	

	if (weapons[wpnid].team != 0 && weapons[wpnid].team != player[id].team) {
		SendBuyFailedMessage(id, 252);
		return 1;
	}
	////

	//already equipped check
	if (player[id].wpntable[wpnid].status > 0) {
		SendBuyFailedMessage(id, 251);
		return 1;
	}
	////

	//passed all the checks
	return 0;

	/* SendBuyFailedMessage param 2:
	 * 242 nothing
	 * 243 Grenade rebuying is not allowed at this server
	 * 244 it's not allowed to buy that weapon at this server
	 * 245 you can't carry more of this
	 * 246 you can't carry more of this
	 * 247 you can't carry an additional weapon
	 * 248 you can't buy more ammo
	 * 249 you are not allowed to buy anything
	 * 250 buying is not allowed
	 * 251 you have already this or something better
	 * 252 you can't buy this item;
	 * 253 insufficient fund;
	 * 254 buytime passed;
	 * 255 you are not in a buyzone
	 */
}

int OnBuy(int id, int wpnid)
{
	int i;
	switch (wpnid) {
	case 57: //armor
	case 58: {//armor
		if (player[id].armor < weapons[wpnid].price / 10) {
			if (player[id].money > weapons[wpnid].price) {
				player[id].money -= weapons[wpnid].price - player[id].armor * 10;
				player[id].armor = weapons[wpnid].price / 10;
			} else {
				SendBuyFailedMessage(id, 253);
				return 1;
			}
		} else {
			SendBuyFailedMessage(id, 251);
			return 1;
		}
		break;
	}
	case 61: { //primary ammo
		int bought = 0;
		for (i = 0; i <= 0xFF; i++) {
			if (weapons[i].slot == 1 && player[id].wpntable[i].status > 0 &&
				player[id].wpntable[i].ammo2 < weapons[i].ammo2) { //if player has non-full weapon in slot 1
				player[id].wpntable[i].ammo2 = weapons[i].ammo2;
				bought = 1;
			}
		}
		if (bought == 0) {
			SendBuyFailedMessage(id, 248);
			return 1;
		} else
			player[id].money -= weapons[wpnid].price;
		break;
	}
	case 62: {//secondary ammo
		int bought = 0;
		for (i = 0; i <= 6; i++) {
			if (player[id].wpntable[i].status > 0 &&
				player[id].wpntable[i].ammo2 < weapons[i].ammo2) { //if player has non-full pistol
				player[id].wpntable[i].ammo2 = weapons[i].ammo2;
				bought = 1;
			}
		}
		if (bought == 0) {
			SendBuyFailedMessage(id, 248);
			return 1;
		}
		else
			player[id].money -= weapons[wpnid].price;
		break;
	}
	default: {
		player[id].money -= weapons[wpnid].price;
		if (weapons[wpnid].slot > 0) {
			player[id].actualweapon = wpnid;
			if (weapons[wpnid].slot < 3) { //dont increase # of weapons only for slot 1 or 2
				for (i = 0; i <= 0xFF; i++) {
					if (player[id].wpntable[i].status > 0 && weapons[i].slot == weapons[wpnid].slot) {
						RemovePlayerWeapon(id, i);
						SendDropMessage(id, i, player[id].wpntable[i].ammo1, player[id].wpntable[i].ammo2);
						break;
					}
				}
			}
		}
		GivePlayerWeapon(id, wpnid);
		break;
	}
	}
	//printf("%s bought %s!\n", player[id].name, weapons[wpnid].name);
	SendBuyMessage(id, wpnid);
	return 0;
}

int OnKill(int hitter, int victim, int wpnid)
{
	if (player[victim].health == 0 ) return 0;
	player[victim].health = 0;
	player[victim].dead = 1;
	player[hitter].score++;
	player[hitter].kills++;
	player[victim].deaths++;
	if (player[hitter].money + 300 > 65000)
		player[hitter].money = 65000;
	else
		player[hitter].money += 300;
	RemoveAllPlayerWeapon(victim);
	SendHitMessage(victim, hitter, player[victim].health, player[victim].armor);
	SendKillMessage(hitter, victim);
	printf("%s killed %s with %s\n", player[hitter].name, player[victim].name, weapons[wpnid].name);
	return 0;
}
/*
 int OnChatMessage(int id, unsigned char *message, int team)
 Return Values:
 0 - OK
 1 - don't send it
 */
int OnChatMessage(int id, unsigned char *message, int team)
{
	if (u_strlen(message) >= 4 && strncmp((char *) message, "!log", 4) == 0)
	{
		char* log = malloc(u_strlen(message) - 5 + 1); //+1 for '\0'
		if (log == NULL)
			error_exit("Memory error ( OnChatMessage() -> log )\n");
		strncpy(log, (char *) message + 5, u_strlen(message) - 5);
		log[u_strlen(message) - 5] = '\0';
		eprintf("[LOG]: %s\n",log);
		free(log);

		return 1;
	} else if (u_strlen(message) >= 4 && strncmp((char *) message, "!fps", 4) == 0) {
		char buffer[30]; //Resulting stringlength unknown: Text = 24 chars
		sprintf(buffer, "Current server FPS: %d", fpsnow);
		SendMessageToPlayer(id, buffer, 1);
		return 1;
        } else if (u_strlen(message) >= 7 && strncmp((char *) message, "!uptime", 7) == 0) {
                char buffer[40];
                sprintf(buffer, "Server uptime (seconds): %d", uptime);
                SendMessageToPlayer(id, buffer, 1);
                return 1;
	}

	if (team == 1)
	{
		printf("%s: %s\n", player[id].name, message);
	}
	else if (team == 2)
	{
		printf("%s (Team): %s\n", player[id].name, message);
	}
	return 0;
}

/*
 int OnTeamChangeAttempt(int id, unsigned char team, unsigned char skin)
 Return Values:
 0 - OK
 1 - Don't join
 */
int OnTeamChangeAttempt(int id, unsigned char team, unsigned char skin){
	INVOKE("team", "iii", id, team, skin);
	return 0;
}

/*
 int OnTeamChange(int id, unsigned char team, unsigned char skin)
 Return Values:
 0 - OK
 */
int OnTeamChange(int id, unsigned char team, unsigned char skin)
{
	if (skin != 5 || team == 0)
	{
		switch (team)
		{
		case 0:
			printf("%s is now spectator\n", player[id].name);
			break;
		case 1:
			printf("%s is now terrorist\n", player[id].name);
			break;
		case 2:
			printf("%s is now counter-terrorist\n", player[id].name);
			break;
		default:
			printf("%s joined a unknown team\n", player[id].name);
			break;
		}
		player[id].health = 0;
		player[id].dead = 1;
		player[id].deaths++;
		RemoveAllPlayerWeapon(id);
		SendKillMessage(0, id);
	}
	if (team <= 2)
	{
		SendTeamChangeMessage(id, team, skin);
	}
	return 0;
}

/*
 int OnMoveAttempt(int id, unsigned short x, unsigned short y, int status)
 Return Values:
 0 - OK
 1 - Don't move
 */
int OnMoveAttempt(int id, unsigned short x, unsigned short y, int status)
{
	/*
	 int newx = (int)ceil((x)/32);
	 int newy = (int)ceil((y)/32);
	 printf("(Debug) Tilemode: %d\n", map[newx][newy].mode);
	 */
	return 0;
}

/*
 int OnDrop(int id, unsigned char wpnid, unsigned short ammo1, unsigned short ammo2, unsigned char unknown1)
 Return Values:
 0 - OK
 1 - Don't drop
 */
int OnDrop(int id, unsigned char wpnid, unsigned short ammo1, unsigned short ammo2)
{
	if ((player[id].wpntable[wpnid].status > 0) &&
		(player[id].wpntable[wpnid].ammo1 == ammo1) &&
		(player[id].wpntable[wpnid].ammo2 == ammo2))
	{
		RemovePlayerWeapon(id, wpnid);
		return 0;
	}
	return 1;
}

int OnSecond(void)
{
	ExecutePeriodicFunctions();
	CheckForTimeout();
	INVOKE("second", "");
	return 1;
}

int OnFrame(void)
{
	memmove(&lcbuffer[1], lcbuffer, sizeof(short)*(LC_BUFFER_SIZE - 1)*(MAX_CLIENTS)*2); //update lc position buffers
	CheckAllPlayerForReload();
	INVOKE("always", "");
	return 1;
}

#undef INVOKE
