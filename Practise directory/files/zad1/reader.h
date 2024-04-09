//
// Created by maciejgrzybacz on 08.04.24.
//

#ifndef SYSOPY_READER_H
#define SYSOPY_READER_H
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include<sys/stat.h>
int read_file(char* path);
int read_dir(char* path);

#endif //SYSOPY_READER_H
