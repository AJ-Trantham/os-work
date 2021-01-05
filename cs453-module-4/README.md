# Overview

In this assignment, you will develop a simple circular buffer library
that will allow fast debug logging for all of your applications. The
[Wikipedia article](https://en.wikipedia.org/wiki/Circular_buffer) is
a good article that illustrates the concept of circular buffers. It
would be advantageous to read through it before you start coding.

The buffer used in fastlog will be a specialized version of the more
general circular buffer. We will maintain only one index for writing
log entries and reset the index to 0 when we are at the last entry. In
other words, if we have a standard array of length 6 (yielding indexes
0-5) when we reach the end of the array (index 5) we wrap back around
to index 0 and write the data. We do not provide an option to remove
an entry from the buffer. When the program starts up we will allocate
a fixed number of entries, initialize the buffer and start accepting
calls.

At first glance this may seem like a bad idea. The circular buffer
will overwrite previous entries, so how is this a good thing? The idea
behind a circular buffer in OS programming is speed. We want a fast
data structure that the application can write log entries into for
debugging purposes. A circular buffer is only a snapshot of the final
few moments of a process's life. However, that snapshot is usually
enough to give the developer an idea of where to start looking for a
potential problem.

All the starter code is provided for you in your Git repo. The
semantics of each method is detailed within the header file provided
(lab.h).

## Learning Objectives

- Implement a common Operating system data structure
- Use System calls getpid() clock_gettime()
- String handling
- Write to persistent storage (disk)

## Requirements

Implement and test all functions that are described in lab.h. You will
need to use the system calls **getpid** and **clock_gettime** to
output the correct log message.

When you save your logs to a file you need to use a binary format not
a text format (Hint: you can save your struts directly to disk)!!

## Testing Hints

Below are some things you should think about when testing your
library.  This list is not complete it is just a place to jump start
your ideas your testing should be more robust.

- What happens if you try and log a NULL string?
  - HINT: logging a NULL string is valid and should not cause a crash
- What happens if you try and log a string that is too long?
  - HINT: The provided header file specifies the maximum string allowed.
- Logs should be displayed (dumped to the console) from the oldest to the newest.
  - HINT: This isn't necessarily from index 0 to MAX_LOG_ENTRY – 1
    because we are using a circular buffer.
- What happens if the index counter, which is an int, overflows?
  - Read about [Therac-25](https://en.wikipedia.org/wiki/Therac-25)
    which is an example of poor programming that caused people to die.

## Grading Rubric

All grading will be executed on onyx.boisestate.edu Submissions that fail 
to compile will not being graded.

- [30 pts] Instructor private test suite for **lab.c**
  - Score will reflect number of tests passed divided by the total number of tests
- [10 pts] Code quality for **lab.c**
  - [4 pts] Code is formatted correctly and follows a consistent style
  - [2 pts] Code is commented when necessary
  - [4 pts] There is more than just the main function in **mytests.c**
- [15 pts] Compiler warnings **ALL files**
  - Each compiler warning will result in a 3 point deduction.
  - You are not allowed to suppress warnings
  - You must build with the flags ```-Wall -Wextra -Wpointer-arith -Wstrict-prototypes -std=gnu89``` 
- [30 pts] Code coverage for **lab.c** (student authored).
  - As reported by **runcov.sh**
  - Grade will be calculated using the formula **min((%cov/80),1) * 30.** 
    - example: **runcov.sh** reports 50% coverage - min((50/80),1) * 30 = 18.75
    - example: **runcov.sh** reports 70% coverage - min((70/80),1) * 30 = 26.25
    - example: **runcov.sh** reports 80% coverage - min((80/80),1) * 30 = 30
    - example: **runcov.sh** reports 90% coverage - min((90/80),1) * 30 = 30
  - **lab.c** is the only file that is checked for code coverage
- [10 pts] Valgrind reports no memory leaks or read/write errors
  - As reported by **runval.sh**
  - This is a **PASS/FAIL** score. 1 read/write error or leaking 1
    byte will result in a zero for this section. There is no partial
    credit for this section.
- [5 pts] In the file lab.c you updated the header on lines 2,3, and 4

## Submission

You can make as many commits to your git repository as you want. When
I grade I will use the top commit as what you are submitting for
credit.

Pushing your code for grading

- git add lab.c
- git add mytests.c
- git commit -am "Finished the lab"
- git push

## AJ Learning Notes

Remember you can't just set a char array to a char*. This is tempting to do when
the **char[]** is part of a struct: **s.str = string** But if struct **s {char str[10]}** 
this will not work at all. Because **strncpy** doesn't require the NUL terminator
it can be susceptible to a variety of exploits if the NUL term is not added. Either define a macro that does 
add the NUL term every time or use strncat. https://devblogs.microsoft.com/oldnewthing/20050107-00/?p=36773