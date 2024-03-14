//
// Created by maciejgrzybacz on 12.03.24.
//

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int byte_after_byte(const char *rfile, const char *wfile) {
    FILE* rf = fopen(rfile,"r");
    FILE* wf = fopen(wfile, "w");

    if(rf == NULL || wf == NULL) {
        printf("Error while file opening. \n");
        return 1;
    }

    fseek(rf,0,2);
    long size= ftell(rf);

    char c;
    for(int i=1; i<=size; i++) {
        if(fseek(rf,-i,2)) {
            printf("Error while seeking file\n");
            return 1;
        }
        fread(&c,1,1,rf);
        fwrite(&c,1,1,wf);
    }

    fclose(rf);
    fclose(wf);

    return 0;
}

int block_after_block(const char *rfile, const char *wfile) {
    FILE* rf = fopen(rfile, "rb");
    FILE* wf = fopen(wfile, "wb");

    if (rf == NULL || wf == NULL) {
        printf("Error while file opening. \n");
        return 1;
    }

    fseek(rf, 0, SEEK_END);
    long size = ftell(rf);

    int block_size = 1024;
    char buffer[block_size];

    for (int i = 1; i <= size / block_size; i++) {
        fseek(rf, -i * block_size, SEEK_END);
        fread(buffer, 1, block_size, rf);
        for (int j = 0; j < block_size / 2; j++) {
            char c = buffer[j];
            buffer[j] = buffer[block_size - j - 1];
            buffer[block_size - j - 1] = c;
        }
        fwrite(buffer, 1, block_size, wf);
    }


    int remaining_size = size % block_size;
    if (remaining_size > 0) {
        fseek(rf, -size, SEEK_END);
        fread(buffer, 1, remaining_size, rf);
        for (int j = 0; j < remaining_size / 2; j++) {
            char c = buffer[j];
            buffer[j] = buffer[remaining_size - j - 1];
            buffer[remaining_size - j - 1] = c;
        }
        fwrite(buffer, 1, remaining_size, wf);
    }

    fclose(rf);
    fclose(wf);

    return 0;
}

int main() {
    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();
    FILE* file = fopen("pomiar_zad_2.txt","w");

    if(byte_after_byte("/home/maciejgrzybacz/CLionProjects/SysOpy/cw02/large_file.txt", "bytefile.txt")==0) {
        printf("byte_after_byte: Success\n");
    }
    else {
        printf("byte_after_byte: Error\n");
        return 1;
    }
    end_time=clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    fprintf(file, "Time needed to reverse a file byte after byte was: %f s.\n", cpu_time_used);


    start_time=clock();
    if(block_after_block("/home/maciejgrzybacz/CLionProjects/SysOpy/cw02/large_file.txt", "blockfile.txt")==0) {
        printf("block_after_block: Success\n");
    }
    else {
        printf("block_after_block: Error\n");
        return 1;
    }
    end_time=clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    fprintf(file, "Time needed to reverse a file block after block was: %f s. \n \n", cpu_time_used);
    fclose(file);
    return 0;
}