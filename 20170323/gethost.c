#include <stdio.h>
#include <netdb.h>
#include <string.h>
int main(int argc, char* argv[]){
	if(argc != 2){
		printf("error args\n");
		return -1;
	}
	struct hostent *p;
	p = gethostbyname(argv[1]);
	if(p == NULL){
		printf("%d", h_errno);
		return -1;
	}
	printf("h_name=%s\n", p->h_name);
	char **pptr;
	for(pptr=p->h_alasec; *pptr!=NULL; pptr++){
		printf("%s\n", *pptr);
	}
	printf("h_length=%d\n", p->h_length);
	char ip[16];
	for(pptr=p->h_addr_list; *pptr!=NULL; pptr++){
		memset(ip, 0, sizeof(ip));
		inet_
	}
}
