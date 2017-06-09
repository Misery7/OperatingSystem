#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>

int arr[]={3,8,1,7,5,0,9,6,2};
int arr2[10];

struct param{
	int *array;
	int start;
	int end;
};

struct res{
	int *array;
};

void *SelectSort(void *args)
{
	struct param* p=(struct param*)args;
	int minloc;
	int i=0,j=0;
	int tmp;
	for(i=0;i<p->end-p->start+1;i++){
		minloc=i;
		for(j=i+1;j<p->end-p->start+1;j++){
			if(*(p->array+minloc)>*(p->array+j)){
				minloc=j;
			}	
		}
		tmp=*(p->array+minloc);
		*(p->array+minloc)=*(p->array+i);
		*(p->array+i)=tmp;
	}
}

int main()
{
	pthread_t worker_thread[2];
	int n=sizeof(arr)/sizeof(int);
	struct param p[2];
	p[0].start=0;
	p[0].end=n/2;
	p[0].array=arr;
	pthread_create(&worker_thread[0], NULL, &SelectSort, &p[0]);
	p[1].start=n/2+1;
	p[1].end=n-1;
	p[1].array=arr+(n/2+1);
	pthread_create(&worker_thread[1], NULL, &SelectSort, &p[1]);
	pthread_join(worker_thread[0], NULL);
	pthread_join(worker_thread[1], NULL);
	int loc1=p[0].start;
	int loc2=p[1].start;
	int i=0;
	int k=0;
	while(loc1<=p[0].end && loc2<=p[1].end){
		if(arr[loc1]<arr[loc2]){
			arr2[k++]=arr[loc1];
			loc1++;
		}			
		else{
			arr2[k++]=arr[loc2];
			loc2++;
		}
	}
	if(loc1<=p[0].end){
		while(loc1<=p[0].end){
			arr2[k++]=arr[loc1++];
		}	
	}
	else{
		while(loc2<=p[1].end){
			arr2[k++]=arr[loc2++];
		}
	}
	for(i=0;i<k;i++){
		printf("%d ",arr2[i]);
	}
	printf("\n");
	return 0;
}
