#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main(){
	sleep(5);
	char buf[50] = {0};
	int len = strlen(buf);
	buf[len-1] = '\0';
	int ret;
	int fd = 0;
	int status;
	status = fcntl(fd, F_GETFL);
	status = status|O_NONBLOCK;
	ret = fcntl(fd, F_SETFL, status);
	if(-1 == ret){
		perror("fcntl");
		return -1;
	}
	read(0, buf, sizeof(buf)-1);
	printf("%s\n", buf);
	return 0;
}
