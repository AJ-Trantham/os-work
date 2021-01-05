/**
 * Name: AJ Trantham
 * Email: ajtrantham@u.boisestate.edu
 * Github Username: AJ-Trantham
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "lab.h"


void fastlog_init(void) {
    memset(&fastlog, 0, sizeof(struct Fastlog));
    fastlog.current_index = 0;
    fastlog.total_added = 0;
    int num_entries = MAX_LOG_ENTRY;
    int i;
    for (i = 0; i < num_entries; i++) {
        memset(&fastlog.entries[i], 0, sizeof(struct FastlogEntry));
        fastlog.entries[i].exists = 0;
        fastlog.entries[i].pid = -1;
        fastlog.entries[i].level = INFO;
        fastlog.entries[i].timestamp = 1;
        // ensure the string array is initialized memory
        int j;
        for (j=0; j < MAX_MSG_LENGTH; j++) {
            fastlog.entries[i].text[j] = 0;
        }
    }
}

void fastlog_destroy(void) {
    int i;
    int num_entries = MAX_LOG_ENTRY;
    for (i = 0; i < num_entries; i++) {
        fastlog.entries[i].exists = 0;
    }
}

void increment_index(void) {
    if (fastlog.current_index < MAX_LOG_ENTRY-1) {
        fastlog.current_index++;
    } else {
        fastlog.current_index = 0;
    }
    // prevents the int from overflowing
    if (fastlog.total_added < INT_MAX) {
        fastlog.total_added++;
    }
}

int get_prev_index(void) {
    if (fastlog.current_index == 0) {
        return MAX_LOG_ENTRY-1;
    } else {
        return fastlog.current_index - 1;
    }
}

void fastlog_write(LEVEL lvl, char *text) {
    if (text != NULL && strnlen(text, MAX_MSG_LENGTH) >= MAX_MSG_LENGTH) {
        printf("text is too long!\n");
    } else {
        struct FastlogEntry newEntry;
        memset(&newEntry, 0, sizeof(struct FastlogEntry)); //initialize the chunk of memory to 0 to avoid valgrind whinning about
        pid_t pid = getpid();
        struct timespec *tp = malloc(sizeof(struct timespec));
        clock_gettime(CLOCK_REALTIME, tp);
        newEntry.level = lvl;
        newEntry.timestamp = tp->tv_sec;
        newEntry.pid = pid;
        newEntry.exists = 1;
        if (text != NULL) {
            strncpy(newEntry.text, text, sizeof(newEntry.text)-1);
        } else {
            strncpy(newEntry.text, "NULL", sizeof(newEntry.text)-1);
        }
        fastlog.entries[fastlog.current_index] = newEntry;
        increment_index();
        free(tp);
    }
}

void fastlog_dump(void) {

    int num_entries = MAX_LOG_ENTRY;
    int oldest_index;

    // edge case before the circular buffer if full
    if (fastlog.total_added <= MAX_LOG_ENTRY) {
        oldest_index = 0;
    } else {
        oldest_index = fastlog.current_index;
    }

    int i;
    int entry_index = oldest_index;
    for (i = 0; i < num_entries; i++) {

        if (fastlog.entries[entry_index].exists) {
            char lvl_str[10];
            lvl_str[0] = '\0';
            switch (fastlog.entries[entry_index].level) {
                case 0:
                    strncat(lvl_str, "INFO", (sizeof(lvl_str) / sizeof(char)) - 1);
                    break;
                case 1:
                    strncat(lvl_str, "WARNING", sizeof(lvl_str) - 1);
                    break;
                case 2:
                    strncat(lvl_str, "ERROR", sizeof(lvl_str) - 1);
                    break;
            }
            fprintf(stderr, "%d-%ld-%s-%s\n", fastlog.entries[entry_index].pid, fastlog.entries[entry_index].timestamp,
                    lvl_str, fastlog.entries[entry_index].text);
        }

        entry_index++;
        if (entry_index > MAX_LOG_ENTRY-1) {
            entry_index = 0;
        }
    }
    printf("\n");
}

void fastlog_dump_stdout(void) {

    int num_entries = MAX_LOG_ENTRY;
    int oldest_index;

    // edge case before the circular buffer if full
    if (fastlog.total_added <= MAX_LOG_ENTRY) {
        oldest_index = 0;
    } else {
        oldest_index = fastlog.current_index;
    }

    int i;
    int entry_index = oldest_index;
    for (i = 0; i < num_entries; i++) {

        if (fastlog.entries[entry_index].exists) {
            char lvl_str[10];
            lvl_str[0] = '\0';
            switch (fastlog.entries[entry_index].level) {
                case 0:
                    strncat(lvl_str, "INFO", (sizeof(lvl_str) / sizeof(char)) - 1);
                    break;
                case 1:
                    strncat(lvl_str, "WARNING", sizeof(lvl_str) - 1);
                    break;
                case 2:
                    strncat(lvl_str, "ERROR", sizeof(lvl_str) - 1);
                    break;
            }

            fprintf(stdout, "%d-%ld-%s-%s\n", fastlog.entries[entry_index].pid, fastlog.entries[entry_index].timestamp,
                    lvl_str, fastlog.entries[entry_index].text);
        }

        entry_index++;
        if (entry_index > MAX_LOG_ENTRY-1) {
            entry_index = 0;
        }
    }
    printf("\n");
}

void fastlog_core_dump(void) {
    char filename[30] = "fastlog.core.";
    int prev_index = get_prev_index();
    int pid = fastlog.entries[prev_index].pid;
    int length = snprintf( NULL, 0, "%d", pid);
    char* str = malloc( length + 1);
    snprintf( str, length + 1, "%d", pid );
    strncat(filename, str, sizeof(filename)-1);

    FILE* fp;
    if ( (fp = fopen(filename, "wb")) == NULL )
    {
        printf("Error opening file\n");
    }

    fwrite(&fastlog, sizeof(struct Fastlog) , 1, fp);
    fclose(fp);
    free(str);
}

void fastlog_core_load(const char *filename) {

    FILE* fp;
    if ((fp = fopen(filename, "rb")) == NULL)
    {
        printf("Error opening file\n");
    }

    size_t res = fread(&fastlog, sizeof(struct Fastlog), 1, fp);
    if (res <= 0) {
        printf("Error reading the file!");
    }
    fclose(fp);
}