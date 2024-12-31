#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

pthread_mutex_t *line_try_lockers;

sem_t writer_sem;

// The string array that will hold the lines to be modified and write back again
char **shared_memory;

/* The int double array that will hold the information on whether the line is modified or not
   Double array because we have to know two things : is line underscored and uppercased ? */
int **status_array;

/* These two global queues above will be filled inside the main() because we don't know the number
   of lines in the file yet */

int total_lines;

struct thread_args {
    char *input_file;
    int id;
};


// Functions below are the ones that will be run by threads to get their work done
void *readThreads(void *arg) {
    while (1) {
        char *file_name = ((struct thread_args *)arg)->input_file;
        int id = ((struct thread_args *)arg)->id;
        char line[256];
        int target_line;
        int current_line = 0;
        int counter = 0;
        for (int i = 0; i < total_lines; i++) {
            int target_line = i;
            int current_line = 0;
            if (pthread_mutex_trylock(&line_try_lockers[i]) == 0) {
                if (status_array[i][0] == 0) {

                    target_line = i;

                    FILE *file = fopen(file_name, "r");

                    if (file == NULL) {
                        printf("File could not be opened!\n");
                        pthread_exit(NULL);
                    }

                    while (fgets(line, sizeof(line), file)) {
                        if (current_line == target_line) {
                            memcpy(shared_memory[target_line], line, strlen(line));
                            status_array[i][0] = 1;
                            break;
                        }
                        current_line++;
                    }


                    printf("Read_%d read the line %d which is %s\n", id, i, line);

                    fclose(file);
                }
                else if (status_array[i][0] == 1) {
                    counter++;
                }
                pthread_mutex_unlock(&line_try_lockers[i]);
            }
            else {
                continue;
            }
            // CRITICAL - FINISH ************
        }
        if (counter == total_lines) {
            pthread_exit(NULL);
        }
    }


}
void *upperThreads(void *arg) {
    while (1){
        char *file_name = ((struct thread_args *)arg)->input_file;
        int id = ((struct thread_args *)arg)->id;

        char line[256];
        int upper_var;
        int counter = 0;

        for (int i = 0; i < total_lines; i++) {
            if (pthread_mutex_trylock(&line_try_lockers[i]) == 0) {
                if (status_array[i][0] == 1 && status_array[i][1] == 0) {
                    //--------------------critical begins-----------------
                    strcpy(line, shared_memory[i]);
                    char * old_line = strdup(line);
                    for (int j = 0; j < strlen(line); j++) {
                        upper_var = toupper((unsigned char)line[j]);
                        line[j] =(unsigned char)(upper_var);
                    }

                    memcpy(shared_memory[i], line, strlen(line));
                    status_array[i][1] = 1;

                    printf("Upper_%d read the index %d and converted %s to %s\n", id, i, old_line, line);


                }
                else if (status_array[i][0] == 1 && status_array[i][1] == 1) {
                    counter++;

                }
                pthread_mutex_unlock(&line_try_lockers[i]);
            }
            else {
                continue;
            }

        }

        if (counter == total_lines) {
            pthread_exit(NULL);
        }
    }
}

void *replaceThreads(void *arg) {
    while (1){
        char *file_name = ((struct thread_args *)arg)->input_file;
        int id = ((struct thread_args *)arg)->id;

        char line[256];

        int counter =0;

        for (int i = 0; i < total_lines; i++) {
            if (pthread_mutex_trylock(&line_try_lockers[i]) == 0) {
                if (status_array[i][0] == 1 && status_array[i][2] == 0) {
                    //--------------------critical begins-----------------
                    strcpy(line, shared_memory[i]);
                    char * old_line = strdup(line);
                    for (int j = 0; j < strlen(line); j++) {
                        if (line [j] == ' ') {
                            line[j] = '_';
                        }
                    }
                    memcpy(shared_memory[i], line, strlen(line));
                    status_array[i][2] = 1;

                    printf("Replace_%d read the index %d and converted %s to %s\n", id, i, old_line, line);

                }
                else if (status_array[i][0] == 1 && status_array[i][2] == 1) {
                    counter++;

                }
                pthread_mutex_unlock(&line_try_lockers[i]);
            }
            else {
                continue;
            }

        }

        if (counter == total_lines) {
            pthread_exit(NULL);
        }
    }
}


void *writeThreads(void *arg) {
    while (1) {
        FILE *file;
        FILE *temp_file;
        char *file_name = ((struct thread_args *)arg)->input_file;
        int id = ((struct thread_args *)arg)->id;
        char line[256];
        int counter = 0;

        for (int i = 0; i < total_lines; i++) {
            sem_wait(&writer_sem);
            if (status_array[i][0] == 1 && status_array[i][1] == 1 && status_array[i][2] == 1 && status_array[i][3] == 0) {

                int target_line = i;
                int current_line = 0;


                file = fopen(file_name, "r");
                if (file == NULL) {
                    perror("File could not be opened!\n");
                    pthread_exit(NULL);
                }

                temp_file= fopen("temp.txt", "w");
                if (temp_file == NULL) {
                    perror("Temp file could not be opened!\n");
                    fclose(file);
                    pthread_exit(NULL);
                }

                while (fgets(line, sizeof(line), file)) {
                    if (current_line == target_line) {
                        fputs(shared_memory[target_line], temp_file);
                        status_array[i][3] = 1;
                    }
                    else {
                        fputs(line, temp_file);
                    }
                    current_line++;
                }


                fclose(file);
                fclose(temp_file);

                remove(file_name);
                rename("temp.txt", file_name);

                printf("Write_%d write line %d back which is %s\n", id, i, shared_memory[target_line]);



            }
            else if (status_array[i][0] == 1 && status_array[i][1] == 1 && status_array[i][2] == 1 && status_array[i][3] == 1) {
                counter++;
            }
            sem_post(&writer_sem);
        }

        if (counter == total_lines) {
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

    total_lines = linecount;

    fclose(file);

    shared_memory = (char **) malloc(linecount * sizeof(char) * 256);
    status_array = (int **) malloc(linecount * sizeof(int) * 4);
    // Initializing the global arrays with the linecount
    for (int i = 0; i < linecount; i++) {
        shared_memory[i] = (char *) malloc(256 * sizeof(char));
    }

    // Filling the status arrays with int[3] that will hold the info on : is read ? is uppercased ? is replaced ? is written ?
    for (int i = 0; i < linecount; i++) {
        status_array[i] = (int *) malloc(4 * sizeof(int));
        status_array[i][0] = 0;
        status_array[i][1] = 0;
        status_array[i][2] = 0;
        status_array[i][3] = 0;
    }

    line_try_lockers = (pthread_mutex_t *) malloc(linecount * sizeof(pthread_mutex_t));
    sem_init(&writer_sem, 0, 1);

    for (int i = 0; i < linecount; i++) {
        pthread_mutex_init(&line_try_lockers[i], NULL);
    }



    /* Part that we create the threads each by each according to given numbers from the user
       Also we are creating pthread_t arrays for each type that will hold the thread id's of the created threads */



    pthread_t read_thread[read_thread_count];
    for (int i = 0; i < read_thread_count; i++) {
        struct thread_args *thread_args = malloc(sizeof(struct thread_args));
        thread_args->input_file = input_file_text;
        thread_args->id = i;
        if (pthread_create(&read_thread[i], NULL, &readThreads, (void *) thread_args) != 0) {
            printf("Error creating read thread\n");
        }
    }

    pthread_t upper_thread[upper_thread_count];
    for (int i = 0; i < upper_thread_count; i++) {
        struct thread_args *thread_args = malloc(sizeof(struct thread_args));
        thread_args->input_file = input_file_text;
        thread_args->id = i;
        if (pthread_create(&upper_thread[i], NULL, &upperThreads, (void *) thread_args) != 0) {
            perror("Error creating upper thread\n");
        }
    }

    pthread_t replace_thread[replace_thread_count];
    for (int i = 0; i < replace_thread_count; i++) {
        struct thread_args *thread_args = malloc(sizeof(struct thread_args));
        thread_args->input_file = input_file_text;
        thread_args->id = i;
        if (pthread_create(&replace_thread[i], NULL, &replaceThreads, (void *) thread_args) !=0 ) {
            perror("Error creating replace thread\n");
        }
    }
    pthread_t write_thread[write_thread_count];
    for (int i = 0; i < write_thread_count; i++) {
        struct thread_args *thread_args = malloc(sizeof(struct thread_args));
        thread_args->input_file = input_file_text;
        thread_args->id = i;
        if (pthread_create(&write_thread[i], NULL, &writeThreads, (void *) thread_args)!= 0) {
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