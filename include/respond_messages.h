/*
 * Published under GPLv3.
 * For more information take a look at the Readme
 * Copyright (c) by the authors of this file
 *
 * Author/s of this file: Jermuk
 */

#ifndef RESPOND_MESSAGES_H_INCLUDED
#define RESPOND_MESSAGES_H_INCLUDED
#include "main.h"

void SendSpawnMessage(int id, unsigned short x, unsigned short y);
void SendAdvancedFireMessage(int id, int status);
void SendFireMessage(int id);
void SendBuyMessage(int id, int wpnid);
void SendHitMessage(int id, int victim, int health, int armor);
void SendWeaponChangeMessage(int id, int wpnid);
void SendPosUpdate(int id, unsigned short x, unsigned short y, int status);
void SendPosRotUpdate(int id, unsigned short x, unsigned short y, int status,
		float rotation);
void SendRotUpdate(int id, float rotation);
void SendMessageToPlayer(int id, char *message, int status);
void SendMessageToAll(char *message, int status);
void SendJoinMessage(int id);
void SendLeaveMessage(int id);
void SendChatMessage(int id, unsigned char *message, int team);
void SendTeamChangeMessage(int id, unsigned char team, unsigned char skin);
void PingAllPlayer(void);
void SendReloadMessage(int id, int status);
void SendSprayMessage(char id, unsigned short xx, unsigned short yy, char c);
void SendKillMessage(int id, int victim);
void SendPingList(void);
void SendBuyFailedMessage(int id, int status);
void SendDropMessage(int id, int wpnid, int ammo1, int ammo2);
void SendRconPwMessage(int id, const unsigned char* message, int len,
		unsigned char success);
#endif // RESPOND_MESSAGES_H_INCLUDED
