#include "func.h"
void send_fd(int fdw,int fd)
{
	struct msghdr msg;
	bzero(&msg,sizeof(msg));
	struct iovec iov[2];
	char buf[10]="hello";
	char buf1[10]="world";
	iov[0].iov_base=buf;
	iov[0].iov_len=strlen(buf);
	iov[1].iov_base=buf1;
	iov[1].iov_len=strlen(buf1);
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	struct cmsghdr *cmsg;
	int len=CMSG_LEN(sizeof(int));
	cmsg=(struct cmsghdr *)calloc(1,len);
	cmsg->cmsg_len=len;
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	*(int*)CMSG_DATA(cmsg)=fd;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	int ret;
	ret=sendmsg(fdw,&msg,0);
	if(-1==ret)
	{
		perror("sendmsg");
		return;
	}
}
void recv_fd(int fdr,int* fd)
{
	struct msghdr msg;
	bzero(&msg,sizeof(msg));
	struct iovec iov[2];
	char buf[10]="hello";
	char buf1[10]="world";
	iov[0].iov_base=buf;
	iov[0].iov_len=strlen(buf);
	iov[1].iov_base=buf1;
	iov[1].iov_len=strlen(buf1);
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	struct cmsghdr *cmsg;
	int len=CMSG_LEN(sizeof(int));
	cmsg=(struct cmsghdr *)calloc(1,len);
	cmsg->cmsg_len=len;
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	int ret;
	ret=recvmsg(fdr,&msg,0);
	if(-1==ret)
	{
		perror("recvmsg");
		return;
	}
	*fd=*(int*)CMSG_DATA(cmsg);
}



int main()
{
	int fds[2];
	socketpair(AF_LOCAL,SOCK_STREAM,0,fds);//初始化socket管道
	if(!fork())
	{
		close(fds[1]);//关闭写端，接下来要进行读
		int fd;
		recv_fd(fds[0],&fd);
		printf("child fd=%d\n",fd);//子进程接收到的fd的值
		char buf[20]={0};
		read(fd,buf,sizeof(buf));
		printf("%s\n",buf);
		exit(0);
	}else{
		close(fds[0]);//关闭读端，接下来要进行写
		int fd;
		fd=open("file",O_RDWR);
		printf("parent fd=%d\n",fd);
		send_fd(fds[1],fd);
		wait(NULL);
	}
}
