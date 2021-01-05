#ifndef __LAB_H
#define __LAB_H

/* The maximum number of logs that our ring buffer will hold at any one time.*/
#define MAX_LOG_ENTRY 500

/*The maximum length of an individual log entry.*/
#define MAX_MSG_LENGTH 80

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>


typedef enum {
    INFO,
    WARNING,
    ERROR
} LEVEL;

struct FastlogEntry {
    pid_t pid;
    LEVEL level;
    time_t timestamp;
    int exists;
    char text[MAX_MSG_LENGTH];
};

struct Fastlog {
    int current_index; // the next available index to add an entry
    int total_added; //tracks if we have completed a circle, important for tracking the oldest entry before the first lap is completed
    struct FastlogEntry entries[MAX_LOG_ENTRY];
} fastlog;

/**
 * Initialize the buffer for use.
 * We must call this function before calling any other function
 * to ensure we are all ready.
 */
void fastlog_init(void);

/**
 * adjusts the current index of fastlog, keeps
 * fastlog cicular by wrapping the index when necessary
 */
 void increment_index(void);

/**
* Gets the index prior to the current index. This represents the
 * elemnt last added.
*/
int get_prev_index(void);

/**
 * Destroys the fastlog, deleting all entries and freeing its
 * memory
 */
void fastlog_destroy(void);

/**
 * Add an entry into the logger
 * We can assume that text is a properly terminated string.
 * However we can NOT assume that entry is less than MAX_MSG_LENGTH,
 * so we must be careful when copying our data.
 */
void fastlog_write(LEVEL lvl, char *text);

/**
 * Print all valid logs to stderr using the following format:
 * [pid]-[timestamp]-[LEVEL]-<message_text>"
 *
 * NOTE: Logs should be printed from oldest to newest
 */
void fastlog_dump(void);

/**
 * This is a copy of fastlog_dump to be used
 * in mytests. If it was not necessary to preserve
 * the fastlog_dump function for the instructor test suite
 * I would have not duplicated this function.
 */
void fastlog_dump_stdout(void);

/**
 * Write the contents of the fastlog buffer to a binary file
 * named "fastlog.core.<pid>" where <pid> is the current
 * process id. This binary data is the array of entry structs and
 * NOT string data.
 *
 */
void fastlog_core_dump(void);

/**
 * Load the contents of the specified core file into the fastlog
 * buffer, overwriting the existing contents, beginning at
 * index position 0.  Once this binary core has been loaded,
 * the fastlog_dump() function can be used to display the logs
 * in the console.
 *
 */
void fastlog_core_load(const char *filename);




#ifdef __cplusplus
}
#endif
#endif //__LAB_H
