#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int readThreads() {

}
int upperThreads() {

}
int replaceThreads() {

}
int writeThreads() {

}


int main(int argc, char *argv[]) {
    char *sharedMemory[]= '';

    if (argc != 8) {
        printf("argüman sayısı hatalı!");
    }
    int read_thread_count = argv[4];
    int upper_thread_count = argv[5];
    int replace_thread_count = argv[6];
    int write_thread_count = argv[7];

    char *input_file_text = argv[2];
    FILE *file = fopen(input_file_text, "r");

    if (file == NULL) {
        perror("Error opening file\n");
    }

    //dosyadaki satır sayısını buluyoruz
    int linecount = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            linecount++;
        }
    }
    //en son satırı da saymak için
    fseek(file, -1, SEEK_END);
    if (fgetc(file) != '\n') {
        linecount++;
    }

    //threadleri yaratma kısmı
    pthread_t read_thread[read_thread_count];
    for (int i = 0; i < read_thread_count; i++) {
        if (pthread_create(&read_thread[i], NULL, readThreads, NULL) != 0) {
            printf("Error creating read thread\n");
        }
    }

    pthread_t upper_thread[upper_thread_count];
    for (int i = 0; i < upper_thread_count; i++) {
        if (pthread_create(&upper_thread[i], NULL, upperThreads, NULL) != 0) {
            perror("Error creating upper thread\n");
        }
    }

    pthread_t replace_thread[replace_thread_count];
    for (int i = 0; i < replace_thread_count; i++) {
        if (pthread_create(&replace_thread[i], NULL, replaceThreads, NULL) !=0 ) {
            perror("Error creating replace thread\n");
        }
    }
    pthread_t write_thread[write_thread_count];
    for (int i = 0; i < write_thread_count; i++) {
        if (pthread_create(&write_thread[i], NULL, writeThreads, NULL)!= 0) {
            perror("Error creating write thread\n");
        }
    }

    //threadleri çalıştırma kısmı

    for (int i = 0; i < read_thread_count; i++) {
       if (pthread_join(read_thread[i],NULL) != 0) {
           perror("Error joining read thread\n");
       }
    }

    for (int i = 0; i < upper_thread_count; i++) {
        if (pthread_join(upper_thread[i],NULL) != 0) {
            perror("Error joining upper thread\n");
        }
    }

    for (int i = 0; i < replace_thread_count; i++) {
        if (pthread_join(replace_thread[i],NULL) != 0) {
            perror("Error joining replace thread\n");
        }
    }
    for (int i = 0; i < write_thread_count; i++) {
        if (pthread_join(write_thread[i],NULL) != 0) {
            perror("Error joining write thread\n");
        }
    }







    return 0;
}