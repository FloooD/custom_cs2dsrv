/*
 * Published under GPLv3.
 * For more information take a look at the Readme
 * Copyright (c) by the authors of this file
 *
 * Author/s of this file: Jermuk
 */

#include "sendto_functions.h"

void SendToPlayer(unsigned char *message, int length, int id, int reliable){
	//printf("SENDQUEUE\t");
	//just(message, length);
	struct sockaddr_in tempclient;
	tempclient.sin_family = AF_INET;
	tempclient.sin_port = player[id].port;
	tempclient.sin_addr = player[id].ip;
	unsigned char *buffer = malloc(length + 2);
	if (buffer == NULL)
		error_exit("Memory error ( SendToPlayer() )");

	if (reliable == 1)
	{
		unsigned short *pNummer = &player[id].server_number;
		memcpy(buffer, pNummer, sizeof(unsigned short));
		player[id].server_number += 2;
	}
	else if (reliable == 0)
	{
		player[id].server_number--;
		unsigned short *pNummer = &player[id].server_number;
		memcpy(buffer, pNummer, sizeof(unsigned short));
		player[id].server_number++;
	}
	memcpy(buffer + 2, message, length);

//	sendstruct *payload = malloc(sizeof(sendstruct));
//	payload->addr = tempclient;
//	payload->length = length+2;
//	payload->msg = buffer;
//	push(&send_q, (void*)payload, mtime());

	send_now(buffer, length+2, tempclient);

	//udp_send(writesocket, buffer, length + 2, &tempclient);
}

void SendToPlayer_imm(unsigned char *message, int length, int id, int reliable, int sock)
{
	struct sockaddr_in tempclient;
	tempclient.sin_family = AF_INET;
	tempclient.sin_port = player[id].port;
	tempclient.sin_addr = player[id].ip;
	unsigned char *buffer = malloc(length + 2);
	if (buffer == NULL)
		error_exit("Memory error ( SendToPlayer() )");

	if (reliable == 1)
	{
		unsigned short *pNummer = &player[id].server_number;
		memcpy(buffer, pNummer, sizeof(unsigned short));
		player[id].server_number += 2;
	}
	else if (reliable == 0)
	{
		player[id].server_number--;
		unsigned short *pNummer = &player[id].server_number;
		memcpy(buffer, pNummer, sizeof(unsigned short));
		player[id].server_number++;
	}
	memcpy(buffer + 2, message, length);

//	sendstruct *payload = malloc(sizeof(sendstruct));
//	payload->addr = tempclient;
//	payload->length = length+2;
//	payload->msg = buffer;
//	push(&send_q, (void*)payload, mtime());

	//send_now(buffer, length+2, tempclient);

	udp_send(sock, buffer, length + 2, &tempclient);
}

int send_now(byte* msg, int length, struct sockaddr_in addr){
	sendstruct *payload = malloc(sizeof(sendstruct));
	payload->addr = addr;
	payload->length = length;
	payload->msg = msg;
	push(&send_q, (void*)payload, mtime());
	return 1;
}

// send after moffset milliseconds
int send_later(byte* msg, int length, struct sockaddr_in addr, int moffset){
	sendstruct *payload = malloc(sizeof(sendstruct));
	payload->addr = addr;
	payload->length = length;
	payload->msg = msg;
	push(&send_q, (void*)payload, mtime()+moffset);
	return 1;
}



void SendToAll(unsigned char *message, int length, int reliable)
{
	int i;
	for (i = 1; i <= sv_maxplayers; i++)
	{
		if (player[i].used == 1 && player[i].joinstatus >= 4)
		{
			SendToPlayer(message, length, i, reliable);
		}
	}
}

void SendToTeam(unsigned char *message, int length, int reliable, int team)
{
	int i;
	for (i = 1; i <= sv_maxplayers; i++)
	{
		if (player[i].used == 1 && player[i].joinstatus >= 4 && player[i].team
				== team)
		{
			SendToPlayer(message, length, i, reliable);
		}
	}
}

void SendToAllOther(int id, unsigned char *message, int length, int reliable)
{
	int i;
	for (i = 1; i <= sv_maxplayers; i++)
	{
		if (player[i].used == 1 && player[i].joinstatus >= 4 && i != id)
		{
			SendToPlayer(message, length, i, reliable);
		}
	}
}

// send_queue
int check_sendqueue(void){ // returns the number of elements sent
	int threshold = 0x30, i = 0, cur = mtime(); // 48 sends per iteration
	while (!empty(&send_q) && i++ < threshold){
		if (peek(&send_q)->cost > cur) return --i; // next element in the send queue isn't ready yet.
		struct {unsigned char* msg; int length; struct sockaddr_in addr;}* packet = pop(&send_q);
		udp_send(sock, packet->msg, packet->length, &(packet->addr));
		//free(packet->msg);
		free(packet);
	}
	return i; // started off at 0, so extra ++ is correct
}


// Priority Queue implementation

void init_queue(pq *q){
	if (!q) return perror("Cannot initialize null queues.");
	if (q->n) perror("The current queue is already linked.");
	q->n = 0;
}

// We will implement our PQ using a heap-like structure through a fixed array.
/*
 * Heap invariants:
 * parent -> (index-1)/2
 * child -> 2*index + 1 or 2
 * child.cost is always less than parent.cost
 * */
void push(pq *q, void* data, long cost){
	// Create a node to work with the data.
	pqnode* x = (pqnode*)malloc(sizeof(pqnode));
	x->data = data;
	x->cost = cost;

	// Traversal indices.
	int prev, next;

	if (q->n > 1000) return perror("The current queue is full, did not add.");
	q->q[(next = q->n++)] = *x;
	prev = (next-1)/2; // heap invariant -> prev node is at (n-1)/2

	while(next > 0 && q->q[next].cost < q->q[prev].cost){ // Heap invariant - child always less
		// Swap
		pqnode cur = q->q[next];
		q->q[next] = q->q[prev];
		q->q[prev] = cur;

		// Re-establish traversal indices
		next = prev; // child
		prev = (next-1)/2; // parent
	}

	// insert the new value
	q->q[next] = *x;
}

void* pop(pq *q){
	int heapsize, root, childpos;

	pqnode result = q->q[0];
	q->q[0] = q->q[--q->n]; // take last element and put on root of tree, then we swap it down until it's no longer less than child

	root = 0;
	if (q->n > 1){
		heapsize = q->n;
		pqnode value = q->q[root];
		while (root < heapsize){
			childpos = ((2*root+1));
			if(childpos < heapsize){
				if((2*(root+1)) < heapsize && q->q[childpos+1].cost < q->q[childpos].cost) childpos++;
				if(q->q[childpos].cost < q->q[root].cost){
					pqnode temp = q->q[root];
					q->q[root] = q->q[childpos];
					q->q[childpos] = temp;
					root = childpos;
				}else{
					q->q[root] = value;
					break;
				}
			}else{
				q->q[root] = value;
				break;
			}
		}
	}
	return result.data;
}

int contains(pq *q, void *data){
	int i;
	for(i=0; i<(*q).n; i++)
		if(q->q[i].data == data) return 1;

	return 0;
}

int remove_data(pq *q, void *data){
	int i;
	for(i=0; i<(*q).n; i++)
		if(q->q[i].data == data){q->q[i].data = 0; return 1;}
	return 0;
}

void* find_cost(pq* q, long cost){
	int root = 0;
	while (root <= 1000){
		pqnode n = q->q[root];
		if (n.cost == cost) return n.data;
		else if (n.cost > cost) return 0;
		// Branch below
		if (q->q[2*root+1].cost <= cost) root = 2*root + 1; // branch left
		else if (q->q[2*(root+1)].cost <= cost) root = 2*(root+1); // branch right
		else return 0;
	}
	return 0;
}

int remove_cost_data(pq* q, void* data, long cost){
	int root = 0;
	while (root <= 1000){
		pqnode n = q->q[root];
		if (n.cost == cost && data == n.data){n.data = 0;return 1;}
		else if (n.cost > cost) return 0;
		// Branch below
		if (q->q[2*root+1].cost <= cost) root = 2*root + 1; // branch left
		else if (q->q[2*(root+1)].cost <= cost) root = 2*(root+1); // branch right
		else return 0;
	}
	return 0;
}

pqnode* peek(pq* q){
	return &(q->q[0]);
}

int empty(pq *q){
	return !q->n;
}


