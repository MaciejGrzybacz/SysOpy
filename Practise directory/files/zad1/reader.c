//
// Created by maciejgrzybacz on 08.04.24.
//

#include "reader.h"


int read_file(char* path) {
    FILE* handler=fopen(path,"r");
    if (!handler) {
        printf("Error while file opening");
        return 1;
    }
    char c;
    while(fread(&c,sizeof(char),1,handler)){
        printf("%c",c);
    }
    fclose(handler);
    return 0;
}

int read_dir(char* path) {
    DIR* dir= opendir(path);
    if(!dir) {
        printf("Error while opening directory");
        return 1;
    }
    struct dirent* entry= readdir(dir);
    struct stat info;
    while(entry) {
        char full_path[2137];
        snprintf(full_path, sizeof(full_path),"%s/%s", path, entry->d_name);

        if(stat(full_path,&info) == -1) {
            printf("Error while getting file info");
            continue;
        }
        if(!S_ISDIR(info.st_mode) & info.st_size<100) {
            read_file(full_path);
        }
        entry= readdir(dir);
    }
    return 0;
}

int main(int argc, char** argv) {
    if(argc<2) {
        printf("Invalid number of arguments");
        return 1;
    }

    read_dir(argv[1]);
}