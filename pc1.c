#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>

char buffer1[4];
char buffer2[4];
int in1,out1;
int in2,out2;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_cond_t wait_empty_buffer1;
pthread_cond_t wait_empty_buffer2;

int buffer1_is_empty()
{
	return in1==out1;
}

int buffer1_is_full()
{
	return (in1+1)%4==out1;
}

int buffer2_is_empty()
{
	return in2==out2;
}

int buffer2_is_full()
{
	return (in2+1)%4==out2;
}

char get_item1()
{

}

char put_item1()
{

}

char get_item2()
{

}

char put_item2()
{

}

void *consume(void *args)
{
	
}

void *produce(void *args)
{

}

void *compute(void *args)
{

}

int main()
{
	pthread_t cons_tid,comp_tid,prod_tid;
	pthread_mutex_init(&mutex1,NULL);
	pthread_mutex_init(&mutex2,NULL);
	pthread_cond_init(&wait_empty_buffer1,NULL);
	pthread_cond_init(&wait_full_buffer1,NULL);
	pthread_cond_init(&wait_empty_buffer2,NULL);
	pthread_cond_init(&wait_full_buffer2,NULL);

	pthread_create(&comp_tid,NULL,&compute,NULL);
	pthread_create(&cons_tid,NULL,&consume,NULL);
	produce();
	return 0;
}
