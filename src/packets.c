/*
 * Published under GPLv3.
 * For more information take a look at the Readme
 * Copyright (c) by the authors of this file
 *
 * Author/s of this file: Jermuk, Lee
 */

#include "packets.h"

#define INT(i) (((byte*)&(i))[0]), (((byte*)&(i))[1]), (((byte*)&(i))[2]), (((byte*)&(i))[3])

/**
 * \fn int unknown(unsigned char *message, int length, unsigned char *buffer, int size, int position)
 * \brief every unknown packet lands here
 * \param *message pointer to the unknown message
 * \param length sizeof message
 * \param *buffer pointer to the udp-packet
 * \param size sizeof buffer
 * \param position where the unknown message in buffer begins
 * \return read bytes (specific: parameter length)
 */
int unknown(stream* packet, int pid){
	eprintf("\tUnknown packet: %d\n",pid);
	int l = packet->size;
	byte* msg = Stream.read(packet, l);
	just(msg,l);
	return 0;
}

/**
 * \fn int connection_setup_unknown(unsigned char *message, int length, struct in_addr ip, unsigned short port)
 * \brief handles a connection setup for unknown player
 * \param *message pointer to the unknown message
 * \param length sizeof message
 * \param ip clients ip
 * \param port clients port
 * \return read bytes (specific: 3)
 */
int connection_setup_unknown(stream* packet, struct sockaddr_in *newclient){
	if (!Stream.read(packet, 2)){
		printf("Invalid packet (connection_setup_unknown)!\n");
		return 0;
	}
	return 1;
}

/**
 * \fn int connection_setup_known(unsigned char *message, int length, struct in_addr ip, unsigned short port)
 * \brief handles a connection setup for known player
 * \param *message pointer to the unknown message
 * \param length sizeof message
 * \param ip clients ip
 * \param port clients port
 * \return read bytes (specific: 3)
 */
int connection_setup_known(stream* packet, int id){
	if (!Stream.read(packet,2)){
		printf("Invalid packet (connection_setup_known)!\n");
		return 0;
	}
	return 1;
}

/**
 * \fn int ping_ingame(unsigned char *message, int length, int id)
 * \brief handles a answer from the server ingame ping. this function calculates the latency
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 5)
 */
int ping_ingame(stream *packet, int id){
	if (!Stream.read(packet,4)){
		printf("Invalid packet (ping_ingame)!\n");
		return 0;
	}
	player[id].latency = mtime() - player[id].start;
	return 1;
}

/**
 * \fn int confirmation_unknown(unsigned char *message, int length, struct in_addr ip, unsigned short port)
 * \brief handles a confirmation for unknown player
 * \param *message pointer to the unknown message
 * \param length sizeof message
 * \param ip clients ip
 * \param port clients port
 * \return read bytes (specific: 3)
 */
int confirmation_unknown(stream* packet, struct sockaddr_in *newclient){
	if (!Stream.read(packet, 2)){
		printf("Invalid packet (confirmation_unknown)!\n");
		return 0;
	}
	return 1;
}

/**
 * \fn int confirmation_known(unsigned char *message, int length, int id)
 * \brief handles a confirmation for known player
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 3)
 */
int confirmation_known(stream *packet, int id){
	if (!Stream.read(packet, 2)){
		printf("Invalid packet (confirmation_known)!\n");
		return 0;
	}
	return 1;
}

/**
 * \fn int fire(unsigned char *message, int length, int id)
 * \brief handles a fire
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 1)
 */
int fire(stream *packet, int id){
	switch (OnFire(id)){
	case 0:
		SendFireMessage(id);
	}
	return 1;
}

/**
 * \fn int advanced_fire(unsigned char *message, int length, int id)
 * \brief handles a advanced_fire (right click)
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 1)
 */
int advanced_fire(stream* packet, int id){
	unsigned int status = Stream.read_byte(packet);

	switch (OnAdvancedFire(id, status)){
	case 0:
		SendAdvancedFireMessage(id, status);
		break;
	}
	return 1;
}

/**
 * \fn int buy(stream* packet, int id)
 * \brief handles a buy request
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 3)
 */
int buy(stream* packet, int id){
	CHECK_STREAM(packet, 2);
	int wpnid = Stream.read_byte(packet);

	switch (OnBuyAttempt(id, wpnid)){
	case 0:
		OnBuy(id, wpnid);
		break;
	}
	Stream.read_byte(packet); // TODO: profile last byte in buy()
	return 1;
}

/**
 * \fn int rotupdate(stream* packet, int id)
 * \brief handles a rotation update
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 5)
 */
int rotupdate(stream* packet, int id){
	CHECK_STREAM(packet, 4);
	float temprotation = Stream.read_float(packet);
	if (temprotation >= -180 && temprotation <= 180)
		SendRotUpdate(id, (player[id].rotation = temprotation));
	Stream.read(packet, 3);
	return 1;
}

/**
 * \fn int posupdatewalk(stream* packet, int id)
 * \brief handles a position walk update
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 5)
 */
int posupdatewalk(stream* packet, int id)
{
	CHECK_STREAM(packet, 4);
	unsigned short tempx = Stream.read_short(packet);
	unsigned short tempy = Stream.read_short(packet);

	switch (OnMoveAttempt(id, tempx, tempy, 0))
	{
	case 0:
		SendPosUpdate(id, tempx, tempy, 0);
		*player[id].x = tempx;
		*player[id].y = tempy;
		break;
	case 1:
		SendPosUpdate(id, *player[id].x, *player[id].y, 0);
	}
	return 1;
}
/**
 * \fn int posupdaterun(stream* packet, int id)
 * \brief handles a position run update
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 5)
 */
int posupdaterun(stream* packet, int id){
	CHECK_STREAM(packet, 4);
	unsigned short tempx = Stream.read_short(packet);
	unsigned short tempy = Stream.read_short(packet);
	switch (OnMoveAttempt(id, tempx, tempy, 0)){
	case 0:
		SendPosUpdate(id, tempx, tempy, 1);
		*player[id].x = tempx;
		*player[id].y = tempy;
		break;
	case 1:
		SendPosUpdate(id, *player[id].x, *player[id].y, 1);
	}
	return 1;
}
/**
 * \fn int posrotupdatewalk(stream* packet, int id))
 * \brief handles a position walk and rotation update
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 9)
 */
int posrotupdatewalk(stream* packet, int id)
{
	CHECK_STREAM(packet, 8);
	unsigned short tempx = Stream.read_short(packet);
	unsigned short tempy = Stream.read_short(packet);
	float rotation = Stream.read_float(packet);
	if (rotation >= -180 && rotation <= 180)
	{
		switch (OnMoveAttempt(id, tempx, tempy, 0))
		{
		case 0:
			//if(rotation < 0) rotation += 360;
			SendPosRotUpdate(id, tempx, tempy, 0, rotation);
			player[id].rotation = rotation;
			*player[id].x = tempx;
			*player[id].y = tempy;
			break;
		case 1:
			SendPosRotUpdate(id, *player[id].x, *player[id].y, 0, player[id].rotation); // no change?

		}
	}
	return 1;
}

/**
 * \fn int posrotupdaterun(stream* packet, int id))
 * \brief handles a position run and rotation update
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 9)
 */
int posrotupdaterun(stream* packet, int id){
	CHECK_STREAM(packet, 8);
	unsigned short tempx = Stream.read_short(packet);
	unsigned short tempy = Stream.read_short(packet);
	float rotation = Stream.read_float(packet);

	if (rotation >= -180 && rotation <= 180){
		switch (OnMoveAttempt(id, tempx, tempy, 1)){
		case 0:
			SendPosRotUpdate(id, tempx, tempy, 1, rotation);
			player[id].rotation = rotation;
			*player[id].x = tempx;
			*player[id].y = tempy;
			break;
		case 1:
			SendPosRotUpdate(id, *player[id].x, *player[id].y, 1, player[id].rotation);
		}
	}
	return 1;
}

/**
 * \fn int respawnrequest(stream* packet, int id)
 * \brief handles a respawn request
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 1)
 */
int respawnrequest(stream* packet, int id)
{
	int tmp;
	switch (OnRespawnRequest(id)){
	case 0:
		// srand(time(NULL)); // This is fucking stupid, srand then rand -> predictable pattern in most cases
		switch (player[id].team){
		case 0:
			printf("%s tried to spawn as spectator!\n", player[id].name);
			break;
		case 1:
			tmp = rand() % tspawncount;
			printf("%s spawned as a terrorist.\n", player[id].name);
			player[id].health = 100;
			player[id].dead = 0;
			*player[id].x = (tspawnx[tmp] + 0.5) * 32;
			*player[id].y = (tspawny[tmp] + 0.5) * 32;
			SendSpawnMessage(id, *player[id].x, *player[id].y);
			break;
		case 2:
			tmp = rand() % ctspawncount;
			printf("%s spawned as a counter-terrorist.\n", player[id].name);
			player[id].health = 100;
			player[id].dead = 0;
			*player[id].x = (ctspawnx[tmp] + 0.5) * 32;
			*player[id].y = (ctspawny[tmp] + 0.5) * 32;
			SendSpawnMessage(id, *player[id].x, *player[id].y);
			break;
		}
		break;
	}
	return 1;
}
/**
 * \fn int weaponchange(stream* packet, int id)
 * \brief handles a weapon change
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 3)
 */
int weaponchange(stream* packet, int id)
{
	CHECK_STREAM(packet, 2);

	byte wpnid = Stream.read_byte(packet);
	switch (OnWeaponChangeAttempt(id, wpnid)){
	case 0:
		SendWeaponChangeMessage(id, wpnid);
	}
	Stream.read_byte(packet); // TODO: profile this
	return 1;
}

int killmsg(stream* packet, int id)
{
	if (player[id].health == 0) return 1;
	player[id].health = 0;
	player[id].dead = 1;
	player[id].deaths++;
	RemoveAllPlayerWeapon(id);
	SendKillMessage(0, id);
	printf("%s commit suicide", player[id].name);
	return 1;
}

/**
 * \fn int teamchange(stream* packet, int id)
 * \brief handles a team change
 * \param *message pointer to the message
 * \param length sizeof message
 * \param id clients player id
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 3)
 */
int teamchange(stream* packet, int id){
	CHECK_STREAM(packet, 2);
	unsigned char team = Stream.read_byte(packet);
	unsigned char skin = Stream.read_byte(packet);
	//printf("%d %d\n", team, skin); //happens in 2 stages. first is [team] 5; second is [team] [look]
	switch (OnTeamChangeAttempt(id, team, skin)){
	case 0:
		player[id].team = team;
		if (skin != 5) player[id].skin = skin;
		OnTeamChange(id, team, skin);
	}
	return 1;
}
/**
 * \fn int ping_serverlist(stream* packet, struct sockaddr_in *client)
 * \brief handles a team change
 * \param *message pointer to the message
 * \param length sizeof message
 * \param *client pointer to the client information structure
 * \param writesocket the socket where it was read
 * \return read bytes (specific: 5)
 */

int ping_serverlist(stream* packet, struct sockaddr_in *client){
	CHECK_STREAM(packet, 4);

	// <- 0000   01 00 fa [## ##] aa bb
	// -> 0000   01 00 fa [## ##] aa bb

	int csum = Stream.read_int(packet);
	byte buf[] = {1,0,0xfa,INT(csum)};

	send_now(buf, 7, *client);
	return 1;
}


int serverinfo_request(stream* packet, struct sockaddr_in *client){
	CHECK_STREAM(packet,1);
	byte* message = packet->mem;
	stream* buf = init_stream(NULL);
	if (!buf)
		error_exit("Memory error ( serverinfo_request() )");

	Stream.write_short(buf, 1);
	Stream.write_byte(buf, 0xfb);
	Stream.write_byte(buf, Stream.read_byte(packet));
	switch (message[0]){
	case 1:
	case 2:
		Stream.write_byte(buf, GetServerStatus());
		Stream.write_str(buf, sv_name);
		Stream.write_str(buf, sv_map);
		Stream.write_byte(buf, onlineplayer);
		Stream.write_byte(buf, sv_maxplayers);
		if (sv_gamemode != 0)
			Stream.write_byte(buf, sv_gamemode);
		Stream.write_byte(buf, bot_count);
		send_now(buf->mem, buf->size, *client);
	}
	free(buf);
	Stream.read(packet, packet->size);
	return 1;
}

int joinroutine_unknown(stream* packet, struct sockaddr_in *client){
	CHECK_STREAM(packet, 1);
	switch (Stream.read_byte(packet)){
	case 0x00:{
		int i;
		for (i = 1; i < MAX_CLIENTS; i++){
			if (player[i].used != 1){
				player[i].used = 1;
				player[i].ip = client->sin_addr;
				player[i].port = client->sin_port;
				player[i].joinstatus = 1;
				player[i].client_number = 4; // wtf?
				player[i].server_number = 2; // and wtf is this?
				break;
			}
		}
		stream* buf = init_stream(NULL);
		if (!buf)
			error_exit("Memory error ( joinroutine_unknown() )");

		Stream.write_short(buf, 1);
		Stream.write_short(buf, 0xfc);
		Stream.write_str(buf, pre_authcode);

		send_now(buf->mem, buf->size, *client);
		//udp_send(writesocket, buffer, stringsize, client);

		free(buf);
		break;
	}
	default:
		printf("Unexpected join data\n");
		return 0;
	}
	return 1;
}

int specpos(stream* packet, int id){
	CHECK_STREAM(packet, 8);
	int x = Stream.read_int(packet);
	int y = Stream.read_int(packet);

	switch (OnSpecmove(id, x, y)){
	case 0:
		player[id].specposx = x;
		player[id].specposy = y;
	}
	return 1;
}

int chatmessage(stream* packet, int id){
	CHECK_STREAM(packet, 4);
	byte team, *msg;

	team = Stream.read_byte(packet);
	msg = Stream.read_str2(packet);

	switch (OnChatMessage(id, msg, team))
	{
	case 0:
		SendChatMessage(id, msg, team);
		break;
	case 1:
		break;
	default:
		printf("Unknown Return value for OnChatMessage()!\n");
		return 0;
	}

	free(msg);
	return 1;
}

int joinroutine_known(stream* packet, int id){
	CHECK_STREAM(packet, 1);

	switch (Stream.read_byte(packet)){
	case 0:
		//printf("Invalid join packet\n");
		Stream.read(packet, packet->size);
		break;
	case 1:{
		if (player[id].joinstatus == 1){
			player[id].name = Stream.read_str(packet);
			byte* password = Stream.read_str(packet);
			byte* encryption1 = Stream.read_str(packet);
			player[id].version = Stream.read_short(packet);
			player[id].usgn = Stream.read_int(packet); // it's a fucking int
			player[id].spraylogo = Stream.read_str(packet);
			byte* mhash = Stream.read_str(packet);
			Stream.read_byte(packet); // we need to profile this
			player[id].win = Stream.read_str(packet);
			// we have one left over byte in this stream... CONFIRMED
			Stream.read_byte(packet);

			stream* buf = init_stream(NULL);
			int tempstatus = CheckPlayerData(password);
			switch (tempstatus){
			case 0x00:
				Stream.write(buf, (byte*)"\xfc\2\0", 3);
				Stream.write_byte(buf, id);
				Stream.write_str(buf, sv_map);
				Stream.write_str(buf, pre_authcode);
				player[id].joinstatus = 2;
				break;
			default:
				Stream.write(buf, (byte*)"\xfc\2", 2);
				Stream.write_byte(buf, tempstatus);
				player[id].joinstatus = 0;
				break;
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
			 11 - Diffrent map (temp, etc.)
			 12 - Map unknown (maptransfer disabled)
			 13++ - Failed to join
			 */

			if (!EMPTY_STREAM(buf))
				SendToPlayer(buf->mem, buf->size, id, 1);


			free(encryption1);
			free(mhash);
			free(buf);

			onlineplayer++;
		} else {
			printf("Unexpected join data (1) from %s; expected %d\n", player[id].name, player[id].joinstatus+1);
			return 0;
		}
		break;
	}
	case 3:{
		if (player[id].joinstatus == 2){
			byte* mhash = Stream.read_str(packet);
			byte* pre_authcode_respond = Stream.read_str(packet);
			byte mapstatus = Stream.read_byte(packet);

			(void)mapstatus; // sentinel for mapstatus
			free(pre_authcode_respond);
			free(mhash);

			SendToPlayer((byte*)"\xfc\4\0", 3, id, 1);
			player[id].joinstatus = 3;
		} else {
			printf("Unexpected join data (3) from %s; expected %d\n", player[id].name, player[id].joinstatus+1);
			return 0;
		}
		break;
	}
	case 5: {
		if (player[id].joinstatus == 3){
			byte* mapname = Stream.read_str(packet);

			//----------- ServerData -----------

			stream* buf = init_stream(NULL);

			Stream.write(buf, (byte*)"\xfc\6\0\xa", 3);
			Stream.write_str(buf, sv_map);
			Stream.write_str(buf, sv_name);
			byte wtf[] = { //seriously what the fuck?
					0,
					0,
					sv_friendlyfire,
					0,
					mp_roundtime,
					mp_freezetime,
					mp_c4timer,
					0x3d, //no fucking idea. not static
					0x4a, //no fucking idea. not static.
					0,
					0,
					1,
					0,
					sv_maxplayers,
					sv_fow,
					mp_specmode,
					sv_gamemode,
					mp_respawndelay,
					mp_infammo,
					1,8,51,58,11,22,32,35,80,73,0, //no fucking idea what these are.
					3, 'A', 'C', 'K'
			};
			Stream.write(buf, wtf, 34);

			SendToPlayer(buf->mem, buf->size, id, 1);
			free(buf);

			//----------- PlayerData -----------
			buf = init_stream(NULL);
			Stream.write(buf, (byte*)"\xfc\7\1", 3);
			Stream.write_byte(buf, onlineplayer);

			int i;

			for (i = 1; i <= sv_maxplayers; i++){
				if (player[i].used == 1 && player[i].joinstatus >= 1 && i != id){
					unsigned char *encodedname = GetEncodedString(player[i].name, u_strlen(player[i].name));
					unsigned short deaths = player[i].deaths;
					unsigned short score = (unsigned) (player[i].deaths + 1000);

					unsigned short x = *(player[i].x);
					unsigned short y = *(player[i].y);
					float rot = player[i].rotation;

					Stream.write_byte(buf, i);
					Stream.write_str(buf, encodedname);

					free(encodedname);

					byte wtf2[] = {
							0,
							player[i].team,
							0,
							player[i].skin,
					};
					Stream.write(buf, wtf2, 4);

					Stream.write_short(buf, score);
					Stream.write_short(buf, deaths);
					Stream.write_int(buf, x);
					Stream.write_int(buf, y);
					Stream.write_short(buf, rot);

					byte wtf3[] = {
							player[i].health, player[i].armor, 0, player[i].actualweapon, 0
					};
					Stream.write(buf, wtf3, 5);
				}
			}

			// Rest of the packet only concerns with the current player
			if (1){
				byte* encodedname = GetEncodedString(player[id].name, u_strlen(player[id].name));
				Stream.write_byte(buf, id);
				Stream.write_str(buf, encodedname);
				byte wtf[] = {
						0,
						player[i].team,
						0,
						0, // skins
						232, 3, // score, in LE 1000 = 0
						0, 0, // deaths
						0, 0, 0, 0, // x
						0, 0, 0, 0, // y
						0, 0, // rot
						0, 0, 0, 0, 0
				};
				Stream.write(buf, wtf, 23);
			}
			SendToPlayer(buf->mem, buf->size, id, 1);
			free(buf);

			//----------- PlayerData -----------
			SendToPlayer((byte*)"\xfc\7\1\0", 4, id, 1);

			//----------- HostageData -----------
			SendToPlayer((byte*)"\xfc\7\2\0", 4, id, 1);

			//----------- ItemData -----------
			SendToPlayer((byte*)"\xfc\7\3\0", 4, id, 1);

			//----------- EnityData -----------
			SendToPlayer((byte*)"\xfc\7\4\0", 4, id, 1);

			//----------- DynamicObjectData -----------
			SendToPlayer((byte*)"\xfc\7\5\0", 4, id, 1);

			//----------- ProjectileData -----------
			SendToPlayer((byte*)"\xfc\7\6\0", 4, id, 1);

			//----------- DynamicObjectImageData -----------
			SendToPlayer((byte*)"\xfc\7\7\0", 4, id, 1);
			
			//----------- uh.. SomethingData??? -----------
			SendToPlayer((byte*)"\xfc\7\x8\0", 4, id, 1);

			//----------- Final ACK ----------- c8 3 41 43 4b 1c 00 01
			SendToPlayer((byte*)"\xfc\7\xc8\3\x41\x43\x4b\x1c\0\1", 10, id, 1);

			player[id].joinstatus = 4;
			free(mapname);

			OnJoin(id);//exit(0);
		}else{
			printf("Unexpected join data 4 from %s; expected %d\n", player[id].name, player[id].joinstatus+1);
			return 0;
		}
		break;
	}
	default:
		printf("Unexpected join data (.) from %s; expected %d\n", player[id].name, player[id].joinstatus+1);
		return 0;
	}
	return 1;
}

int leave(stream* packet, int id){
	CHECK_STREAM(packet, 1);
	int status = Stream.read_byte(packet);

	OnLeave(id, status);

	onlineplayer--;
	ClearPlayer(id);
	return 1;
}

int reload(stream* packet, int id){
	CHECK_STREAM(packet, 1);

	int status = Stream.read_byte(packet);

	if (status == 1){
		SendReloadMessage(id, 1);
		player[id].reloading = player[id].actualweapon;
		player[id].reloadtimer = mtime() + weapons[player[id].actualweapon].reloadtime;
	}
	return 1;
}

int spray(stream* packet, int id){
	// 28 0 xx yy c
	//  0 1 23 45 6
	CHECK_STREAM(packet, 6);
	byte type = Stream.read_byte(packet);

	switch (type){
	case 0:{
		unsigned short x = Stream.read_short(packet), y = Stream.read_short(packet);
		unsigned char c = Stream.read_byte(packet);
		unsigned char i = (char) id;

		// Postprocessing if needed, then send back exact same data
		// xx and yy are positions, not tiles.

		SendSprayMessage(i, x, y, c);
		break;
	}
	case 2:{
		// 28 02 ## ...
		short size = Stream.read_short(packet);
		unsigned long final = (unsigned long)Stream.read_int(packet);
		byte* logo = Stream.read(packet, size-4);
		if (debug)
			printf("[SPRAY] Received spraylogo from %s (%d bytes)\n", player[id].name, size);

#ifdef ZLIB_H
		byte* payload = (byte*)malloc(final);
		int err = uncompress(payload, &final, logo, size-4);

		if(err){
			printf("[SPRAY] Invalid spraylogo received from %s.\n", player[id].name);
		}

		byte bmp_head[] = {
				'B','M', // magic number
				0x36,4,0,0, // file size
				0,0,0,0, // reserved
				0x36,0,0,0, // off set into actual data
				0x28,0,0,0,
				0x20,0,0,0,
				0x20,0,0,0,
				1,0,
				8,0,
				0,0,0,0,
				0,4,0,0,
				0x13,0xb,0,0,
				0x13,0xb,0,0,
				0,0,0,0,
				0,0,0,0
		};
		stream* bmp = init_stream(NULL);
		Stream.write(bmp, bmp_head, 54);
		Stream.write(bmp, payload, 1024);

		FILE* bmp_file = fopen("lol.bmp", "w");
		fwrite(bmp->mem,1,bmp->size,bmp_file);
		fclose(bmp_file);

//		int i, j;
//		for (i=0;i<32;i++){
//			for (j=0;j<32;j++)
//				eprintf("%2x ", payload[i+j*32]);
//			puts("\n");
//		}

		player[id].spray_payload = payload;
		player[id].spray_payload_size = final;
#endif

		player[id].spray_data = logo;
		player[id].spray_data_size = size;
		break;
	}
	}

	return 1;
}

int usgn_add(stream* packet, struct sockaddr_in *newclient){
	int code;
	if ((code = Stream.read_byte(packet)) == 1)
		printf("[USGN] Server successfully added to the serverlist..\n");
	else{
		printf("[USGN] Server NOT added to the serverlist.. (Code: %d)\n", code);
		return 0;
	}
	return 1;
}

int usgn_update(stream* packet, struct sockaddr_in *newclient){
	int code;
	if ((code = Stream.read_byte(packet)) == 2)
		printf("[USGN] Server successfully updated in the serverlist..\n");
	else{
		printf("[USGN] Server NOT updated to the serverlist.. (Code: %d)\n", code);
		return 0;
	}
	return 1;
}

int drop(stream* packet, int id){
	CHECK_STREAM(packet, 6);

	unsigned char wpnid, unknown3;
	unsigned short ammo1, ammo2;

	wpnid = Stream.read_byte(packet);
	ammo1 = Stream.read_short(packet);
	ammo2 = Stream.read_short(packet);
	unknown3 = Stream.read_byte(packet);

	switch (OnDrop(id, wpnid, ammo1, ammo2)){
	case 0:
		SendDropMessage(id, wpnid, ammo1, ammo2);
	}
	return 1;
}

int rcon_pw(stream* packet, int id){
	// 236 ln pw
	// 0 1 ln
	CHECK_STREAM(packet, 1);
	unsigned char key[] = "mysecretremotecontrolpmysecret";

	byte* msg = Stream.read_str(packet);
	int expected = strlen((char*)msg);
	byte* pw = malloc(expected);

	int i;
	for (i = 0; i < expected; i++){
		pw[i] = (byte) (msg[i] - key[i]);
	}
	pw[expected] = '\0';

	// Check against actual rcon
	if (!strcmp((char*)pw, (char*)sv_rcon)){
		printf("[Rcon_pw] Success\n");
		player[id].rcon = 1;
		//SendRconPwMessage(id, message, length, 1);
	}else{
		printf("[Rcon_pw] Bad attempt by %s.\n", player[id].name);
		stream* buf = init_stream(NULL);
		Stream.write_byte(buf, 236);
		Stream.write_str(buf, msg);
		SendRconPwMessage(id, buf->mem, buf->size, 0);
		free(buf);
	}
	return 1;
}

int rcon(stream* packet, int id){
	CHECK_STREAM(packet, 4); // rcon must be ln 1, 1 word for len even if no message
	const char* rcon_hash = "rconcm", *rcon_pw_hash = "mysecretremotecontrolp";
	byte* pw = Stream.read_str(packet);
	byte* msg = Stream.read_str2(packet);
	int i,h=strlen(rcon_hash),ph=strlen(rcon_pw_hash);
	for (i=0;i<strlen((char*)pw);i++)
		pw[i] = (0xff+pw[i]-(byte)rcon_pw_hash[i%ph])%0xff;
	for (i=0;i<strlen((char*)msg);i++)
		msg[i] = (0xff+msg[i]-(byte)rcon_hash[i%h])%0xff;

	if (player[id].rcon){
		printf("[RCON:%s] %s\n", player[id].name, msg);
		parse((char*)msg);
	}else{
		printf("[RCON] Bad attempt by %s: %s.\n", player[id].name, msg);
		return 0;
	}

	return 1;
}




// auxiliary functions
//Stream.quanta = 0xff;
stream* init_stream(stream* s){
	if (!s || !s->n || !s->mem) s = (stream*)malloc(sizeof(struct stream_));
	s -> mem = (byte*)malloc(Stream.quanta);
	s -> cur = s->mem;
	s -> size = 0;
	s -> n = 1;
	return s;
}

byte* s__read__(stream* s, int n){
	byte* m = s->mem;
	if (n > s->size) return 0;
	s->size -= n;
	s->mem += n;
	return m;
	//return (byte*)(n<=s->size && (((s->size-=n) && 0) + ((s->mem+=n) && 0) + m));
}

int s__write__(stream* s, byte* a, int n){
	if (!s) return 0;
	if (!s->n || !s->mem) init_stream(s);
	int lg = (s->size + n) - (Stream.quanta*s->n);
	if (lg > 0) {
		byte* t = s->mem;
		s->mem = (byte*)malloc(Stream.quanta * (s->n += 1+lg/Stream.quanta));
		if (!s->mem) return 0;
		memcpy(s->mem, t, s->size);
		s->cur = s->mem+s->size;
	}
	//printf("%d %d\n", s->mem, s->cur);
	memcpy(s->cur, a, n);
	//return 0;
	s->cur+=n;
	s->size += n;
	return 1;
}

byte* s__peek__(stream* s){
	return s->mem;
}

int s__seek__(stream* s, int pos){
	if (pos < 0) return 0;
	if (pos < s->size) return (int)(s->cur = s->mem+pos);
	else return (int)(s->cur = s->mem + s->size);
}

int s__trim__(stream* s, int pos){
	if (pos < 0) return 0;
	if (pos < s->size) return
		((s->cur = s->mem+pos) && (s->size = pos));
	else return (int)(s->cur = s->mem + s->size);
}

// specific cases
byte read_byte(stream* s){
	byte* i = (byte*)Stream.read(s, 1);
	return i ? *i : 0u;
}

int write_byte(stream* s, byte c){
	return Stream.write(s, &c, 1);
}

short read_short(stream* s){
	short* i = (short*)Stream.read(s, 2);
	return i ? *i : 0;
}

int write_short(stream* s, short c){
	return Stream.write(s, (byte*)&c, 2);
}

int read_int(stream* s){
	int* i = (int*)Stream.read(s, 4);
	return i ? *i : 0;
}

int write_int(stream* s, int c){
	return Stream.write(s, (byte*)&c, 4);
}

float read_float(stream* s){
	float* f = (float*)Stream.read(s, 4);
	return f ? *f : 0.0;
}

int write_float(stream* s, float c){
	return Stream.write(s, (byte*)&c, 4);
}

byte* read_str(stream* s){
	byte i = read_byte(s);
	if (!i) return (byte*)"";
	byte* str = (byte*)malloc(i+1), *src = Stream.read(s, i);
	if (!src) return 0;
	memcpy(str, src, i);
	str[i] = 0;
	return str;
}

byte* read_str2(stream* s){
	short i = read_short(s);
	if (!i) return (byte*)"";
	byte* str = (byte*)malloc(i+1), *src = Stream.read(s, i);
	if (!src) return 0;
	memcpy(str, src, i);
	str[i] = 0;
	return str;
}

int write_str2(stream* s, byte* str){
	int n = strlen((char*)str);
	byte* str_ = (byte*)malloc(n+2);
	memcpy(str_+2, str, n);
	*str_++ = (n++)&0xff;
	*str_-- = ((n++)-1)/0xff;
	return Stream.write(s, str_, n);
}

int write_str(stream* s, byte* str){
	int n = strlen((char*)str)+1;
	byte* str_ = (byte*)malloc(n--);
	memcpy(str_+1, str, n);
	*str_ = n++;
	return Stream.write(s, str_, n);
}

byte* read_line(stream* s){
	byte* raw = Stream.peek(s);
	int n = 0;
	while(*raw++ != '\n' && n < s->size)n++;
	if (!n) return 0;
	byte* str = (byte*)malloc(n+1), *src = Stream.read(s, n+1);
	memcpy(str, src, n);
	str[n] = 0;
	return str;
}

int write_line(stream* s, byte* str){
	int n = strlen((char*)str)+1;
	byte* str_ = (byte*)malloc(n--);
	memcpy(str_, str, n);
	str_[n++] = '\n';
	return Stream.write(s, str_, n);
}

void start_stream(){
	Stream.quanta = 0xff;
	Stream.read = &s__read__;
	Stream.write = &s__write__;
	Stream.peek = &s__peek__;
	Stream.seek = &s__seek__;
	Stream.trim = &s__trim__;

	Stream.read_byte = &read_byte;
	Stream.read_short = &read_short;
	Stream.read_int = &read_int;
	Stream.read_float = &read_float;
	Stream.read_str = &read_str;
	Stream.read_line = &read_line;

	Stream.write_byte = &write_byte;
	Stream.write_short = &write_short;
	Stream.write_int = &write_int;
	Stream.write_float = &write_float;
	Stream.write_str = &write_str;
	Stream.write_line = &write_line;

	Stream.read_str2 = &read_str2;
	Stream.write_str2 = &write_str2;
}

// optable
void init_optable(){
	known_table = (known_handler*)malloc(sizeof(known_handler)*0x100);
	unknown_table = (unknown_handler*)malloc(sizeof(known_handler)*0x100);
	memset(known_table, 0, 0x400);
	memset(unknown_table, 0, 0x400);

	// TODO: fill out the rest of the packets in here.
#define K(i,a) known_table[(i)] = (known_handler)&(a)
	K(1, confirmation_known);
	K(3, connection_setup_known);
	K(7, fire);
	K(8, advanced_fire);
	K(9, weaponchange);
	K(10, posupdaterun);
	K(11, posupdatewalk);
	K(12, rotupdate);
	K(13, posrotupdaterun);
	K(14, posrotupdatewalk);
	K(16, reload);
	//K(17, hit);
	K(19, killmsg);
	K(20, teamchange);
	//K(21, spawn_msg); <- client?
	//K(22, round_start);
	K(23, buy);
	K(24, drop);
	//K(25, pickup);
	//K(27, projectile);

	K(28, spray);
	K(32, specpos);
	K(39, respawnrequest);
	K(236, rcon_pw);
	K(0xf1, rcon);
	K(240, chatmessage);
	K(249, ping_ingame);
	K(252, joinroutine_known);
	K(253, leave);
#undef K

#define U(i,a) unknown_table[(i)] = (unknown_handler)&(a)
	U(1, confirmation_unknown);
	U(3, connection_setup_unknown);
	U(27, usgn_add);
	U(28, usgn_update);
	U(250, ping_serverlist);
	U(251, serverinfo_request);
	U(252, joinroutine_unknown);
#undef U
}

#undef INT
