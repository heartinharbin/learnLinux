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
	int len;
	char buf[1000] = {0};
//	ret = recv(sfd, buf, sizeof(buf), 0);
	ret = recv(sfd, &len, 4, 0);
	printf("len = %d\n", len);
	ret = recv(sfd, buf, len, 0);//必须是读取的len值
	if(-1 == ret){
		perror("recv");
	}
	printf("ret = %d\n", ret);
	printf("buf = %s\n", buf);
	int fd = open(buf,O_RDWR|O_CREAT, 0600);
	if(-1 == fd){
		perror("open");
		close(sfd);
		return -1;
	}
	while(1){
		recv_n(sfd, &len, sizeof(len));
		if(len > 0){
			memset(buf, 0 , sizeof(buf));
			recv_n(sfd, buf, len);
			write(fd, buf, len);
		}else{
			break;
		}
	}
	close(fd);
	close(sfd);	
}
