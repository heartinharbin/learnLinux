#include "func.h"

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("error args\n");
		return -1;
	}
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfd){
		perror("socket");
		return -1;
	}
	int ret;
	struct sockaddr_in sci;
	memset(&sci, 0, sizeof(sci));
	sci.sin_family = AF_INET;
	sci.sin_port = htons(atoi(argv[2]));
	sci.sin_addr.s_addr = inet_addr(argv[1]);
	ret = connect(sfd, (struct sockaddr*)&sci, sizeof(sci));
	if(-1 == ret){
		perror("connect");
		return -1;
	}
	printf("connect success\n");
	char buf[50] = {0};
	ret = recv(sfd, buf, sizeof(buf), 0);
	close(sfd);	

}
