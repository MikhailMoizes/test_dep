#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct name_and_update {
    char *list;
    unsigned int update_time;
} List;

int scanfolder(int, char*);
int diag(char*);
int compare(const List *x1, const List *x2); // comparator for "qsort"

static List list[512];

int main(int argc, char* argv[]) {

    if(chdir(argv[1]) < 0) {
        return diag(argv[1]);
    }

    if(scanfolder(S_IFDIR, "/\n") > 0) {
        return diag(argv[1]);
    }

    scanfolder(S_IFREG, " \n");
    return 0;

}

int compare(const List *x1, const List *x2) {
    return (x1->update_time - x2->update_time);
}

int diag(char* mes) {
    if (mes != NULL) {
        write(1, mes, strlen(mes));
    } else {
        write(1, "<NULL> ", strlen("<NULL> "));
    }
        write(1, ": ", 2);
        write(1, strerror(errno), strlen(strerror(errno)));
        write(1, "\n", 1);
        exit(errno);
}

int scanfolder(int type, char* suf) {

    DIR* fdir;
    struct dirent* folder;
    struct stat sbuf[1];
    char* start;
    char* s;
    size_t count = 0;
    int len;

    if((fdir = opendir(".")) == NULL) {
        return(errno);
    }

    start = sbrk(0);
    while((folder = readdir(fdir)) != NULL) {
        stat(folder->d_name, sbuf);
        if(((sbuf->st_mode) & S_IFMT) != type) {
            continue;
        }

        len = strlen(folder->d_name);
        s = sbrk(len+1);
        memcpy(s, folder->d_name, len);
        list[count].list = s;
        list[count].update_time = sbuf->st_ctime;
        list[++count].list = NULL;
    }

    closedir(fdir);
    qsort(list, count, sizeof(List), compare);
    count = 0;
    while(list[count].list != NULL) {
        len = strlen(list[count].list);
        write(1, list[count++].list, len);
        write(1, suf, 2);
    }
    brk(start);
    return 0;
}