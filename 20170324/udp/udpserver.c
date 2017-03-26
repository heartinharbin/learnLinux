#include "func.h"

/*
int socket(int domain, int type, int protocol);
int bind(int sockfd, const struct sockaddr *addr, socket_t addrlen);
int listen(int sockfd, int backlog);
int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
*/

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("error argv\n");
		return -1;
	}
	int sfd;
	int ret, ret1;
	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == sfd){
		perror("socket");
		return -1;
	}	
	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(atoi(argv[2]));
	sa.sin_addr.s_addr = inet_addr(argv[1]);
	ret = bind(sfd, (struct sockaddr*)&sa, sizeof(sa));
	if(-1 == ret){
		perror("bind");
		return -1;
	}
	char buf[50] = {0};
	int newfd;
	struct sockaddr_in clientaddr;
	memset(&clientaddr, 0, sizeof(clientaddr));
	int addrlen = sizeof(clientaddr);
	ret = recvfrom(sfd, buf, sizeof(buf), 0, (struct sockaddr*)&clientaddr, &addrlen);
	if(-1 == ret){
		perror("recvfrom");
		return -1;
	}
	puts(buf);
	ret = sendto(sfd, "i love lucy", 11, 0, (struct sockaddr*)&clientaddr, addrlen);
	if(-1 == ret){
		perror("sendto");
		return -1;
	}
	close(sfd); 		
}
