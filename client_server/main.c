#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>

#define BUFFER_SIZE 256
#define CLIENTS_NUMBER 3

char buffer[BUFFER_SIZE] = "";
int stringId = 0;
int clientsWorkingCount = 0;
FILE* file;
pthread_rwlock_t bufferLock;
pthread_rwlock_t fileLock;
pthread_rwlock_t countLock;
pthread_rwlock_t idLock;


void * server(void *argp) {
    while(1) {
        pthread_rwlock_wrlock(&countLock);
        if(clientsWorkingCount == 0) {
            pthread_rwlock_wrlock(&bufferLock);
            fgets(buffer, BUFFER_SIZE, stdin);
            pthread_rwlock_wrlock(&idLock);
            stringId++;
            pthread_rwlock_unlock(&idLock);
            clientsWorkingCount = CLIENTS_NUMBER;
            if(strcmp("quit\n",buffer) == 0) {
                pthread_rwlock_unlock(&bufferLock);
                break;
            }
            pthread_rwlock_unlock(&bufferLock);
        }
        pthread_rwlock_unlock(&countLock);
        sched_yield();
    }
    pthread_exit(0);
}

void * client(void *argp) {
    int myStringId = 0;
    while(1) {
        pthread_rwlock_rdlock(&idLock);
        if(myStringId != stringId) {
            pthread_rwlock_rdlock(&bufferLock);
            if(strcmp("quit\n",buffer) == 0) {
                pthread_rwlock_unlock(&bufferLock);
                break;
            }
            pthread_rwlock_wrlock(&fileLock);
            fputs(buffer,file);
            pthread_rwlock_unlock(&fileLock);
            pthread_rwlock_unlock(&bufferLock);
            myStringId = stringId;
            pthread_rwlock_wrlock(&countLock);
            clientsWorkingCount--;
            pthread_rwlock_unlock(&countLock);
        }
        pthread_rwlock_unlock(&idLock);
        sched_yield();
    }
    pthread_exit(0);
}

int main() {
    pthread_t serverThread, client1Thread, client2Thread, client3Thread;

    file = fopen("output.txt","w+");

    pthread_rwlock_init(&bufferLock, 0);
    pthread_rwlock_init(&fileLock, 0);
    pthread_rwlock_init(&idLock, 0);
    pthread_rwlock_init(&countLock, 0);

    pthread_create(&serverThread, 0, server, 0);
    pthread_create(&client1Thread, 0, client, 0);
    pthread_create(&client2Thread, 0, client, 0);
    pthread_create(&client3Thread, 0, client, 0);

    pthread_join(serverThread, 0);
    pthread_join(client1Thread, 0);
    pthread_join(client2Thread, 0);
    pthread_join(client3Thread, 0);

    pthread_rwlock_destroy(&countLock);
    pthread_rwlock_destroy(&idLock);
    pthread_rwlock_destroy(&fileLock);
    pthread_rwlock_destroy(&bufferLock);

    fclose(file);

    return 0;
}
