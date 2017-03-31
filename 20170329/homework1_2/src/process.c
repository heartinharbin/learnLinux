#include "func.h"

void process_request(int new_fd){
//	printf("下载完毕");
 	train t;
	memset(&t, 0, sizeof(t));
	
	t.len = strlen(FILENAME);
	strcpy(t.buf, FILENAME);
	send(new_fd, &t, 4+t.len, 0);
	
	int fd;
	fd = open(FILENAME, O_RDWR);
	if(-1 == fd){
		perror("open");
		close(new_fd);
		return;
	}
	int ret;
	while((t.len = read(fd, t.buf, sizeof(t.buf)))>0){
		ret = send_n(new_fd, &t, 4+t.len);
		if(-1 == ret){
//			printf("client close\n");
			goto end;		
		}
	}	
	t.len = 0;
	send(new_fd, &t.len, 4, 0);
end:
	close(fd); //别忘记了关闭文件描述符
	close(new_fd);
}
