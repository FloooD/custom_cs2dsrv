/*
 * Published under GPLv3.
 * For more information take a look at the Readme
 * Copyright (c) by the authors of this file
 *
 * Author/s of this file: Jermuk
 */

#include "respond_messages.h"

#define SHORT(i) (((byte*)&(i))[0]), (((byte*)&(i))[1])
#define INT(i) (((byte*)&(i))[0]), (((byte*)&(i))[1]), (((byte*)&(i))[2]), (((byte*)&(i))[3])
#define FLOAT(i) INT(i)

void SendSpawnMessage(int id, unsigned short x, unsigned short y){
	stream* buf = init_stream(NULL);
	byte buffer[] = {0x15,id,SHORT(x),SHORT(y),50,0,SHORT(player[id].money),STARTWEAPONS_COUNT};
	Stream.write(buf, buffer, 11);
	int i;
	for (i = 0; i < STARTWEAPONS_COUNT; i++){
		Stream.write_byte(buf, startweapons[i]);
		GivePlayerWeapon(id, startweapons[i]);
		if (startweapons[i] == 50) player[id].actualweapon = i; //else 0
	}

	SendToAll(buf->mem, buf->size, 1);
	free(buf);
}

void SendAdvancedFireMessage(int id, int status){
	byte buffer[] = {8,id,status};
	SendToAll(buffer, 3, 1);
}

void SendFireMessage(int id){
	byte buffer[] = {7,id};
	SendToAll(buffer, 2, 1);
}

void SendBuyMessage(int id, int wpnid){
	byte buffer[] = {23,id,wpnid, SHORT(player[id].money), 0};
	SendToPlayer(buffer, 6, id, 1);

	if (weapons[wpnid].slot > 0)
		SendWeaponChangeMessage(id, wpnid);
}

void SendHitMessage(int id, int victim, int health, int armor){
	byte buffer[] = {17,id,victim,health,armor};
	SendToAll(buffer, 5, 1);
}

void SendWeaponChangeMessage(int id, int wpnid){
	byte buffer[] = {9, id, wpnid, 0};
	SendToAll(buffer, 4, 1);
}

void SendPosUpdate(int id, unsigned short x, unsigned short y, int status){
	byte buffer[] = {status?10:11,id,SHORT(x),SHORT(y)};
	SendToAllOther(id, buffer, 6, 1);
}

void SendPosRotUpdate(int id, unsigned short x, unsigned short y, int status,float rotation){
	byte buffer[] = {status?13:14,id,SHORT(x),SHORT(y),FLOAT(rotation)};
	SendToAllOther(id, buffer, 10, 1);
}

void SendRotUpdate(int id, float rotation){
	byte buffer[] = {12,id,FLOAT(rotation)};
	SendToAllOther(id, buffer, 6, 1);
}

void SendMessageToPlayer(int id, char *message, int status){
	stream* buf = init_stream(NULL);
	byte buffer[] = {240,0,status};
	Stream.write(buf, buffer,3);
	Stream.write_str2(buf, (byte*)message);
	SendToPlayer(buf->mem, buf->size, id, 1);
	printf("To %s (%s): %s\n", player[id].name, status==1?"Chat":status==2?"Screen":"Console", message);

	free(buf);
}

void SendMessageToAll(char *message, int status){
	stream* buf = init_stream(NULL);
	byte buffer[] = {240,0,status};
	Stream.write(buf, buffer,3);
	Stream.write_str2(buf, (byte*)message);
	SendToAll(buf->mem, buf->size, 1);

	printf("To All (%s): %s\n", status==1?"Chat":status==2?"Screen":"Console", message);
	free(buf);
}

void SendJoinMessage(int id){
	stream* buf = init_stream(NULL);
	Stream.write_byte(buf, 248);
	Stream.write_byte(buf, id);
	Stream.write_str(buf, player[id].name);
	Stream.write_byte(buf, 0);
	SendToAll(buf->mem, buf->size, 1);
	free(buf);
}

void SendLeaveMessage(int id){
	if (player[id].joinstatus < 4) return;
	byte buffer[] = {253,id,0};
	SendToAll(buffer, 3, 1);
}

void SendChatMessage(int id, unsigned char *message, int team){
	stream* buf = init_stream(NULL);
	Stream.write_byte(buf, 240);
	Stream.write_byte(buf, id);
	Stream.write_byte(buf, team);
	Stream.write_str2(buf, message);
	void (*send)() = team==1?(void (*)())&SendToAll:(void (*)())&SendToTeam; // What black magic is this? Just don't try this on VC++ ie use _MSC_VER macro, but it'd break anyways so who gives a fuck.
	send(buf->mem, buf->size, 1, player[id].team);
	free(buf);
}

void SendTeamChangeMessage(int id, unsigned char team, unsigned char skin){
	byte buffer[] = {20,id,team,skin};
	SendToAll(buffer, 4, 1);
}

void PingAllPlayer(){
	int i;
	for (i = 1; i <= sv_maxplayers; i++)
		if (player[i].used == 1 && player[i].joinstatus >= 4)
			player[i].start = mtime();

	byte buffer[] = {249,0,32,191,0};
	SendToAll(buffer, 5, 0);
}

void SendReloadMessage(int id, int status){
	byte buffer[] = {16,id,status};
	SendToAll(buffer, 3, 1);
}

// 28 - id - xx - yy - c
void SendSprayMessage(char id, unsigned short x, unsigned short y, char c){
	//byte buffer[] = {28,0,id,SHORT(x),SHORT(y),c};
	//SendToAll(buffer, 8, 1); // <-- Make sure that the spray-image send packet is crafted first.
}

//FIXME complete SendKillMessage
void SendKillMessage(int id, int victim)
{
	int suicide = 0;
	if (id == 0) {id = victim; suicide = 1;}
	byte sHealth[4];
	byte sArmor[4]; //= player[id].health
	int sHealthLength = sprintf((char*)sHealth, "%i", (int) player[id].health);
	int sArmorLength = sprintf((char*)sArmor, "%i", (int) player[id].armor);
	int sNameLength = u_strlen(player[id].name);
	int length = 4 + sNameLength + sHealthLength + sArmorLength;

	stream* buf = init_stream(NULL);
	byte buffer[] = {19,victim,((suicide == 1) ? 0 : id),
					player[id].actualweapon,
					SHORT(*player[victim].x),SHORT(*player[victim].y),
					240,0,1,SHORT(length),'k',166}; // 166 acts like a string separator, rest of this line is like wtf
	Stream.write(buf, buffer, 15);
	if (suicide == 0) {
		Stream.write(buf, player[id].name, sNameLength);
		Stream.write_byte(buf, 166);
		Stream.write(buf, sHealth, sHealthLength);
		Stream.write_byte(buf, 166);
		Stream.write(buf, sArmor, sArmorLength);
		Stream.write_byte(buf, 166);
	}

	SendToPlayer(buf->mem, buf->size, victim, 1);
	free(buf->mem);
	free(buf);

	byte buffer2[] = {19,victim,id,player[id].actualweapon,SHORT(*player[victim].x),SHORT(*player[victim].y)};
	SendToAllOther(victim, buffer2, 8, 1);
}

void SendPingList(void){
	stream* buf = init_stream(NULL);
	Stream.write_byte(buf, 247);
	Stream.write_byte(buf, onlineplayer);
	int i;
	for (i = 1; i <= sv_maxplayers; i++)
		if (player[i].used == 1 && player[i].joinstatus >= 4){
			Stream.write_byte(buf, i);
			Stream.write_short(buf, player[i].latency);
		}
	SendToAll(buf->mem, buf->size, 0);
	free(buf);
}

void SendBuyFailedMessage(int id, int status){
	//TODO what does the 44,1,0 do?
	byte buffer[] = {23,id,status,44,1,0};
	/* Status ->
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
	SendToPlayer(buffer, 6, id, 1);
}

void SendDropMessage(int id, int wpnid, int ammo1, int ammo2){
	unsigned short tilex = (unsigned short) *player[id].x / 32;
	unsigned short tiley = (unsigned short) *player[id].y / 32;

	// Check that ammo1 and ammo2 aren't shorts since ammo can go over 0xff. Confirmed
	byte buffer[] = {24,id,wpnid,SHORT(ammo1),SHORT(ammo2),
					SHORT(tilex), SHORT(tiley),0,0,0,1,0,0,0,50};
	SendToAll(buffer, 19, 1);
}

void SendRconPwMessage(int id, const unsigned char* message, int len, unsigned char success)
{
	int stringsize = len + 1;
	unsigned char *buffer = malloc(stringsize * sizeof(char));
	if (buffer == NULL)
		error_exit("Memory error ( SendRconPwMessage() )\n");
	memcpy(buffer, (void*) message, len);
	buffer[stringsize] = success;
	SendToPlayer(buffer, stringsize, id, 1);
	free(buffer);
}

#undef SHORT
#undef INT
#undef FLOAT
