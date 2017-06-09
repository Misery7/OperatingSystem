#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

double sum2 = 0;

void* assistThread(void *args)
{
	
	int n = *(int*)args;
	int i = 0;
	double den = 0;
	for(i=0; i<n; i++){
		den = 4*i+3;
		sum2 += 1.0/den;
	}
	return NULL;
}

int main()
{
	int n = 20000;
	int i = 0;
	double sum1 = 0, sum;
	double den = 0;

	pthread_t work_tid;
	pthread_create(&work_tid, NULL, &assistThread, &n);

	for(i=0;i<n;i++){
		den = 4*i+1;
		sum1 += 1.0/den;
	}

	pthread_join(work_tid, NULL);
	sum = sum1 - sum2;
	printf("result : %f\n", sum);
	return 0;
}
