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
	ret = bind(sfd, (struct sockaddr*)&sa, sizeof(sa));
	if(-1 == ret){
		perror("bind");
		return -1;
	}
	ret = listen(sfd, 10);
	if(-1 == ret){
		perror("listen");
		return -1;
	}
	int newfd;
	struct sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);
	newfd = accept(sfd, (struct sockaddr*)&clientaddr, &addrlen);
	if(-1 == newfd){
		perror("accept");
		return -1;
	}
	printf("accept success! ip=%s port=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	fd_set rdset;
	char buf[100] = {0};
	int ret2;
	while(1){
		FD_ZERO(&rdset);
		FD_SET(0, &rdset);
		FD_SET(newfd, &rdset);
		ret1 = select(newfd+1, &rdset, NULL, NULL, NULL);
		if(ret1 > 0){
			if(FD_ISSET(newfd, &rdset)){
				memset(buf, 0, sizeof(buf));
				ret = recv(newfd, buf, sizeof(buf), 0);
				if(ret > 0){
					puts(buf);
				}else if(-1 == ret){
					perror("recv");	
					return -1;
				}else{
					printf("byebye\n");
					break;
				}
			}
			if(FD_ISSET(0, &rdset)){
				memset(buf, 0, sizeof(buf));
				ret2 = read(0, buf, sizeof(buf)-1);
				if(ret2 > 0){
					ret = send(newfd, buf, strlen(buf)-1, 0);
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
		}
	}
}
