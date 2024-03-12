//
// Created by maciejgrzybacz on 12.03.24.
//

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int show_files_and_sizes(const char *path) {
    DIR *dir = opendir(path);
    struct dirent *entry;
    struct stat file_info;
    long long cumulative_size = 0;

    if (dir == NULL) {
        printf("Error while opening directory\n");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_info) == -1) {
            printf("Error while getting file information.\n");
            continue;
        }

        if (!S_ISDIR(file_info.st_mode)) {
            printf("Name: %s, size: %lld\n", entry->d_name, (long long)file_info.st_size);
            cumulative_size += (long long)file_info.st_size;
        }
    }

    printf("Cumulative size: %lld\n", cumulative_size);
    closedir(dir);
    return 0;
}

int main() {
    show_files_and_sizes("/home/maciejgrzybacz/CLionProjects/SysOpy/cw02/zad1");
    return 0;
}
