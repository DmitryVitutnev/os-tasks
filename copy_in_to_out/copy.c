#include <stdio.h>
#include <zconf.h>
#include <fcntl.h>
#include <stdlib.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Not enough arguments");
        return 1;
    }
    int bufferSize;
    if(sscanf(argv[1], "%d", &bufferSize) == 0 || bufferSize < 0) {
        printf("Buffer length is incorrect");
        return 1;
    }
    void* buffer = malloc(bufferSize);
    close(0);
    if(open(argv[2], O_RDONLY) == -1) {
        printf("File not exist");
        return 1;
    }
    close(1);
    open(argv[3], O_WRONLY | O_CREAT, FILE_MODE);

    int bytes_num;
    while ((bytes_num = read(0, buffer, bufferSize)) > 0)  {
        write(1, buffer, bytes_num);
    }

    close(0);
    close(1);

    return 0;
}
