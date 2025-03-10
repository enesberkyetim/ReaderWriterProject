#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// The string array that will hold the lines to be modified and write back again
char **shared_memory;

/* The int double array that will hold the information on whether the line is modified or not
   Double array because we have to know two things : is line underscored and uppercased ? */
int **status_array;

/* These two global queues above will be filled inside the main() because we don't know the number
   of lines in the file yet */


// Functions below are the ones that will be run by threads to get their work done
void *readThreads(void *arg) {
    while (1) {
        char *file_name = (char *) arg;
        char *line = malloc(256 * sizeof(char));
        int target_line;
        int current_line = 0;
        int counter = 0;
        for (int i = 0; i < sizeof(status_array) / (4 * sizeof(int)); i++) {
            if (status_array[i][0] == 0) {
                // CRITICAL - BEGIN **********
                target_line = i;

                FILE *file = fopen(file_name, "r");

                if (file == NULL) {
                    printf("File could not be opened!\n");
                    return 1;
                }

                while (fgets(line, sizeof(line), file)) {
                    if (current_line == target_line) {
                        memcpy(shared_memory[target_line], line, sizeof(char) * strlen(line));
                        status_array[i][0] = 1;
                        break;
                    }
                    current_line++;
                }

                fclose(file);
                // CRITICAL - FINISH ************
                break;
            }
            else if (status_array[i][0] == 1) {
                counter++;
            }
        }
        if (counter == sizeof(status_array) / (4 * sizeof(int))) {
            pthread_exit(NULL);
        }
    }


}
void *upperThreads(void *arg) {
    while (1){

        char* line = malloc(256 * sizeof(char));
        int upper_var;
        int counter =0;

        for (int i = 0; i < sizeof(status_array) / (4 * sizeof(int)); i++) {
            if (status_array[i][0] == 1 && status_array[i][1] == 0) {
                //--------------------critical begins-----------------
                strcpy(line, shared_memory[i]);
                for (int j = 0; j < strlen(line); j++) {
                    upper_var = toupper((unsigned char)line[j]);
                    line[j] =(unsigned char)(upper_var);
                }

                memcpy(shared_memory[i], line, sizeof(char) * strlen(line));
                status_array[i][1] = 1;
                break;
            }

            else if (status_array[i][0] == 1 && status_array[i][1] == 1) {
                counter++;
            }
        }

        if (counter == sizeof(status_array) / (4 * sizeof(int))) {
            pthread_exit(NULL);
        }
    }
}

void *replaceThreads() {
    while (1){

        char* line = malloc(256 * sizeof(char));

        int counter =0;

        for (int i = 0; i < sizeof(status_array) / (4 * sizeof(int)); i++) {
            if (status_array[i][0] == 1 && status_array[i][2] == 0) {
                //--------------------critical begins-----------------
                strcpy(line, shared_memory[i]);
                for (int j = 0; j < strlen(line); j++) {
                    if (line [j] == '') {
                        line[j] = '_';
                    }
                }
                memcpy(shared_memory[i], line, sizeof(char) * strlen(line));
                status_array[i][2] = 1;
                break;
            }

            else if (status_array[i][0] == 1 && status_array[i][2] == 1) {
                counter++;
            }
        }

        if (counter == sizeof(status_array) / (4 * sizeof(int))) {
            pthread_exit(NULL);
        }
    }
}


void *writeThreads(void *arg) {
    while (1) {
        FILE *file;
        FILE *temp_file;
        char *file_name = (char *) arg;
        char *line = malloc(256 * sizeof(char));
        int target_line;
        int current_line = 0;
        int counter = 0;

        for (int i = 0; i < sizeof(status_array) / (4 * sizeof(int)); i++) {
            if (status_array[i][0] == 1 && status_array[i][1] == 1 && status_array[i][2] == 1 && status_array[i][3] == 0) {
                file = fopen((char *) arg, "r");
                if (file == NULL) {
                    perror("File could not be opened!\n");
                    return 1;
                }

                temp_file= fopen("temp.txt", "w");
                if (temp_file == NULL) {
                    perror("Temp file could not be opened!\n");
                    fclose(file);
                    return 1;
                }

                while (fgets(line, sizeof(line), file)) {
                    if (current_line == target_line) {
                        fputs(shared_memory[target_line], temp_file);
                        status_array[i][3] = 1;
                        break;
                    }
                    fputs(line, temp_file);
                    current_line++;
                }
                fclose(file);
                fclose(temp_file);

                remove((char *) arg);
                rename("temp.txt", (char *) arg);

                break;
            }
            else if (status_array[i][0] == 1 && status_array[i][1] == 1 && status_array[i][2] == 1 && status_array[i][3] == 1) {
                counter++;
            }
        }

        if (counter == sizeof(status_array) / (4 * sizeof(int))) {
            pthread_exit(NULL);
        }
    }
}

int main(int argc, char *argv[]) {

    /* Here we check the argument count and storing the input from user that will tell us how many threads
        will be created for each type */
    if (argc != 8) {
        perror("Argument count is incorrect!");
    }
    int read_thread_count = atoi(argv[4]);
    int upper_thread_count = atoi(argv[5]);
    int replace_thread_count = atoi(argv[6]);
    int write_thread_count = atoi(argv[7]);

    // Opening the file to be read for counting the number of lines (we need to fill char ** and int ** global arrays)
    char *input_file_text = argv[2];
    FILE *file = fopen(input_file_text, "r");

    if (file == NULL) {
        perror("Error opening file\n");
    }

    // Finding the number of lines inside the input file
    int linecount = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            linecount++;
        }
    }

    // Also counting the last line (in case of there is no \n at the end if there is EOF)
    fseek(file, -1, SEEK_END);
    if (fgetc(file) != '\n') {
        linecount++;
    }

    fclose(file);


    // Initializing the global arrays with the linecount
    shared_memory = (char **) malloc(linecount * sizeof(char *));
    status_array = (int **) malloc(linecount * sizeof(int *));

    // Filling the status arrays with int[3] that will hold the info on : is read ? is uppercased ? is replaced ? is written ?
    for (int i = 0; i < linecount; i++) {
        status_array[i] = (int *) malloc(4 * sizeof(int));
    }

    /* Part that we create the threads each by each according to given numbers from the user
       Also we are creating pthread_t arrays for each type that will hold the thread id's of the created threads */
    pthread_t read_thread[read_thread_count];
    for (int i = 0; i < read_thread_count; i++) {
        if (pthread_create(&read_thread[i], NULL, readThreads, (void *) input_file_text) != 0) {
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

    /* Here the main thread will wait for all other threads that it created before exiting
       Otherwise it will end before them and cause all of them to be terminated early */

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