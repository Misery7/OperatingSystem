#include<stdio.h>
#include<sys/types.h>
#include<pthread.h>
#include<stdlib.h>

#define N 5

int count=0;
int *finalres;
pthread_t subthread[N+1];
pthread_t mid;

void *add(void *args)
{
	int p=*(int*)args;
	if(p<N){
		printf("Thread %d Send %d To Thread %d\n",p,p,p+1);
	}
	else{
		printf("Thread %d Send %d To Thread 1\n",p,p);
	}
	if(p<N){
		p++;
		printf("Thread %d Add 1 To Param\n",p);
		pthread_create(&subthread[p-1],NULL,&add,&p);
		pthread_join(subthread[p-1],NULL);
	}
	else{
		*finalres=p;
		pthread_join(subthread[N-1],(void**)&finalres);
		printf("N = %d\n",*(int*)finalres);
	}
}

int main()
{
	int i=1;
	finalres=(int*)malloc(sizeof(int));
	pthread_create(&subthread[0],NULL,&add,&i);
	pthread_join(subthread[0],(void**)&finalres);
	return 0;
}
