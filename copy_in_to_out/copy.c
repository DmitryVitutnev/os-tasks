#include <stdio.h>
#include <zconf.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("You need to specify buffer length");
        return 1;
    }
    int bufferSize;
    if(sscanf(argv[1], "%d", &bufferSize) == 0 || bufferSize < 0) {
        printf("Buffer length is incorrect");
        return 1;
    }
    void* buffer = malloc(bufferSize);

    int bytes_num;
    while ((bytes_num = read(STDIN_FILENO, buffer, bufferSize)) > 0)  {
        write(STDOUT_FILENO, buffer, bytes_num);
    }

    return 0;
}
