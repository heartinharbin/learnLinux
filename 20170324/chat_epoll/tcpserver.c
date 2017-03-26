#include "func.h"

/*
int socket(int domain, int type, int protocol);
int bind(int sockfd, const struct sockaddr *addr, socket_t addrlen);
int listen(int sockfd, int backlog);
int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
*/

void change_nonblock(int fd){
	int status, ret;
	status = fcntl(fd, F_GETFL);
	status = status | O_NONBLOCK;
	ret = fcntl(fd, F_SETFL, status);
	if(-1 == ret){
		perror("fcntl");
		return;
	}
}

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
	int addrlen;
	int epfd = epoll_create(1);
	struct epoll_event event, evs[3];
	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = 0;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event);
	if(-1 == ret){
		perror("epoll_ctl");
		return -1;
	}
	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = sfd;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event);
	if(-1 == ret){
		perror("epoll_ctl");
		return -1;
	}
	char buf[5] = {0};
	int ret2;
	int i;
	while(1){
		memset(&event, 0, sizeof(event));
		ret = epoll_wait(epfd, evs, 3, -1);
		for(i = 0; i < ret; i++){
			if(evs[i].data.fd == sfd){
				addrlen = sizeof(clientaddr);
				newfd = accept(sfd, (struct sockaddr*)&clientaddr, &addrlen);
				if(-1 == newfd){
					perror("accept");
					return -1;
				}
				event.events = EPOLLIN||EPOLLET;
				change_nonblock(newfd);//非阻塞
				event.data.fd = newfd;
				ret = epoll_ctl(epfd, EPOLL_CTL_ADD, newfd, &event);
				if(-1 == ret){
					perror("epoll_ctl");
					return -1;
				}
				printf("accept succss,cli ip=%s,port=%d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
			}
			if(evs[i].events == EPOLLIN && evs[i].data.fd == newfd){
				while(1){
					memset(buf, 0, sizeof(buf));
					ret = recv(newfd, buf, sizeof(buf), 0);
					if(ret > 0){
						printf("%s", buf);
					}else if(0 == ret){
						close(newfd);
						event.events=EPOLLIN;
						event.data.fd=newfd;
						epoll_ctl(epfd, EPOLL_CTL_DEL, newfd, &event); 
						printf("byebye\n");
						break;
					}else if(-1 == ret && errno == EAGAIN){
						break;
					}
				}
				printf("\n");
			}
			if(evs[i].events == EPOLLIN && evs[i].data.fd == 0){
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
					close(newfd);
					event.events=EPOLLIN;
					event.data.fd=newfd;
					epoll_ctl(epfd, EPOLL_CTL_DEL, newfd, &event);
					printf("byebye\n");
				}
			}
		}
	}
}
