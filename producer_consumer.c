#include <semaphore.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CONSUMERS 10
#define NUM_PRODUCERS 10
#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int counter = 0;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void* producer(void* p) {
    int item;
    int producer_id = *((int*)p);
    free(p);

    while(true) {
        item = rand() % 1000;

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[counter] = item;
        counter++;
        printf("Producer nr: %d added item: %d\n", producer_id, item);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        sleep(1);
    }
}

void* consumer(void* p) {
    int item;
    int consumer_id = *((int*)p);
    free(p);

    while(true) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        
        item = buffer[counter - 1];
        counter--;
        printf("Consumer nr: %d consumed item: %d\n", consumer_id, item);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main() {
    srand(time(NULL));

    pthread_t producer_thread[NUM_PRODUCERS];
    pthread_t consumer_thread[NUM_CONSUMERS];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);


    for(int i = 0; i < NUM_PRODUCERS; i++) {
        int* arg = malloc(sizeof(int));
        *arg = i; //pass producer id to the thread

        if(pthread_create(&producer_thread[i], NULL, producer, arg) != 0) {
            fprintf(stderr, "Error creating producer thread\n");
            return 1;
        }
    }

    for(int i = 0; i < NUM_CONSUMERS; i++) {
        int* arg = malloc(sizeof(int));
        *arg = i;

        if(pthread_create(&consumer_thread[i], NULL, consumer, arg) != 0) {
            fprintf(stderr, "Error creating consumer thread\n");
            return 1;
        }
    }

    for(int i = 0; i < NUM_PRODUCERS; i++) {
        if(pthread_join(producer_thread[i], NULL) != 0) {
            fprintf(stderr, "Error joining producer thread\n");
            return 1;
        }
    }

    for(int i = 0; i < NUM_CONSUMERS; i++) {
        if(pthread_join(consumer_thread[i], NULL) != 0) {
            fprintf(stderr, "Error joining consumer thread\n");
            return 1;
        }
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
}