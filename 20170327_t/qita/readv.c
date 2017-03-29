#include "func.h"

int main()
{
	int fd;
	fd=open("file1",O_RDWR);
	char buf[10]={0};
	char buf1[10]={0};
	struct iovec iov[2];
	iov[0].iov_base=buf;
	iov[0].iov_len=6;
	iov[1].iov_base=buf1;
	iov[1].iov_len=4;
	readv(fd,iov,2);
	puts(buf);
	puts(buf1);
	close(fd);
}
	
