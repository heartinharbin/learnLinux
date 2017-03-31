#ifndef __WORK_QUE_
#define __WORK_QUE_

#include "head.h"

typedef struct tag_node{
	int new_fd;
	struct tag_node *pnext;
}node_t, *pnode;

typedef struct{
	pnode phead, ptail;
	int capacity;
	int size;
	pthread_mutex_t mutex;
}que_t, *pque;

void que_init(pque, int);
void que_set(pque, pnode);
void que_get(pque, pnode*);


#endif
