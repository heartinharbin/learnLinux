#include "func.h"

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	int sfd;//socket描述符
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));//将端口号转换为网络字节序
	ser.sin_addr.s_addr=inet_addr(argv[1]);//将点分十进制IP地址转换为网络字节序
	int ret;
	ret=connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}
	printf("connect succss\n");
	fd_set rdset;
	char buf[10]={0};
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(STDIN_FILENO,&rdset);//要监控标准输入
		FD_SET(sfd,&rdset);//监控new_fd是否可读
		ret=select(sfd+1,&rdset,NULL,NULL,NULL);
		if(ret>0)
		{
			if(FD_ISSET(0,&rdset))
			{
				bzero(buf,sizeof(buf));			
				ret=read(0,buf,sizeof(buf));
				if(ret>0)
				{
					ret=send(sfd,buf,strlen(buf)-1,0);
					if(-1==ret)
					{
						perror("send");
						return -1;
					}
				}else{
					printf("byebye\n");
					break;
				}
			}
			if(FD_ISSET(sfd,&rdset))
			{
				bzero(buf,sizeof(buf));
				ret=recv(sfd,buf,sizeof(buf),0);
				if(ret>0)
				{
					printf("%s\n",buf);
				}else if(0==ret)
				{
					printf("byebye\n");
					break;
				}else{
					perror("recv");
					return -1;
				}
			}
		}
	}
	close(sfd);
	return 0;
}
