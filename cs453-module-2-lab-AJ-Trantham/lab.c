/**
 * Name: AJ Trantham
 * Email: ajtrantham@u.boisestate.edu
 * Github Username: AJ-Trantham
 */
#include "lab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>


#define MAX_SIZE 10
#define MAX_STRING 100
#define MAX_DIR_NAME_SIZE 256
// process states
//enum states {R, S, D, Z, T, TRACING, PAGING, DEAD,};

int countRefresh = 5;

struct process ** create_procs_array(void) {
    struct process **pointers = (struct process**) calloc(sizeof(struct process*) * MAX_PROCESSES, sizeof(struct process));
    return pointers;
}

void destroy_procs_array(struct process ** procs) {
    int i = 0;
    while(i < MAX_PROCESSES && procs[i] != NULL) {
        destroyProcess(procs[i]);
        procs[i] = NULL;
        i++;
    }
    free(procs);
}

void get_all_procs(struct process **procs, char * filePath) {
    // open proc dir
    DIR *dir; // directory
    struct dirent *de; //directory entry
    if (!filePath) {
        filePath = "/proc/";
    }
    //TODO: change so user doesn't need to put slash at the end
    //char slash[2] = "/";
    //printf("%s", slash);
//    strncat(filePath, slash, strlen(slash));
    //printf("%s",filePath);
    char file[10] = "/stat";
    int i = 0;

    // read all the files - basically need to store the numbers to get all the proc entries
    dir = opendir(filePath);
    if (dir != NULL) {
        while ((de = readdir(dir)) && i < MAX_PROCESSES) {
            int dName = atoi(de->d_name);
            if (dName && de->d_type == DT_DIR) {
                // I have a process - construct its path
                size_t pathLen = strlen(de->d_name) + strlen(filePath) + strlen(file) + 1;
                char *path = malloc(sizeof(char) * pathLen);
                strncpy(path, filePath, strlen(filePath) + 1); // +1 to account for null term
                strncat(path, de->d_name, strlen(de->d_name));
                strncat(path, file, strlen(file));
                // add process struct to storage mechanism
                procs[i] = readProc(path);
                free(path);
                i++;
            }
        }
        closedir(dir);
    } else {
        perror("Couldn't open directory!");
    }
}

// I could also just collect the the data here and build the process using my createProc function
struct process * readProc(char *filename) {
    // open /proc/pid/stat
    struct process *p = malloc(sizeof(struct process));
    FILE *fp = fopen(filename,"r");
    if (fp) {
        char *line = NULL;
        size_t len = 0;
        int charsRead = getline(&line, &len, fp);
        if (charsRead != -1) {
            char *tok = strtok(line, " ");
            int tokNum = 1;
            while(tok != NULL) {
                switch (tokNum) {
                    case 1:
                        p->pid = atoi(tok);
                        break;
                    case 2:
                        p->comm = strndup(tok, MAX_STRING);
                        break;
                    case 3:
                        p->state = *tok;
                        break;
                    case 14:
                        p->utime = strtol(tok, NULL, 0);
                        break;
                    case 15:
                        p->stime = strtol(tok, NULL, 0);
                        break;
                    case 39:
                        p->processor = atoi(tok);
                        break;
                }
                tokNum++;
                tok = strtok(NULL, " ");
            }
            free(line);
        } else {
            perror("File is empty\n");
        }
        fclose(fp);
    } else {
        perror("Error reading process. Proc file does not exits\n");
    }

    return p;
}

void destroyProcess(struct process *p) {
    free(p->comm);
    p->comm = NULL;
    free(p);
}

void printProcess(struct process p) {
    printf("%d\t", p.pid);
    printf("%s\t", p.comm);
    printf("%c\t", p.state);
    printf("%ldl\t", p.utime);
    printf("%ldl\t", p.stime);
    printf("%d\t", p.processor);
    printf("\n");
}

void print_pid_order(struct process ** procs) {
    printf("Printing Procs in PID order: \n");
    int i = 0;
    while(i < MAX_PROCESSES && procs[i] != NULL) {
        printProcess(*procs[i]);
        i++;
    }
}

static int comparator(const void * p, const void * q) {
    char *pComm;
    char *qComm;
    if (!p || !q) {
        return 0;
    }
    pComm = strdup(((struct process *) p)->comm);
    qComm = strdup(((struct process *) q)->comm);
    if (!pComm || !qComm) {
        return 0;
    }
    int res = strcmp(pComm, qComm);
    free(pComm);
    free(qComm);
    return res;
}

void print_comm_order(struct process ** procs) {

    // make an array that holds all the processes
    //struct process * processes = malloc(sizeof(struct process *) * MAX_PROCESSES);
    struct process processes[MAX_PROCESSES];
    int i = 0;
    while(i < MAX_PROCESSES && procs[i] != NULL) {
        processes[i] = *procs[i];
        i++;
    }

    // sort these processes in place
    qsort(processes, i, sizeof(struct process), comparator);

    // print the array
    int j = 0;
    for (; j < i; j++) {
        printProcess(processes[j]);
    }
}

void print_zombies(struct process ** procs) {
    printf("Printing Zombie Procs: \n");
    int i = 0;
    while(i < MAX_PROCESSES && procs[i] != NULL) {
        if (procs[i]->state == 'Z') {
            printProcess(*procs[i]);
        }
        i++;
    }
}

void sig_handler(int signum) {
    //printf("-----------------------Time to run again! - handler-------------------------- \n");
    alarm(5); // reschedule
    // create a storage mechanism for my procs
    struct process ** procs = create_procs_array();
    // get all procs and stores in procs
    get_all_procs(procs, NULL);

    printf("------------------------------------------------\n");
    print_pid_order(procs);
    destroy_procs_array(procs);
    countRefresh--;
}

int testmain(int argc, char **argv)
{
    // parse cmd line options
    int pidOrder = 0;
    int commOrder = 0;
    int zombies = 0;
    int test = 0;
    char *userDir = NULL;
    int c;
    opterr = 0;
    optind = 1;

    while ((c = getopt(argc, argv, "pcztd:")) != -1)
        switch (c)
        {
            case 'p':
                pidOrder = 1;
                break;
            case 'c':
                commOrder = 1;
                break;
            case 'z':
                zombies = 1;
                break;
            case 't':
                test = 1;
                break;
            case 'd':
                userDir = optarg;
                break;
            case '?':
                if (optopt == 'c')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
                return 1;
            default:
                abort ();
        }

    // create a storage mechanism for my procs
    struct process ** procs = create_procs_array();
    // get all procs and stores in procs
    get_all_procs(procs, userDir);

    if (argc > 1) {
        // consider the input param and sort the data appropriately
        if (pidOrder) {
            print_pid_order(procs);
        }
        if (commOrder) {
            print_comm_order(procs);
        }
        if (zombies) {
            print_zombies(procs);
        }
        if (test) {
            print_pid_order(procs);
            destroy_procs_array(procs);
            signal(SIGALRM, sig_handler);
            alarm(5);
            while(countRefresh >= 0) {

            }
            return 0;
        }
    } else { // no cmd line args given
        print_pid_order(procs);
        destroy_procs_array(procs);
        signal(SIGALRM, sig_handler);
        alarm(5);

        // wait for alarm
        while (1) {

        }

    }

    // destroy  storage mechanism
    destroy_procs_array(procs);

    return 0;
}