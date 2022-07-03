#include <semaphore.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

#define MAX_THINKING_TIME 1
#define MAX_EATING_TIME 1

sem_t forks[NUM_PHILOSOPHERS+1];
sem_t allow; /*This semaphore prevents deadlock if all n philosophers
               were hungry at the same time, we let only n-1 of them
               attempt to pick up forks.*/

void* philosopher(void* p) {
    int p_id = *((int*)p);
    while(true) {
        printf("%d started thinking\n", p_id);
        sleep(rand()%MAX_EATING_TIME);

        printf("%d is hungry...\n", p_id);

        sem_wait(&allow);
        sem_wait(&forks[p_id]);
        sem_wait(&forks[(p_id+1)%NUM_PHILOSOPHERS]);
        
        printf("%d started eating...\n", p_id);
        sleep(rand()%MAX_EATING_TIME);
        printf("...%d finished eating\n", p_id);
        
        sem_post(&forks[p_id]);
        sem_post(&forks[(p_id+1)%NUM_PHILOSOPHERS]);
        sem_post(&allow);
    }
}


int main() {
    srand(time(NULL));

    for(int i = 0; i < NUM_PHILOSOPHERS+1; i++) {
        sem_init(&forks[i], 0, 1);
    }
    sem_init(&allow, 0, NUM_PHILOSOPHERS-1);

    pthread_t philosophers[NUM_PHILOSOPHERS];
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        int* arg = malloc(sizeof(int));
        *arg = i;
        if(pthread_create(&philosophers[i], NULL, philosopher, arg) != 0) {
            fprintf(stderr, "Error creating reader thread\n");
            return 1;
        }
    }

    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        if(pthread_join(philosophers[i], NULL) != 0) {
            fprintf(stderr, "Error joining writer thread\n");
            return 1;
        }
    }

    for(int i = 0; i < NUM_PHILOSOPHERS+1; i++) {
        sem_destroy(&forks[i]);
    }
    sem_destroy(&allow);

    return 0;
}