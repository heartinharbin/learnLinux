#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <sys/socket.h>
#include <strings.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef struct{
	pid_t pid;
	int fdw;//管道的写端
	short busy;//判断进程是否忙碌
}cdata,*pcdata;

void hand_request(int);
