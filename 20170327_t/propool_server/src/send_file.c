#include "func.h"

void hand_request(int new_fd)
{
	printf("send file success\n");
	close(new_fd);
}
