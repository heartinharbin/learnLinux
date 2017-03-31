#include "func.h"

void send_fd(int fdw, int fd){
	struct msghdr msg;
	bzero(&msg, sizeof(msg));
//	msg = (struct msghdr *)calloc(1, sizeof(struct msghdr));
	char buf[10] = "hello";
	char buf1[10] = "world";
	struct iovec iovs[2];
	iovs[0].iov_base = buf;
	iovs[0].iov_len = strlen(buf);
	iovs[1].iov_base = buf1;
	iovs[1].iov_len = strlen(buf1);
	msg.msg_iov = iovs;
	msg.msg_iovlen = 2;
	struct cmsghdr *cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1, len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	*(int *)CMSG_DATA(cmsg) = fd;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;
	int ret = sendmsg(fdw, &msg, 0);
	if(-1 == ret){
		perror("sendmsg");
		return;
	}
	
}

void recv_fd(int fdw, int *fd){
	struct msghdr msg;
//	msg = (struct msghdr *)calloc(1, sizeof(struct msghdr));
	char buf[10] = "hello";
	char buf1[10] = "world";
	struct iovec iovs[2];
	iovs[0].iov_base = buf;
	iovs[0].iov_len = strlen(buf);
	iovs[1].iov_base = buf1;
	iovs[1].iov_len = strlen(buf1);
	msg.msg_iov = iovs;
	msg.msg_iovlen = 2;
	struct cmsghdr *cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1, len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	int ret = recvmsg(fdw, &msg, 0);
	if(-1 == ret){
		perror("sendmsg");
		return;
	}
	*fd = *(int *)CMSG_DATA(cmsg);
}
