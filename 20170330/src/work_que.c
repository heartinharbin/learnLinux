#include "work_que.h"

void que_init(pque p, int cap){
	p->capacity = cap;
	pthread_mutex_init(&p->mutex, NULL);
}


void que_set(pque p, pnode pnew){
	if(NULL == p->ptail){
		p->phead = pnew;
		p->ptail = pnew;
	}else{
		p->ptail->pnext = pnew;
		p->ptail = pnew;
	}
	p->size++;
	printf("set node success\n");
}


void que_get(pque p, pnode *pcur){
	*pcur = p->phead;
	p->phead = p->phead->pnext;
	if(NULL == p->phead){
		p->ptail = NULL;
	}
	p->size--;
	printf("get node success\n");
}
