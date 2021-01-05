#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "lab.h"

/**
 * Fills the buffer with num_samples samples
 * @param num_samples
 * @return
 */
int fill_with_sample_data(int num_samples) {

    printf("\n");
    printf("Fill with %d samples\n", num_samples);

    int i;
    assert(num_samples > 0);
    char output_str[MAX_MSG_LENGTH * MAX_LOG_ENTRY * 10] = "";
    for(i=0; i < num_samples; i++) {
        // create text to write to log
        int length = snprintf( NULL, 0, "%d", i );
        char* str = malloc( length + 1 );
        snprintf( str, length + 1, "%d", i );
        char text[MAX_MSG_LENGTH] = "text\0";
        strncat(text,str, sizeof(text) - 1);
        fastlog_write(i%3,text); // mod 3 is to match the 3 enum values

        // the expected result is only the last MAX_LOG_ENTRY values
        if (i > (num_samples - MAX_LOG_ENTRY-1)) {
            char expected_message[MAX_MSG_LENGTH] ="";
            switch (i%3) {
                case 0:
                    strncat(expected_message, "x-x-INFO-", sizeof(expected_message)-1);
                    break;
                case 1:
                    strncat(expected_message, "x-x-WARNING-", sizeof(expected_message)-1);
                    break;
                case 2:
                    strncat(expected_message, "x-x-ERROR-", sizeof(expected_message)-1);
                    break;
            }

            strncat(text,"\n", sizeof(text)-1);
            strncat(expected_message, text,sizeof(expected_message)-1);
            strncat(output_str, expected_message, sizeof(output_str)-1);
        }
        free(str);
        }
    printf("Expected Output:\n%s \n", output_str);

    return 0;
}

/**
 * Test the circular buffer when filled to 1.5 capacity
 */
void test_over_fill_buffer(void) {
    printf("Testing: overfill_buffer\n");
    fastlog_init();
    fill_with_sample_data(MAX_LOG_ENTRY * 1.5);
    printf("Actual Output: \n");
    fastlog_dump_stdout();
    fastlog_destroy();
}

/**
 * Test the circular buffer when filled full
 */
void test_fill_buffer(void) {
    printf("Testing: fill_buffer\n");
    fastlog_init();
    fill_with_sample_data(MAX_LOG_ENTRY);
    printf("Actual Output: \n");
    fastlog_dump_stdout();
    fastlog_destroy();
}

/**
 * Tests inserting null values
 */
void test_try_null(void) {
    printf("Testing: try_null\n");
    fastlog_init();
    fastlog_write(WARNING, NULL);
    printf("Fill w/ 1 Null\n");
    fill_with_sample_data(1);
    fastlog_write(ERROR, NULL);
    printf("Fill w/ 1 more Null\n");
    printf("Actual Output: \n");
    fastlog_dump_stdout();
    fastlog_destroy();
}

/**
 * Tests to make sure only the valid entries are
 * printed when buffer is not full
 */
void test_not_full(void) {
    printf("Testing: not_full");
    fastlog_init();
    fill_with_sample_data(MAX_LOG_ENTRY/2);
    printf("Actual Output: \n");
    fastlog_dump_stdout();
    fastlog_destroy();
}


void test_core_dump_and_load(void) {
    printf("Testing: core_dump and core_load\n");
    fastlog_init();
    fill_with_sample_data(MAX_LOG_ENTRY);
    printf("Fastlog before Saving: \n");
    fastlog_dump();
    //save fastlog to file
    fastlog_core_dump();

    int  pid = getpid();
    char filename[30] = "fastlog.core.";
    int length = snprintf( NULL, 0, "%d", pid);
    char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", pid );
    strncat(filename, str, sizeof(filename)-1);
    // restore fastlog from file
    fastlog_core_load(filename);
    printf("Fastlog after reloading file: \n");
    fastlog_dump();
    fastlog_destroy();
    free(str);
}

void test_long_mess_len(void) {
    printf("Testing: test_long_mess_len\n");
    fastlog_init();
    int i;
    char test_str[MAX_MSG_LENGTH+1];
    for (i = 0; i < MAX_MSG_LENGTH +1; i++) {
        test_str[i] = 'a';
    }
    test_str[i] = '\0';
    fastlog_write(INFO, test_str);
    printf("Actual Output: \n");
    fastlog_dump_stdout();
    fastlog_destroy();
}

/**
 * This function tests the fast logger
 * @return
 */
int main(void)
{
    test_fill_buffer();
    test_try_null();
    test_not_full();
    test_core_dump_and_load();
    test_over_fill_buffer();
    test_long_mess_len();

    return 0;
}
