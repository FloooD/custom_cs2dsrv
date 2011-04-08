/*
 * Published under GPLv3.
 * For more information take a look at the Readme
 * Copyright (c) by the authors of this file
 *
 * Author/s of this file: Jermuk, FloooD
 */

#include "functions.h"

/**
 * \fn int IsPlayerKnown(struct in_addr ip, u_short port)
 * \brief checks if a player is already known
 * \param ip client's ip
 * \param port client's port
 * \return the playerid or 0 if not found
 */
int IsPlayerKnown(struct in_addr ip, u_short port){
	int i;
	for (i = 1; i <= sv_maxplayers; i++){
		if (!strcmp(inet_ntoa(ip), inet_ntoa(player[i].ip)) && port == player[i].port){
			return i;
		}
	}
	return 0; // false is 0
}

/**
 * \fn void ClearPlayer(int id)
 * \brief clear all player variables
 */
void ClearPlayer(int id)
{
	player[id].used = 0;
	player[id].client_number = 0;
	player[id].server_number = 0;
	player[id].lastpacket = 0;
	player[id].joinstatus = 0;

	free(player[id].name);
	player[id].name = NULL;
	free(player[id].spraylogo);
	player[id].spraylogo = NULL;
	free(player[id].win);
	player[id].win = NULL;
	player[id].usgn = 0;
	player[id].version = 0;

	player[id].team = 0;
	player[id].deaths = 0;
	player[id].score = 0;

	player[id].x = &lcbuffer[0][id-1][0];
	player[id].y = &lcbuffer[0][id-1][1];

	player[id].health = 0;
	player[id].armor = 0;

	player[id].actualweapon = 0;
	player[id].reloading = 0;
	player[id].reloadtimer = 0;
	player[id].zoomtimer = 0;
	player[id].firetimer = 0;
	player[id].rcon = 0;

	int i;
	for (i = 0; i <= 0xFF; i++)
	{
		player[id].wpntable[i].status = 0;
		player[id].wpntable[i].ammo1 = 0;
		player[id].wpntable[i].ammo2 = 0;
	}
	for (i = 0; i < LC_BUFFER_SIZE; i++)
	{
		lcbuffer[i][id-1][0] = 0;
		lcbuffer[i][id-1][1] = 0;
	}
	player[id].rotation = 0;

	player[id].dead = 1;

	player[id].start = 0;
	player[id].money = mp_startmoney;

	//free(player[id].spray_payload);
	player[id].spray_payload = NULL;
	player[id].spray_payload_size = 0;
	//free(player[id].spray_data);
	player[id].spray_data = NULL;
	player[id].spray_data_size = 0;

	/* Address*/
	player[id].port = 0;
	//player[id].ip = "";
}

/**
 * \fn void ClearAllPlayer(void)
 * \brief clear all player variables
 */
void ClearAllPlayer(void)
{
	int i;
	for (i = 1; i <= MAX_CLIENTS; i++)
	{
		ClearPlayer(i);
	}
}

/**
 * \fn void CheckForTimeout(int writesocket)
 * \brief check all player for timeout
 */
void CheckForTimeout(int writesocket)
{
	int i;
	for (i = 1; i <= sv_maxplayers; i++)
	{
		if (player[i].used == 1 && PlayerTimeout(i))
		{
			printf("Client %d timed out!\n", i);
			SendLeaveMessage(i);
			ClearPlayer(i);
		}
	}
}
/**
 * \fn int GivePlayerWeapon(int id, int wpnid)
 * \brief adds an specific weapon to a player
 * \param id player-id
 * \param wpnid weapon-id
 * \return the weapon-id or 0 if not found
 */
int GivePlayerWeapon(int id, int wpnid)
{
	player[id].wpntable[wpnid].status = 1;
	player[id].wpntable[wpnid].ammo1 = weapons[wpnid].ammo1;
	player[id].wpntable[wpnid].ammo2 = weapons[wpnid].ammo2;
	return wpnid;
}
/**
 * \fn void RemovePlayerWeapon(int id, int wpnid)
 * \brief removes an specific weapon to a player
 * \param id player-id
 * \param wpnid weapon-id
 */
void RemovePlayerWeapon(int id, int wpnid)
{
	player[id].wpntable[wpnid].status = 0;
	player[id].wpntable[wpnid].ammo1 = 0;
	player[id].wpntable[wpnid].ammo2 = 0;
}
/**
 * \fn void RemoveAllPlayerWeapon(int id)
 * \brief remove all weapons from a player
 * \param id player-id
 */
void RemoveAllPlayerWeapon(int id)
{
	int wpnid;
	for (wpnid = 0; wpnid <= 0xFF; wpnid++)
	{
		player[id].wpntable[wpnid].status = 0;
		player[id].wpntable[wpnid].ammo1 = 0;
		player[id].wpntable[wpnid].ammo2 = 0;
	}
}

/**
 * \fn int PlayerTimeout(int id)
 * \brief checks if a specific player is timeout
 * \param id player-id
 * \return 1 - timeout; 0 - success
 *
 */
int PlayerTimeout(int id)
{
	//id = 0;

	int actualtime = mtime();

	if (((player[id].lastpacket + TIMEOUT * 1000) < actualtime)
			&& player[id].lastpacket != 0)
	{
		return 1;
	}
	return 0;
}

/**
 * \fn unsigned short GetServerStatus(void)
 * \brief needs for serverlist
 * \return serverstatus
 */
unsigned short GetServerStatus(void)
{
	unsigned short serverstatus = 0;
	if (strcmp((char*) sv_password, ""))
	{
		serverstatus += 1;
	}
	if (sv_usgnonly == 1)
	{
		serverstatus += 2;
	}
	if (sv_fow == 1)
	{
		serverstatus += 4;
	}
	if (sv_friendlyfire == 1)
	{
		serverstatus += 8;
	}
	if (1) //Same Version?
	{
		serverstatus += 16;
	}
	if (sv_gamemode != 0)
	{
		serverstatus += 32;
	}
	if (1) //LuaScripts
	{
		serverstatus += 64;
	}
	if (1) //Dedicated
	{
		serverstatus += 128;
	}
	return serverstatus;
}

/**
 * \fn unsigned short endian_swap_short(unsigned short *x)
 * \brief swap the endian for a short
 * \param *x pointer to an unsigned short
 * \return the endian-changed unsigned short
 */
unsigned short endian_swap_short(unsigned short *x)
{
	*x = (*x >> 8) | (*x << 8);
	return *x;
}
/**
 * \fn unsigned int endian_swap_int(unsigned int *x)
 * \brief swap the endian for an int
 * \param *x pointer to an int
 * \return the endian-changed int
 */
unsigned int endian_swap_int(unsigned int *x)
{
	*x = (*x >> 24) | ((*x << 8) & 0x00FF0000) | ((*x >> 8) & 0x0000FF00) | (*x
			<< 24);
	return *x;
}

/**
 * \fn int ValidatePacket(char *message, int id)
 * \brief validate if the first two bytes follow the numbering for a player and rise them if necessary
 * \param *message pointer to the message
 * \param id player-id
 * \return 0 if invalid; 1 if valid
 */
int ValidatePacket(unsigned char *message, int id){
	unsigned short cid = *(unsigned short *)message; // first two bytes in little endian = short (cid is in LE so we're good)
	if (cid&1) // checks the LSBit, alternative  -> if (cid&1), not sure what the rationale of this is.
		// TODO: this breaks if the player loses connection for a while, etc
		if (cid < player[id].client_number - 2 || cid > player[id].client_number + 2) // makes sure that cid is within 4 of the last time
			printf("Client ID is not synchronized with the server. Resynchronizing...");
			//return 0;

	if (cid - 1 > player[id].client_number)
		player[id].client_number = cid;
	return 1;
}
/**
 * \fn void PacketConfirmation(char *message, int id, int writesocket)
 * \brief sends an confirmation to a player if necessary
 * \param *message pointer to the message
 * \param id player-id
 */
void PacketConfirmation(unsigned char *message, int id)
{
	unsigned short *pTempNummer = (unsigned short *) message;
	/*
	 unsigned short *pTempNummer = malloc(sizeof(unsigned short));
	 pTempNummer[0] = message[0];
	 pTempNummer[1] = message[1];
	 */
	if (*pTempNummer % 2 == 0)
	{
		int stringsize = 3;
		unsigned char *buffer = malloc(stringsize);
		buffer[0] = 0x01;
		memcpy(buffer + 1, pTempNummer, 2);

		SendToPlayer(buffer, stringsize, id, 0);

		free(buffer);
	}
	//free(pTempNummer);
}
/**
 * \fn int CheckPlayerData(char *password)
 * \brief check all player data and looks if he/she can join
 * \param *password pointer to the password
 * \return the error number or 0
 */
int CheckPlayerData(unsigned char *password)
{
	if (strcmp((char*) sv_password, (char*) password) != 0)
	{
		printf("Password wrong!\n");
		return 1;
	}
	if (sv_maxplayers == onlineplayer)
	{
		printf("Server full!\n");
		return 3;
	}
	/*
	 0 - Normal
	 1 - Password wrong
	 2 - USGN only
	 3 - Server full
	 4 - Banned
	 5 - Another Game Version
	 6 - ??
	 7 - Can't validate user (USGN offline)
	 8 - ??
	 9 - Connection already exists
	 10 - Wrong client (pre_authcode)
	 11 - Different map (temp, etc.)
	 12 - Map unknown (maptransfer disabled)
	 13++ - Failed to join
	 */

	return 0;
}

/**
 * \fn unsigned char *GetEncodedString(unsigned char *string, int length)
 * \brief convert the playername into cs2d format
 * \param *string playername
 * \param length length from playername
 * \return the converted name
 */
unsigned char *GetEncodedString(unsigned char *string, int length)
{
	unsigned char *buffer = malloc(length + 1); //+1 need for \0
	if (buffer == NULL)
		error_exit("Memory error ( GetEncodedString() )\n");

	int i;
	for (i = 0; i <= length; i++) //length and not length-1 because malloc(length+1)
	{
		if ((i % 3) == 0)
		{
			buffer[i] = string[i] + 110;
		}
		else if ((i % 3) == 1)
		{
			buffer[i] = string[i] + 97;
		}
		else if ((i % 3) == 2)
		{
			buffer[i] = string[i] + 109;
		}
	}
	buffer[length] = '\0';

	return buffer;
}

/**
 * \fn void CheckAllPlayerForReload(int writesocket)
 * \brief check all player if their reload is ended,
 * send message if necessary, and rise their magazine
 */
void CheckAllPlayerForReload(int writesocket)
{
	int i;
	for (i = 1; i <= sv_maxplayers; i++) {
		if (player[i].reloading != 0 && player[i].reloadtimer <= mtime()) {
			if ((player[i].wpntable[player[i].reloading].ammo2 -=
				weapons[player[i].reloading].ammo1 -
				player[i].wpntable[player[i].reloading].ammo1) < 0)
			{
				player[i].wpntable[player[i].reloading].ammo1 =
					weapons[player[i].reloading].ammo1 + player[i].wpntable[player[i].reloading].ammo2;
				player[i].wpntable[player[i].reloading].ammo2 = 0;
			} else { //normal
				player[i].wpntable[player[i].reloading].ammo1 = weapons[player[i].reloading].ammo1;
			}
			SendReloadMessage(i, 2);
			player[i].reloading = 0;
		}
	}
}

struct in_addr GetIp(char *name)
{
	struct in_addr ip;
	struct hostent *dnsresolve;
	if ((dnsresolve = gethostbyname(name)) == NULL)
	{
		printf("Error: Failed to resolve %s!\n", name);
		ip.s_addr = inet_addr("85.214.102.60");
		return ip; //return usgn ip
	}
	memcpy(&ip, dnsresolve->h_addr_list[0], dnsresolve->h_length);

	return ip;
}

int UsgnRegister(int writesocket)
{
	struct sockaddr_in tempclient;

	tempclient.sin_family = AF_INET;
	tempclient.sin_port = htons(36963);
	tempclient.sin_addr = GetIp("usgn.de");

	unsigned char *buffer = malloc(4);
	if (buffer == NULL)
		error_exit("Memory error ( UsgnRegister() )");
	int position = 0;

	buffer[position] = 1;
	position++;
	buffer[position] = 0;
	position++;
	buffer[position] = 27;
	position++;
	buffer[position] = 1;
	position++;

	//send_now(buffer, 4, tempclient);
	udp_send(writesocket, buffer, 4, &tempclient); // DO NOT QUEUE
	free(buffer);

	printf("[USGN] Sent ADD request to %s...\n", inet_ntoa(tempclient.sin_addr));
	return 0;
}

int UsgnUpdate(int writesocket)
{
	struct sockaddr_in tempclient;

	tempclient.sin_family = AF_INET;
	tempclient.sin_port = htons(36963);
	tempclient.sin_addr = GetIp("usgn.de");

	unsigned char *buffer = malloc(4);
	if (buffer == NULL)
		error_exit("Memory error ( UsgnRegister() )");
	int position = 0;

	buffer[position] = 1;
	position++;
	buffer[position] = 0;
	position++;
	buffer[position] = 28;
	position++;
	buffer[position] = 2;
	position++;

	send_now(buffer, 4, tempclient);
	//udp_send(writesocket, buffer, 4, &tempclient);
	free(buffer);

	printf("[USGN] Sent UPDATE request to %s...\n", inet_ntoa(tempclient.sin_addr));
	return 0;
}

void ExecuteFunctionsWithTime(int writesocket) //called once a second
{
	uptime++;
	if (uptime % 5 == 0) { //execute every 5 seconds
		SendPingList(writesocket);
		//SendMessageToAll("This is an alpha version! Don't play at it!", 1); //Do not remove or change this until server reaches beta status
		PingAllPlayer();
	} else if (uptime % 50 == 0) {
		UsgnUpdate(writesocket);
	}
	OnSecond();
}

size_t u_strlen(unsigned char* buffer)
{
	return strlen((char*) buffer);
}

/**
 * \fn int line_seg_sqr(float ex, float ey, float sqx, float sqy, float sqhlen, float *ox, float *oy)
 * \brief finds first collision between a line segment and a square.
 * 		origin is defined as line segment's starting point.
 * \param ex x coordinate of line segment's ending point
 * \param ey y coordinate of line segment's ending point
 * \param sqx x coordinate of square's center
 * \param sqy y coordinate of square's center
 * \param sqhlen half of length of square's side
 * \param *ox where to store x coordinate of intersection
 * \param *oy where to store y coordinate of intersection
 * \return 0 - no collision; 1 - collision at *ox, *oy
 */
int line_seg_sqr(float ex, float ey, float sqx, float sqy, float sqhlen, float *ox, float *oy)
{
	if (!ox) {float x; ox = &x;}
	if (!oy) {float y; oy = &y;}
	int cx = (fabsf(sqx) <= sqhlen);
	int cy = (fabsf(sqy) <= sqhlen);
	if (cx && cy) {
		if (fabsf(ex - sqx) <= sqhlen && fabsf(ey - sqy) <= sqhlen) {
			*ox = ex; *oy = ey;
			return 1;
		}
		sqhlen *= -1;
	}
	*ox = (ex >= 0) ? sqx - sqhlen : sqx + sqhlen;
	*oy = (ey >= 0) ? sqy - sqhlen : sqy + sqhlen;
	if ((ex != 0) && ((!(cx ^ cy) && ((fabsf(ey * *ox) >= fabsf(ex * *oy)) ^ (sqhlen < 0))) || (cy && !cx)))
		goto collision;
	if (ey == 0) goto no_collision;
	float temp = ex; ex = ey, ey = temp;
	temp = sqx; sqx = sqy; sqy = temp;
	float *tptr; tptr = ox; ox = oy; oy = tptr;
	collision:
	if ((*ox * ex >= 0) && (fabsf(*ox) <= fabsf(ex))) {
		*oy = (*ox) * ey / ex;
		if (fabsf(*oy - sqy) <= fabsf(sqhlen))
			return 1;
	}
	no_collision:
	return (*ox = *oy = 0);
}
/**
 * \fn void simulate_bullet(int id, unsigned char wpn, short dmg, float rot)
 * \brief simulates a bullet
 * \param id id of player who fired the bullet
 * \param wpn wpn from which the bullet was fired
 * \param dmg damage of the bullet
 * \param rot angle of the bullet
 */
void simulate_bullet(int id, unsigned char wpn, short dmg, float rot)
{
	float start_x = (float)(*player[id].x);
	float start_y = (float)(*player[id].y);
	float range_x = 3.0 * (float)weapons[wpn].range * sinf(rot * 0.0174532925);
	float range_y = -3.0 * (float)weapons[wpn].range * cosf(rot * 0.0174532925);
	int tile_x;
	int tile_y;
	int temp_x = (tile_x = ((int)start_x) / 32);
	int temp_y = (tile_y = ((int)start_y) / 32);
	int tilemode;
	
	int inc_x = 0;
	if (rot < 0)
		inc_x = -1;
	else if (rot > 0 && rot != 180)
		inc_x = 1;
		
	int inc_y = 0;
	if (fabsf(rot) > 90)
		inc_y = 1;
	else if (fabsf(rot) < 90)
		inc_y = -1;

	while (1) {
		if (tile_x < 0 || tile_y < 0)
			break;
		tilemode = map.tiles[tile_x][tile_y].mode;
		if (tilemode == 1 || tilemode == 3 || tilemode == 4)
		{
			float temp_rx = range_x;
			float temp_ry = range_y;
			line_seg_sqr(temp_rx, temp_ry , 32 * (temp_x) + 16 - start_x, 32 * (temp_y) + 16 - start_y, 16, &range_x, &range_y);
			break;
		}
		
		if (0!=inc_x && 1==line_seg_sqr(range_x, range_y, 32 * (temp_x + inc_x) + 16 - start_x, 32 * (temp_y) + 16 - start_y, 16, NULL, NULL))
			tile_x = temp_x + inc_x;
		if (0!=inc_y && 1==line_seg_sqr(range_x, range_y, 32 * (temp_x) + 16 - start_x, 32 * (temp_y + inc_y) + 16 - start_y, 16, NULL, NULL))
			tile_y = temp_y + inc_y;
		if (tile_x == temp_x && tile_y == temp_y)
			break;
		temp_x = tile_x;
		temp_y = tile_y;
	}

	int frames = (int)player[id].latency * fpsnow / 1000;
	if (frames > sv_lcbuffer) frames = sv_lcbuffer;
	int i;
	for (i = 1; i <= sv_maxplayers; i++) {
		if (player[i].dead == 0
		&& player[i].joinstatus >= 4
		&& player[i].used == 1
		&& i != id
		&& (player[i].team != player[id].team || sv_friendlyfire == 1))
			if (line_seg_sqr(range_x, range_y, lcbuffer[frames][i - 1][0] - start_x, lcbuffer[frames][i - 1][1] - start_y, 12, NULL, NULL))
				OnHit(id, i, wpn, dmg);
	}
}
