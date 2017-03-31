#include "func.h"

void child_handle(int fdr){
	int new_fd;
	int flag = 1;
	while(1){
		recv_fd(fdr, &new_fd);
		process_request(new_fd);
		write(fdr, &flag, sizeof(flag));
//		close(fdr);
	}
}

void make_child(pcdata p, int num){
	int i;
	int sv[2];
	int pid;
	int ret;
	for(i = 0; i < num; i++){
		ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, sv);
		pid = fork();
		if(0 == pid){
			close(sv[1]);
			child_handle(sv[0]);
		}
		close(sv[0]);
		p[i].pid = pid;
		p[i].busy = 0;
		p[i].fdw = sv[1];
	}
}
