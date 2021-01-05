# Overview

This lab is a warm up for the rest of the class. C style strings are
very different than strings in other languages and are a common source
of bugs. In this lab you will author a small handful of functions that
mirror functions in the standard library. In addition to refreshing
your C programming skills this lab will also ensure that you have all
the correct tool chains installed and you are able to build a small
program on onyx.boisestate.edu.

## Learning Objectives

- Review C style strings
- Review pointers
- Review malloc/free
- Review header files and implementations files
- Make sure you can compile/run/debug your code

## Requirements

In your git repository you will find a header file named lab.h with
detailed documentation for a set of function prototypes. Your job is
to implement all the functions declared in lab.h in the file named
lab.c and write tests that exercise your code in the file mytests.c.

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
- git commit -am “Finished lab”
- git push

## AJ's Learning Notes:
- Valgrind write error occurs when a program tries to write to memory where it 
shouldn't. 
- Calloc: like malloc but sets memory to 0 bytes
- Difference between size_t and int:
- Remember C functions need void explicitly set in there parameters if they don't have any
- represents the size in bytes of an object
