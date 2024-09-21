#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void consume_item(char *item) {
    // Simulate item consumption
    printf("Consumed: %s", item);
}

int main() {
    //open semaphores initialized by starter
    sem_t *mutex = sem_open("/mutex", 0);
    sem_t *empty = sem_open("/empty", 0);
    sem_t *full = sem_open("/full", 0);

    //error check
    if (mutex == SEM_FAILED || empty == SEM_FAILED || full == SEM_FAILED) {
        perror("Failed to open semaphores");
        return 1;
    }

    //create file and item buffer
    FILE *file;
    char item[20];

    while (1) {
        //wait until buffer is full
        sem_wait(full);

        //wait until buffer is not being accessed
        sem_wait(mutex);

        //open the file to read and consume the produced item
        file = fopen("buffer.txt", "r+");
        if (file == NULL) {
            perror("Failed to open buffer");
            exit(1);
        }

        //read the file
        if (fgets(item, sizeof(item), file) != NULL) {
            consume_item(item);
        }

        //remove the consumed item from the file
        //rewind to beginning
        fseek(file, 0, SEEK_SET);

        //create temp file for truncation
        FILE *temp = fopen("temp.txt", "w");
        char line[20];

        //copy every line except for the last one
        while (fgets(line, sizeof(line), file) != NULL) {
            if (strcmp(line, item) != 0) {
                fprintf(temp, "%s", line);
            }
        }

        //close the two files
        fclose(file);
        fclose(temp);

        //update buffer
        rename("temp.txt", "buffer.txt");

        //indicate no longer in use
        sem_post(mutex);

        //indicate empty
        sem_post(empty);

        //make easier to read
        sleep(2);
    }
}
