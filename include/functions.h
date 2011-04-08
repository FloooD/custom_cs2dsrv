/*
 * Published under GPLv3.
 * For more information take a look at the Readme
 * Copyright (c) by the authors of this file
 *
 * Author/s of this file: Jermuk
 */

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "main.h"
int IsPlayerKnown(struct in_addr ip, u_short port);
void ClearPlayer(int id);
void ClearAllPlayer(void);
void CheckForTimeout(int writesocket);
int GivePlayerWeapon(int id, int wpnid);
void RemovePlayerWeapon(int id, int wpnid);
void RemoveAllPlayerWeapon(int id);
int PlayerTimeout(int id);
unsigned short GetServerStatus(void);
unsigned short endian_swap_short(unsigned short *x);
unsigned int endian_swap_int(unsigned int *x);
int ValidatePacket(unsigned char *message, int id);
void PacketConfirmation(unsigned char *message, int id);
int CheckPlayerData(unsigned char *password);
unsigned char *GetEncodedString(unsigned char *string, int length);
void CheckAllPlayerForReload(int writesocket);
struct in_addr GetIp(char *name);
int UsgnRegister(int writesocket);
int UsgnUpdate(int writesocket);
void ExecuteFunctionsWithTime(int writesocket);
size_t u_strlen(unsigned char* buffer);
int line_seg_sqr(float ex, float ey, float sqx, float sqy, float sqhlen, float *ox, float *oy);
void simulate_bullet(int id, unsigned char wpn, short dmg, float rot);

#endif // FUNCTIONS_H_INCLUDED
