#ifndef _FACTORY_H_
#define _FACTORY_H_

#include "work_que.h"

typedef void* (*pthfunc)(void *);

typedef struct{
	que_t que;
	int tnum;
	pthread_cond_t cond;
	pthfunc threadfunc;
	pthread_t *pth;
	short startflag;	
}factory, *pfac;

typedef struct{
	int len;
	char buf[1000];
}train, *ptrain;

#define FILENAME "onepunch"

void factory_init(pfac, int, int, pthfunc);
void factory_start(pfac);
int send_n(int, char*, int);
int recv_n(int, char*, int);
void process_request(int);

#endif
