#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef struct{
	pid_t pid;
	int fdw;
	short busy;
}cdata, *pcdata;


void make_child(pcdata, int);
void send_fd(int, int);
void recv_fd(int, int*);
void process_request(int);
void child_handle(int);
