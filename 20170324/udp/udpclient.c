#include "func.h"

/*
int socket(int domain, int type, int protocol);
int bind(int sockfd, const struct sockaddr *addr, socket_t addrlen);
int listen(int sockfd, int backlog);
int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
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
	char buf[100] = {0};
	ret = sendto(sfd, "wo love panda", 13, 0, (struct sockaddr*)&sa, sizeof(sa));
	if(-1 == ret){
		perror("sendto");
		return -1;
	}
	int len = sizeof(sa);
	ret = recvfrom(sfd, buf, sizeof(buf), 0, (struct sockaddr*)&sa, &len);
	if(-1 == ret){
		perror("recvfrom");
		return -1;
	}
	puts(buf);
	close(sfd);
	return 0;
}
