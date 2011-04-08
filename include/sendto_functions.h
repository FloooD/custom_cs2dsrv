/*
 * Published under GPLv3.
 * For more information take a look at the Readme
 * Copyright (c) by the authors of this file
 *
 * Author/s of this file: Jermuk
 */

#ifndef SENDTO_FUNCTIONS_H_
#define SENDTO_FUNCTIONS_H_

#include "main.h"
void SendToPlayer(unsigned char *message, int length, int id, int reliable);
void SendToAll(unsigned char *message, int length, int reliable);
void SendToTeam(unsigned char *message, int length, int reliable, int team);
void SendToAllOther(int id, unsigned char *message, int length, int reliable);

int check_sendqueue(void);

// A priority queue for scheduling
typedef struct {
	void* data;
	long long cost;
} pqnode;

typedef struct {
	pqnode q[1000];
	int n;
} pq;

// Macro for queue initialization
#define INIT_QUEUE(q) ((q)->n = 0)
void init_queue(pq* q); // checks for null pointers and etc
void push(pq* q, void* data, long cost); // enqueue, lowest cost comes out first
void* pop(pq* q); // dequeue
int empty(pq* q);
int contains(pq*, void* data);
int remove_data(pq*, void* data);
void* find_cost(pq*, long cost);
int remove_data_cost(pq*, void*, long);
pqnode* peek(pq*);

pq send_q; // used for throttling (detection of flooding can be implemented via this as well)

int send_now(byte* msg, int length, struct sockaddr_in addr);
int send_later(byte* msg, int length, struct sockaddr_in addr, int moffset);

typedef struct sendstruct{
	unsigned char* msg; int length; struct sockaddr_in addr;
}sendstruct;

#endif /* SENDTO_FUNCTIONS_H_ */
