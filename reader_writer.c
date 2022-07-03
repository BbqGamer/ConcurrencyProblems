#include <semaphore.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 15
#define NUM_WRITERS 2

#define MAX_WAITING_TIME 10
#define MAX_READING_TIME 2
#define MAX_WRITING_TIME 8

sem_t sem_w;
sem_t sem_r;

int active_readers;

void* reader(void* p) {
    int reader_id = *((int*)p);
    while(true) {
        sleep(rand()%MAX_WAITING_TIME);

        sem_wait(&sem_r);
        if(active_readers == 0) {
            sem_wait(&sem_w);
        }
        active_readers += 1;
        sem_post(&sem_r);

        printf("reader %d started reading...\n", reader_id);
        sleep(rand()%MAX_READING_TIME);
        printf("... reader %d ended reading\n", reader_id);

        sem_wait(&sem_r);
        active_readers -= 1;
        if(active_readers == 0) {
            sem_post(&sem_w);
        }
        sem_post(&sem_r);
    }
}

void* writer(void* p) {
    int reader_id = *((int*)p);
    while(true) {
        sleep(rand()%MAX_WAITING_TIME);
        sem_wait(&sem_w);

        printf("\twriter %d started writing...\n", reader_id);
        sleep(rand()%MAX_WRITING_TIME);
        printf("\t... writer %d ended writing\n", reader_id);

        sem_post(&sem_w);
    }
}


int main() {
    srand(time(NULL));

    sem_init(&sem_w, 0, 1);
    sem_init(&sem_r, 0, 1);

    pthread_t readers[NUM_READERS];
    for(int i = 0; i < NUM_READERS; i++) {
        int* arg = malloc(sizeof(int));
        *arg = i; //PASS READER ID TO THREAD
        if(pthread_create(&readers[i], NULL, reader, arg) != 0) {
            fprintf(stderr, "Error creating reader thread\n");
            return 1;
        }
    }

    pthread_t writers[NUM_WRITERS];
    for(int i = 0; i < NUM_WRITERS; i++) {
        int* arg = malloc(sizeof(int));
        *arg = i;
        if(pthread_create(&writers[i], NULL, writer, arg) != 0) {
            fprintf(stderr, "Error creating writer thread\n");
            return 1;
        }
    }

    for(int i = 0; i < NUM_READERS; i++) {
        if(pthread_join(readers[i], NULL) != 0) {
            fprintf(stderr, "Error joining reader thread\n");
            return 1;
        }
    }

    for(int i = 0; i < NUM_WRITERS; i++) {
        if(pthread_join(writers[i], NULL) != 0) {
            fprintf(stderr, "Error joining writer thread\n");
            return 1;
        }
    }

    sem_destroy(&sem_w);
    sem_destroy(&sem_r);

    return 0;
}