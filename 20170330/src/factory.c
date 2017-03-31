#include "factory.h"

void factory_init(pfac pf, int tnum, int cap, pthfunc thfunc){
	que_init(&pf->que, cap);
	pthread_cond_init(&pf->cond, NULL);
	pf->pth = (pthread_t *)calloc(tnum, sizeof(pthread_t));
	pf->tnum = tnum;
	pf->startflag = 0;
	pf->threadfunc = thfunc;
}


void factory_start(pfac pf){
	int i;
	if(0 == pf->startflag){
		for(i = 0; i < pf->tnum; i++){
			pthread_create(pf->pth+i, NULL, pf->threadfunc, pf);
		}
		pf->startflag = 1;
	}else{
		printf("未开始\n");
	}
}
