#include <stdio.h>
#include <zconf.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>

#define MAX_FILE_NAME 1000

struct node {
    struct node* prev;
    char name[MAX_FILE_NAME];
};

int get_cwd(char* buffer, size_t size) {

    struct stat* _stat = malloc(sizeof(struct stat));
    stat("/", _stat);
    __ino_t _root_ino = _stat->st_ino;
    __dev_t _root_dev = _stat->st_dev;

    char* homedir;
    homedir = getpwuid(getuid())->pw_dir;
    stat(homedir, _stat);
    __ino_t _home_ino = _stat->st_ino;
    __dev_t _home_dev = _stat->st_dev;

    stat(".", _stat);
    __ino_t _start_ino = _stat->st_ino;
    __dev_t _start_dev = _stat->st_dev;
    int _link_count = 0;

    struct node* _cur_node = NULL;
    __ino_t _cur_ino;
    __dev_t _cur_dev;
    char _str[MAX_FILE_NAME] = "";
    while(1) {


        DIR* _dir = opendir(".");
        struct dirent* _dirent;
        while((_dirent = readdir(_dir)) != NULL) {
            strcpy(_str, "");
            strcat(_str, _dirent->d_name);
            stat(_str, _stat);
            if(_stat->st_ino == _cur_ino && _stat->st_dev == _cur_dev) {
                struct node* _node = malloc(sizeof(struct node));
                _node->prev = _cur_node;
                strcpy(_node->name, _dirent->d_name);
                _cur_node = _node;
            }
            if(_stat->st_ino == _start_ino && _stat->st_dev == _start_dev) {
                lstat(_str, _stat);
                if(_stat->st_ino != _start_ino || _stat->st_dev != _start_dev) {
                    _link_count++;
                }
            }

        }

        stat(".", _stat);
        _cur_ino = _stat->st_ino;
        _cur_dev = _stat->st_dev;
        if(_cur_ino == _root_ino && _cur_dev == _root_dev) {
            break;
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

    free(_stat);

    return _link_count;


}


int get_cwd_sandbox(char* buffer, size_t size) {

    struct stat* _stat = malloc(sizeof(struct stat));
    stat("/", _stat);
    __ino_t _root_ino = _stat->st_ino;
    __dev_t _root_dev = _stat->st_dev;

    char* homedir;
    homedir = getpwuid(getuid())->pw_dir;
    stat(homedir, _stat);
    __ino_t _home_ino = _stat->st_ino;
    __dev_t _home_dev = _stat->st_dev;

    stat(".", _stat);
    __ino_t _start_ino = _stat->st_ino;
    __dev_t _start_dev = _stat->st_dev;
    int _link_count = 0;

    struct node* _cur_node = NULL;
    __ino_t _cur_ino;
    __dev_t _cur_dev;
    char _str[MAX_FILE_NAME] = "";
    while(1) {


        DIR* _dir = opendir(".");
        struct dirent* _dirent;
        while((_dirent = readdir(_dir)) != NULL) {
            strcpy(_str, "");
            strcat(_str, _dirent->d_name);
            stat(_str, _stat);
            if(_stat->st_ino == _cur_ino && _stat->st_dev == _cur_dev) {
                struct node* _node = malloc(sizeof(struct node));
                _node->prev = _cur_node;
                strcpy(_node->name, _dirent->d_name);
                _cur_node = _node;
            }
            if(_stat->st_ino == _start_ino && _stat->st_dev == _start_dev) {
                lstat(_str, _stat);
                if(_stat->st_ino != _start_ino || _stat->st_dev != _start_dev) {
                    _link_count++;
                }
            }


        }

        stat(".", _stat);
        _cur_ino = _stat->st_ino;
        _cur_dev = _stat->st_dev;
        if(_cur_ino == _root_ino && _cur_dev == _root_dev) {
            break;
        }
        if(_cur_ino == _home_ino && _cur_dev == _home_dev) {
            _link_count = 0;
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

    free(_stat);

    return _link_count;

}


int main() {
    size_t BUFFER_SIZE = MAX_FILE_NAME;
    char* buffer = malloc(BUFFER_SIZE);

    int link_count = get_cwd(buffer, BUFFER_SIZE);
    printf("%s\n", buffer);
    printf("%i links\n", link_count);

    return 0;
}


