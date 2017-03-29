#include "func.h"

int main()
{
	int fd;
	fd=open("file1",O_RDWR);
	char buf[10]="hello";
	char buf1[10]="world";
	struct iovec iov[2];
	iov[0].iov_base=buf;
	iov[0].iov_len=strlen(buf);
	iov[1].iov_base=buf1;
	iov[1].iov_len=strlen(buf1);
	writev(fd,iov,2);
	close(fd);
}
	
