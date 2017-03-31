#include "factory.h"

void* entry(void *pf){
	pfac p = (pfac)pf;
	pque q = &p->que;
	pnode pcur;
	while(1){
		pthread_mutex_lock(&q->mutex);
		if(0 == q->size){
			pthread_cond_wait(&p->cond, &q->mutex);
		}
		que_get(q, &pcur);
		pthread_mutex_unlock(&q->mutex);
		process_request(pcur->new_fd);
		free(pcur);
		pcur = NULL;
	}
}

int main(int argc, char* argv[]){
	if(argc != 5){
		printf("error argv\n");
		return -1;
	}
	int tnum = atoi(argv[3]);
	int capacity = atoi(argv[4]);
	factory f;
	memset(&f, 0, sizeof(f));
	factory_init(&f, tnum, capacity, entry);//栈空间比堆空间好，就不calloc了
	factory_start(&f);

	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfd){
		perror("socket");
		return -1;
	}
	int ret;
	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(atoi(argv[2]));
	sa.sin_addr.s_addr = inet_addr(argv[1]);
	ret = bind(sfd, (struct sockaddr*)&sa, sizeof(sa));
	if(-1 == ret){
		perror("bind");
		close(sfd);
		return -1;
	}
	ret = listen(sfd, capacity);
	if(-1 == ret){
		perror("listen");
		close(sfd);
		return -1;
	}
	int new_fd;
//	pque pq = &f.que;
	while(1){
		new_fd = accept(sfd, NULL, NULL);
		if(-1 == new_fd){
			perror("accept");
			close(sfd);
			return -1;
		}
		pnode pnew = (pnode)calloc(1, sizeof(node_t));
		pnew->new_fd = new_fd;
		pthread_mutex_lock(&f.que.mutex);
		que_set(&f.que, pnew);
		pthread_mutex_unlock(&f.que.mutex);
		pthread_cond_signal(&f.cond);
	}
	return 0;
}
