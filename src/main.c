/*
 * Published under GPLv3.
 * For more information take a look at the Readme
 * Copyright (c) by the authors of this file
 *
 * Author/s of this file: Jermuk, FloooD
 */

#include <main.h>
#include <getopt.h>
#include "settings.h"

void just(byte* str, int l){
	printf(""); int i;
	for (i=0;i<l;i++)eprintf("%x ", str[i]);
	eprintf("\n");
}

void parse_opts(int argc, char *argv[]){
	static struct option long_options[] = {
		{"cfg", required_argument, 0, 'c'},
		{"name", required_argument, 0, 0},
		{"nousgn", no_argument, 0, 1},
		{"lua", required_argument, 0, 'l'},
		{"debug", no_argument, 0, 'd'},
		{"strict", no_argument, 0, 's'},
		{0, 0, 0, 0}
	};

	while (1){
		int option_index = 0;
		int c = getopt_long (argc, argv, "c:", long_options, &option_index);
		if (c<0) return;
		switch (c){
		case 'c':
			cfg_file = optarg;
			break;
		case 'l':
			lua_file = optarg;
			break;
		case 'd':
			debug = 1;
			break;
		case 's':
			lua_strict = 1;
			break;
		case 0:
			sv_name = (unsigned char*)optarg;
			break;
		case 1:
			no_usgn = 1;
			break;
		default:
			return;
		}
	}
}

/**
 * \fn int main()
 * \brief initialize sockets and if a message was received, give it to the right function.
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int sock; //socket for everyyyyythinggggg
int main(int argc, char *argv[]){
	init_parse();
	// Parses the commandline arguments
	parse_opts(argc, argv); // IE: ./server -cserver.cfg --name "My Server"

	/**
	 * Initalize variables, weapons, players and sockets
	 */

	struct sockaddr_in newclient;
	unsigned char buffer[MAX_BUF];
	int size;
	fd_set descriptor; //I don't know

	ClearAllPlayer();
	WeaponInit();
	ReadServerCfg(cfg_file ? :"server.cfg"); // Reads the server.cfg file (We can also check argv for --cfg or -c flag

	sock = create_socket();
	bind_socket(&sock, INADDR_ANY, sv_hostport);
	atexit(&cleanup);
	signal(SIGABRT, &exit);
	signal(SIGTERM, &exit);
	signal(SIGINT, &exit);

	init_lua();

	OnServerStart();

	// moved since usgnregister requires a send queue
	init_queue(&send_q);
	ReadMap();
	if (!no_usgn)
	  UsgnRegister();

	init_optable();
	start_stream();

#ifdef _WIN32
//hahahahaha windows lol
#else
	const int inc = NS_PER_S / sv_fps;
	int frame = 0;
	int previous = 0;

	struct timespec current, next;
	clock_gettime(CLOCK_MONOTONIC, &next);
#endif

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	while (1)
	{
#ifdef _WIN32
#else
		frame++; //eh.. better to keep these out of script-funcs
		next.tv_nsec += inc;
		while (next.tv_nsec > NS_PER_S) { //happens exactly once a second
			next.tv_nsec -= NS_PER_S;
			next.tv_sec++;

			fpsnow = frame - previous;
			previous = frame;
			OnSecond();
		}
#endif
		OnFrame();

		FD_ZERO(&descriptor);
		FD_SET(sock, &descriptor);
		select(sock + 1, &descriptor, NULL, NULL, &timeout);

		if (FD_ISSET(sock, &descriptor)){
			size = udp_receive(sock, buffer, MAX_BUF, &newclient);

			if (size < 3) {
				perror("Invalid packet! (size < 3)\n");
			} else {
				stream *packet = init_stream(NULL);
				Stream.write(packet, buffer+2, size-2);

				// There's a chance that the guy left before all of the packet has been processed.
				while(1){
					int id = IsPlayerKnown(newclient.sin_addr, newclient.sin_port);
					if (id){
						if (ValidatePacket(buffer,id)){
							PacketConfirmation(buffer,id); //If the numbering is even, send a confirmation
							player[id].lastpacket = mtime();
							int pid = Stream.read_byte(packet);
							known_handler h = known_table[pid];
							if (!h){
								printf("Unhandled packet originating from %s (id:%d)\n", player[id].name, id);
								//stream *lolbuf = init_stream(NULL);
								//Stream.write(lolbuf, buffer, size);
								//unknown(lolbuf, pid);
								unknown(packet, pid);
							} else
								h(packet, id);
						}
					}else{
						int pid = Stream.read_byte(packet);
						unknown_handler h = unknown_table[pid];
						if (!h)
							unknown(packet, pid);
						else
							h(packet, &newclient);
					}

					if (EMPTY_STREAM(packet)){
						free(packet);
						break;
					}
				}
			}
		}

		check_sendqueue();

#ifdef _WIN32
		Sleep(1000 / sv_fps); //who cares about windows :D
#else
		clock_gettime(CLOCK_MONOTONIC, &current);
		if (((current.tv_sec == next.tv_sec) && (current.tv_nsec < next.tv_nsec)) || (current.tv_sec < next.tv_sec)) {
			clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
		} else {
			next.tv_nsec = current.tv_nsec + (current.tv_sec - next.tv_sec) * NS_PER_S;
		}
#endif
	}
	return EXIT_SUCCESS;
}
