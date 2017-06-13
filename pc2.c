#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<semaphore.h>

#define CAPACITY 4
#define ITEM_COUNT 8

char buffer1[CAPACITY];
char buffer2[CAPACITY];
int in1,out1;
int in2,out2;

sem_t mutex1;
sem_t mutex2;
sem_t buffer1_empty;
sem_t buffer1_full;
sem_t buffer2_empty;
sem_t buffer2_full;

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
		sem_wait(&buffer1_empty);
		sem_wait(&mutex1);

		item='a'+i;
		printf("Produce Item : %c\n",item);
		put_item1(item);
		
		sem_post(&mutex1);
		sem_post(&buffer1_full);
	}
}

void *compute(void *args)
{
	int i;
	char item;
	for(i=0;i<ITEM_COUNT;i++){
		sem_wait(&buffer1_full);
		sem_wait(&mutex1);

		item=get_item1();
		printf("Calculate Item : %c To ",item);
		item-=32;
		printf("%c\n",item);
		
		sem_post(&mutex1);
		sem_post(&buffer1_empty);

		sem_wait(&buffer2_empty);
		sem_wait(&mutex2);

		put_item2(item);

		sem_post(&mutex2);
		sem_post(&buffer2_full);
	}
	return NULL;
}

void *consume(void *args)
{
	int i;
	char item;
	for(i=0;i<ITEM_COUNT;i++){
		sem_wait(&buffer2_full);
		sem_wait(&mutex2);

		item=get_item2();
		printf("Consume Item : %c\n",item);

		sem_post(&mutex2);
		sem_post(&buffer2_empty);
	}
	return NULL;
}

int main()
{
	pthread_t cons_tid,comp_tid;
	sem_init(&mutex1,0,1);
	sem_init(&buffer1_empty,0,CAPACITY);
	sem_init(&buffer1_full,0,0);
	sem_init(&mutex2,0,1);
	sem_init(&buffer2_empty,0,CAPACITY);
	sem_init(&buffer2_full,0,0);

	pthread_create(&cons_tid,NULL,&consume,NULL);
	pthread_create(&comp_tid,NULL,&compute,NULL);
	produce();
	pthread_join(cons_tid,NULL);
	return 0;
}
