#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>

#define CAPACITY 4
#define ITEM_COUNT 8

char buffer1[4];
char buffer2[4];
int in1,out1;
int in2,out2;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_cond_t wait_empty_buffer1;
pthread_cond_t wait_full_buffer1;
pthread_cond_t wait_empty_buffer2;
pthread_cond_t wait_full_buffer2;

int buffer1_is_empty()
{
	return in1==out1;
}

int buffer1_is_full()
{
	return (in1+1)%CAPACITY==out1;
}

int buffer2_is_empty()
{
	return in2==out2;
}

int buffer2_is_full()
{
	return (in2+1)%CAPACITY==out2;
}

char get_item1()
{
	char item;
	item=buffer1[out1];
	out1=(out1+1)%CAPACITY;
	return item;
}

void put_item1(char item)
{
	buffer1[in1]=item;
	in1=(in1+1)%CAPACITY;	
}

char get_item2()
{
	char item;
	item=buffer2[out2];
	out2=(out2+1)%CAPACITY;
	return item;
}

void put_item2(char item)
{
	buffer2[in2]=item;
	in2=(in2+1)%CAPACITY;	
}

void produce()
{
	int i;
	char item;
	for(i=0;i<ITEM_COUNT;i++){
		pthread_mutex_lock(&mutex1);
		while(buffer1_is_full()){
			pthread_cond_wait(&wait_empty_buffer1, &mutex1);
		}
		
		item=i+'a';
		printf("Produce Item : %c\n",item);
		put_item1(item);
		
		pthread_cond_signal(&wait_full_buffer1);
		pthread_mutex_unlock(&mutex1);
	}
}

void *compute(void *args)
{
	int i;
	char item;
	for(i=0;i<ITEM_COUNT;i++){
		pthread_mutex_lock(&mutex1);
		while(buffer1_is_empty()){
			pthread_cond_wait(&wait_full_buffer1,&mutex1);
		}
		
		item=get_item1();
		printf("Calculate Item : %c To ",item);
		item-=32;
		printf("%c\n",item);
		
		pthread_cond_signal(&wait_empty_buffer1);
		pthread_mutex_unlock(&mutex1);
		
		pthread_mutex_lock(&mutex2);
		while(buffer2_is_full()){
			pthread_cond_wait(&wait_empty_buffer2,&mutex2);
		}
		
		put_item2(item);
		
		pthread_cond_signal(&wait_full_buffer2);
		pthread_mutex_unlock(&mutex2);
	}
	return NULL;
}

void *consume(void *args)
{
	int i;
	char item;
	for(i=0;i<ITEM_COUNT;i++){
		pthread_mutex_lock(&mutex2);
		while(buffer2_is_empty()){
			pthread_cond_wait(&wait_full_buffer2,&mutex2);
		}

		item=get_item2();
		printf("Consume Item : %c\n",item);
		
		pthread_cond_signal(&wait_empty_buffer2);
		pthread_mutex_unlock(&mutex2);
	}
	return NULL;
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
	pthread_join(cons_tid,NULL);
	return 0;
}
