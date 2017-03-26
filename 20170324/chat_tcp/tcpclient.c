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
	sfd = socket(AF_INET, SOCK_STREAM, 0);
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
	ret = connect(sfd, (struct sockaddr*)&sa, sizeof(sa));
	if(-1 == ret){
		perror("connect");
		return -1;
	}
	fd_set rdset;
	int ret2;
	while(1){	
		FD_ZERO(&rdset);
		FD_SET(0, &rdset);
		FD_SET(sfd, &rdset);
		int ret1 = select(sfd+1, &rdset, NULL, NULL, NULL);
		if(ret1 > 0){
			if(FD_ISSET(0, &rdset)){
				memset(buf, 0, sizeof(buf));
				ret2 = read(0 ,buf, sizeof(buf)-1);
				if(ret2 > 0){
					ret = send(sfd, buf, strlen(buf)-1, 0);
					if(-1 == ret){
						perror("send");
						return -1;
					}
				}else if(-1 == ret2){
					perror("read");
					return -1;
				}else{
					printf("byebye\n");
					break;
				}
			}
			if(FD_ISSET(sfd, &rdset)){
				memset(buf, 0, sizeof(buf));
			 	ret = recv(sfd, buf, sizeof(buf), 0);
				if(ret > 0){
					puts(buf);
				}else if(-1 == ret){
					perror("recv");	
					return -1;
				}else{
					printf("byebye\n");
					return -1;	
				}
			}
		}
	}
	close(sfd);
	return 0;
}
