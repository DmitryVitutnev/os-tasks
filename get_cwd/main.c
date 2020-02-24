#include <stdio.h>
#include <zconf.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define MAX_FILE_NAME 100

struct node {
    struct node* prev;
    char name[MAX_FILE_NAME];
};

int get_cwd(char* buffer, size_t size) {

    struct stat *_stat = malloc(sizeof(struct stat));
    stat("/", _stat);
    __ino_t _root_ino = _stat->st_ino;

    struct node* _cur_node;


    while(1) {
        stat(".", _stat);
        if(_stat->st_ino == _root_ino) {
            break;
        }
        DIR* _dir = opendir("..");
        struct dirent* _dirent = malloc(sizeof(struct dirent));
        while((_dirent = readdir(_dir)) != NULL) {
            if(_dirent->d_ino == _stat->st_ino) {
                struct node* _node = malloc(sizeof(struct node));
                _node->prev = _cur_node;
                strcpy(_node->name, _dirent->d_name);
                _cur_node = _node;
            }
        }
        chdir("..");
    }

    strcpy(buffer, "");
    struct node* _old_node;
    while(_cur_node != NULL) {
        strcat(buffer, "/");
        strcat(buffer, _cur_node->name);
        _old_node = _cur_node;
        _cur_node = _cur_node->prev;
        free(_old_node);
    }

    return 0;

}

int main() {
    size_t BUFFER_SIZE = 128;
    char* buffer = malloc(BUFFER_SIZE);

    /*getcwd(buffer, BUFFER_SIZE);
    printf("%s\n", buffer);
    chdir("../../../../..");
    getcwd(buffer, BUFFER_SIZE);
    printf("%s\n", buffer);
    struct stat *result = malloc(sizeof(struct stat));
    stat("..", result);
    printf("%ld",result->st_ino);*/

    get_cwd(buffer, BUFFER_SIZE);
    printf("%s\n", buffer);
    return 0;
}


