#include "func.h"
//tcp通信的服务器端,实现即时聊天,可以重复连接,使用epoll多路复用
//边沿触发模式
void change_nonblock(int fd)
{
	int status,ret;
	status=fcntl(fd,F_GETFL);
	status=status|O_NONBLOCK;
	ret=fcntl(fd,F_SETFL,status);
	if(-1==ret)
	{
		perror("fcntl");
		return;
	}
}
					

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
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(ret==-1)
	{
		perror("bind");
		return -1;
	}
	ret=listen(sfd,10);
	if(-1==ret)
	{
		perror("listen");
		return -1;
	}
	int new_fd=-1;//new_fd才是负责和客户端进行通信的描述符
	struct sockaddr_in cli;
	char buf[5]={0};
	int epfd=epoll_create(1);
	struct epoll_event event,evs[3];
	bzero(&event,sizeof(event));
	event.events=EPOLLIN;//读事件
	event.data.fd=0;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,0,&event);
	if(-1==ret)
	{
		perror("epoll_ctl");
		return -1;
	}	
	bzero(&event,sizeof(event));
	event.events=EPOLLIN;//读事件
	event.data.fd=sfd;//sfd是否可读
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);//注册sfd
	if(-1==ret)
	{
		perror("epoll_ctl");
		return -1;
	}	
	int len;
	int i;
	while(1)
	{
		bzero(evs,sizeof(evs));
		ret=epoll_wait(epfd,evs,3,-1);
		for(i=0;i<ret;i++)	
		{
			if(sfd==evs[i].data.fd)
			{
				memset(&cli,0,sizeof(cli));
				len=sizeof(cli);
				new_fd=accept(sfd,(struct sockaddr*)&cli,&len);//accept会阻塞
				if(-1==new_fd)
				{
					perror("accept");
					return -1;
				}
				event.events=EPOLLIN|EPOLLET;//监控读事件，使用边沿触发模式
				change_nonblock(new_fd);//非阻塞
				event.data.fd=new_fd;
				epoll_ctl(epfd,EPOLL_CTL_ADD,new_fd,&event);//注册
				printf("accept succss,cli ip=%s,port=%d\n",inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
			}
			if(evs[i].events== EPOLLIN && new_fd==evs[i].data.fd)
			{
				while(1)
				{
					bzero(buf,sizeof(buf));			
					ret=recv(new_fd,buf,sizeof(buf),0);//new_fd和客户端进行通信
					if(ret >0)
					{
						printf("%s",buf);
					}else if(ret==0)//读到0,就是对端断开
					{
						close(new_fd);
						event.events=EPOLLIN;
						event.data.fd=new_fd;
						epoll_ctl(epfd,EPOLL_CTL_DEL,new_fd,&event);//解除注册
						printf("byebye\n");
						break;
					}else if(-1==ret && errno==EAGAIN){
						break;
					}
				}
				printf("\n");
			}
			if(evs[i].events== EPOLLIN && 0==evs[i].data.fd)
			{
				bzero(buf,sizeof(buf));
				ret=read(STDIN_FILENO,buf,sizeof(buf));
				if(ret>0)
				{
					ret=send(new_fd,buf,strlen(buf),0);
					if(-1==ret)
					{
						perror("send");
						return -1;
					}
				}else{
					printf("byebye\n");
					close(new_fd);
					event.events=EPOLLIN;
					event.data.fd=new_fd;
					epoll_ctl(epfd,EPOLL_CTL_DEL,new_fd,&event);//解除注册
				}
			}
		}
	}
	close(new_fd);
	close(sfd);
	return 0;
}	
	
	
