/**
 *
 * This is a POSIX solution using unnamed semaphores.
 * 
 * This solution will not work on OS X systems, 
 * but will work with Linux.
 */

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1

buffer_item buffer[BUFFER_SIZE];

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int insertPointer = 0;

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item)
{
	/* Buffer not full */
	if(insertPointer  < BUFFER_SIZE) {
		buffer[insertPointer] = item;
		insertPointer++;
		return 0;
	} else { /* Buffer full */
		return 1;
	}
}

int remove_item(buffer_item *item)
{
	/* Buffer not empty */
	if(insertPointer  > 0) {
		*item = buffer[(insertPointer - 1)];
		insertPointer--;
		return 0;
	} else { /* Buffer empty */
		return 1;
	}
	
}


int main(int argc, char *argv[])
{
	int sleepTime, producerThreads, consumerThreads;
	int i, j;

	if(argc != 4)
	{
		fprintf(stderr, "Useage: <sleep time> <producer threads> <consumer threads>\n");
		return -1;
	}

	sleepTime = atoi(argv[1]);
	producerThreads = atoi(argv[2]);
	consumerThreads = atoi(argv[3]);

	/* Initialize the synchronization tools */
	printf("%d\n",pthread_mutex_init(&mutex, NULL));
	printf("%d\n",sem_init(&empty, 0, 5));
	printf("%d\n",sem_init(&full, 0, 0));
	srand(time(0));

	/* Create the producer and consumer threads */
	for(i = 0; i < producerThreads; i++)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, producer, NULL);
	}

	for(j = 0; j < consumerThreads; j++)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, consumer, NULL);
	}

	/* Sleep for user specified time */
	sleep(sleepTime);
	return 0;
}

void *producer(void *param)
{
	buffer_item random;
	int r;

	while(TRUE)
	{
		r = rand() % 5;
		sleep(r);
		random = rand();

		/* Aquire empty s */
		sem_wait(&empty);

		/* Aquire lock */
		pthread_mutex_lock(&mutex);


		if(insert_item(random))
			fprintf(stderr, "Error");
		else {
			printf("Producer produced %d \n", random);
		}

		/* Release lock */
		pthread_mutex_unlock(&mutex);

		/* Signal full s*/
		sem_post(&full);


	}

}

void *consumer(void *param)
{
	buffer_item random;
	int r;

	while(TRUE)
	{
		r = rand() % 5;
		sleep(r);

		/* Aquire full s */
		sem_wait(&full);

		/* Aquire lock */
		pthread_mutex_lock(&mutex);

		
		if(remove_item(&random))
			fprintf(stderr, "Error Consuming");
		else {
			printf("Consumer consumed %d \n", random);
		}

		/* Release lock */
		pthread_mutex_unlock(&mutex);

		/* Signal empty s*/
		sem_post(&empty);
	}
}
