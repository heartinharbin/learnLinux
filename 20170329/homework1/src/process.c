#include "func.h"

void process_request(int new_fd){
	printf("下载完毕\n");
	close(new_fd);
}
