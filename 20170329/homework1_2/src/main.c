#include "func.h"

int main(int argc, char* argv[]){
	if(argc != 4){
		printf("argv error\n");
		return -1;
	}
	pcdata p = (pcdata)calloc(atoi(argv[3]), sizeof(cdata));
	int num = atoi(argv[3]);
	make_child(p, num);
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfd){
		perror("socket");
		return -1;
	}
	struct sockaddr_in sc;
	memset(&sc, 0, sizeof(sc));
	sc.sin_family=AF_INET;
	sc.sin_port=htons(atoi(argv[2]));
	sc.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret = bind(sfd, (struct sockaddr*)&sc, sizeof(sc));
	if(-1 == ret){
		perror("bind");
		return -1;
	}
	ret = listen(sfd, num);
	if(-1 == ret){
		perror("listen");
		return -1;
	}
	int epfd = epoll_create(1);
	if(-1 == epfd){
		perror("epoll_create");
		return -1;
	}
	struct epoll_event event, *evs;
	evs = (struct epoll_event*)calloc(num+1, sizeof(event));
	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = sfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event);
	int i, j;	
	for(i = 0; i < num; i++){
		memset(&event, 0, sizeof(event));
		event.events = EPOLLIN;
		event.data.fd = p[i].fdw;
		epoll_ctl(epfd, EPOLL_CTL_ADD, event.data.fd, &event);
	}
	int new_fd;
	int flag;
	while(1){
		ret = epoll_wait(epfd, evs, num+1, -1);
		if(-1 == ret){
			perror("epoll_wait");
			return -1;
		}
		if(ret > 0){
			for(i = 0; i < ret; i++){
				if(evs[i].data.fd == sfd){
					new_fd = accept(sfd, NULL, NULL);		
					for(j = 0; j < num; j++){
						if(0 == p[j].busy){
							printf("pid:%d will be busy\n", p[j].pid);
							send_fd(p[j].fdw, new_fd);
							p[j].busy = 1;
							break;
						}
					}
				}
				close(new_fd);

				for(j = 0; j < num; j++){
					if(evs[i].data.fd == p[j].fdw){
						read(p[j].fdw, &flag, sizeof(flag));
						printf("pid:%d is not busy\n", p[j].pid);
						p[j].busy = 0;
//						close(fdw);
					}
				}
			}
		}
	}

}
