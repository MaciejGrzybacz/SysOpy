//
// Created by maciejgrzybacz on 12.03.24.
//

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

int byte_after_byte(const char *rfile, const char *wfile) {
    FILE* rf = fopen(rfile,"rb");
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

#include <stdio.h>

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

    // Obracanie bloków danych
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

    // Obracanie pozostałych danych (jeśli istnieją)
    int remaining_size = size % block_size;
    if (remaining_size > 0) {
        fseek(rf, -size, SEEK_END); // Przesunięcie na początek pliku
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
    if(byte_after_byte("large_file.txt", "bytefile.txt")==0) {
        printf("byte_after_byte: Success\n");
    }
    else {
        printf("byte_after_byte: Error\n");
        return 1;
    }
    if(block_after_block("large_file.txt", "blockfile.txt")==0) {
        printf("block_after_block: Success\n");
    }
    else {
        printf("block_after_block: Error\n");
        return 1;
    }
    return 0;
}