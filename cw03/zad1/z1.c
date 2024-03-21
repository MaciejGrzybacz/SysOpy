//
// Created by maciejgrzybacz on 12.03.24.
//
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#ifdef BYTE_MODE
int byte_after_byte(const char *rfile, const char *wfile) {
    FILE* rf = fopen(rfile,"rb");
    FILE* wf = fopen(wfile, "wb");

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
#else
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
#endif

int main(int argc, char* args[]) {
    clock_t start_time, end_time;
    double cpu_time_used;
    FILE* file = fopen("pomiar_zad_2.txt","a");

#ifdef BYTE_MODE
    start_time = clock();
    if(byte_after_byte(args[1], args[2])==0) {
        printf("byte_after_byte: Success\n");
    }
    else {
        printf("byte_after_byte: Error\n");
        return 1;
    }
    end_time=clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    fprintf(file, "Time needed to reverse a %s byte after byte was: %f s.\n", args[1], cpu_time_used);
#else
    start_time=clock();
    if(block_after_block(args[1], args[2])==0) {
        printf("block_after_block: Success\n");
    }
    else {
        printf("block_after_block: Error\n");
        return 1;
    }
    end_time=clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    fprintf(file, "Time needed to reverse a %s block after block was: %f s. \n",args[1], cpu_time_used);
#endif

    fclose(file);
    return 0;
}
