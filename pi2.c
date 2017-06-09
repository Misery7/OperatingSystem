#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>

struct param{
	int n;
	int start;
};

struct result{
	double sum;
};

void *compute_thread(void *arg)
{
	int i=0;
	struct param *p=(struct param*)arg;
	double sum=0;
	double numer=1.0;
	double denom=2*(p->start)+1;
	int loc=p->start+1;
	struct result *res;
	for(i=0;i<100;i++){
		if(loc%2==0){
			numer=-1.0;
		}
		else{
			numer=1.0;
		}
		sum += numer/denom;
		denom += 2*(p->n);
		loc += p->n;
	}	
	res = (struct result*)malloc(sizeof(struct result));
	res -> sum = sum;
	return res;
}

int main()
{
	int n;
	int i=0;
	double sum=0;
	struct param p[100];
	pthread_t worker_tid[100];
	scanf("%d",&n);
	for(i=0;i<n;i++){
		p[i].n=n;
		p[i].start=i;
		pthread_create(&worker_tid[i], NULL, &compute_thread, &p[i]);
	}
	for(i=0;i<n;i++){
		struct result *res;
		pthread_join(worker_tid[i], (void**)&res);
		sum += res->sum;
		free(res);	
	}
	printf("%f\n",sum);
	return 0;
}
