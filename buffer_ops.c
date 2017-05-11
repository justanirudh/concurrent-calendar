#include<stdlib.h>
#include<stdio.h>
#include"declarations.h"
#include<pthread.h>

BufferManager initializeBuffer(int size) {
	BufferManager bm = malloc(sizeof(struct bufferManager));
	EventInfo buffer = malloc(size * sizeof(struct eventInfo));
	bm->in = 0;
	bm->out = 0;
	bm->numElems = 0;
	bm->size = size;	
	bm->buffer = buffer;
	int ret;

	pthread_mutex_t lock; 
	ret = pthread_mutex_init(&lock, NULL) ;
	if (ret) {
		printf("Mutex initialization failed:  %d", ret);
		exit(1);
	}
	bm->lock = lock;

	pthread_cond_t notFull;
	ret = pthread_cond_init( &notFull, NULL);
	if (ret) {
		printf("Condition variable initialization failed:  %d", ret);
		exit(1);
	}
	bm->notFull = notFull;

	pthread_cond_t notEmpty;
	ret = pthread_cond_init( &notEmpty, NULL);
	if (ret) {
		printf("Condition variable initialization failed:  %d", ret);
		exit(1);
	}
	bm->notEmpty = notEmpty;

	return bm;
}

void addToBuffer(BufferManager bm, EventInfo ei) {
	pthread_mutex_lock(&(bm->lock));	
	while(!((bm->numElems) < bm->size)){ //if numelems >= numsize, wait
		pthread_cond_wait(&(bm->notFull), &(bm->lock)); //waiting for it to be not full
	}
	(bm->buffer)[bm->in] = *ei;
	bm->in = ((bm->in) + 1)%(bm->size);
	(bm->numElems)++;
	pthread_cond_signal(&(bm->notEmpty));//signal consumer that buffer is not empty
	pthread_mutex_unlock(&(bm->lock));
}

EventInfo removeFromBuffer(BufferManager bm){
	pthread_mutex_lock(&(bm->lock));
	while(!((bm->numElems) > 0)){ //if numelems <= 0, wait
		pthread_cond_wait(&(bm->notEmpty), &(bm->lock)); //waiting for it to be not empty
	}
	EventInfo ei = &((bm->buffer)[bm->out]);
	bm->out = ((bm->out) + 1)%(bm->size);
	(bm->numElems)--;
	pthread_cond_signal(&(bm->notFull));//signal consumer that buffer is not full
	pthread_mutex_unlock(&(bm->lock));
	return ei;
}

void printBuffer(BufferManager bm) {
	EventInfo buf = bm->buffer;
	for(int i = 0; i < (bm->numElems); ++i){		
		struct eventInfo curr = buf[i];
		printf("%s|%s|%s|%s|%s\n", curr.action, curr.name, curr.date, curr.time, curr.location);
	}
}


