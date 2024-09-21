#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

void produce_item(char *item) {
    //create a random number and put it into item
    sprintf(item, "item-%d\n", rand() % 100);
}

int main() {
    //open semaphores initialized by the starter
    sem_t *mutex = sem_open("/mutex", 0);
    sem_t *empty = sem_open("/empty", 0);
    sem_t *full = sem_open("/full", 0);

    //check for errorss
    if (mutex == SEM_FAILED || empty == SEM_FAILED || full == SEM_FAILED) {
        perror("Failed to open semaphores");
        return 1;
    }

    //create our file and our line buffer
    FILE *file;
    char item[20];

    while (1) {
        produce_item(item);

        //wait until the file is empty
        sem_wait(empty);

        //wait until file is available
        sem_wait(mutex);

        // Open the file to append the produced item
        file = fopen("buffer.txt", "a");
        if (file == NULL) {
            perror("Failed to open buffer");
            exit(1);
        }

        //write produced item
        fprintf(file, "%s", item);
        fclose(file);

        printf("Produced: %s", item);

        //indicate no longer in use
        sem_post(mutex);

        //indicate that the file is full
        sem_post(full);

        //make easier to read
        sleep(1);
    }
}
