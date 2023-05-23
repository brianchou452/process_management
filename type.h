#ifndef TYPE_H
#define TYPE_H

#define NPROC 9
#define SSIZE 1024

#define FREE 0  // proc status
#define READY 1
#define SLEEP 2
#define ZOMBIE 3

#define __FILENAME__ \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_DEBUG(...)                                                \
  do {                                                                \
    printf("\033[0;32;34mDEBUG @%s/%s():%d ", __FILENAME__, __func__, \
           __LINE__);                                                 \
    printf(__VA_ARGS__);                                              \
    printf("\033[0m\n");                                              \
  } while (0)

#define LOG_ERROR(...)                                                \
  do {                                                                \
    printf("\033[0;32;31mERROR @%s/%s():%d ", __FILENAME__, __func__, \
           __LINE__);                                                 \
    printf(__VA_ARGS__);                                              \
    printf("\033[0m\n");                                              \
  } while (0)

char *pstatus[] = {"FREE   ", "READY  ", "SLEEP  ", "ZOMBIE", "RUNNING"};

typedef struct proc {
  struct proc *next;  // next proc pointer
  int *saved_sp;      // at offset 4: do NOT alter

  int pid;       // pid = 0 to NPROC-1
  int ppid;      // parent pid
  int status;    // PROC status: FREE|READY|etc
  int priority;  // scheduling priority
  int event;     // event value to sleep on
  int exitCode;  // exit code value

  struct proc *child;    // first child PROC pointer
  struct proc *sibling;  // sibling PROC pointer
  struct proc *parent;   // parent PROC pointer

  int kstack[SSIZE];  // processs stack
} PROC;

PROC proc[NPROC];  // NPROC procs
PROC *freeList;    // free list of procs
PROC *readyQueue;  // priority queue of READY procs
PROC *sleepList;   // linked list of SLEEP procs
PROC *running;     // current running proc pointer
PROC *root;        // root PROC pointer
char *log_formate_string;

void tswitch(void);
int do_switch(void);
int do_fork(void);
int do_ps(void);
int do_exit(void);
int do_wait(void);
int do_sleep(void);
int do_wakeup(void);
int kfork(int (*func)(void));
int body(void);

int init(void);
int kexit(int value);
int ksleep(int event);
int kwakeup(int event);
int kwait(int *status);

void printList(char *name, PROC *p);
int enqueue(PROC **queue, PROC *p);
PROC *dequeue(PROC **queue);
void removeProc(PROC **queue, PROC *p);

PROC *createNode(PROC *parent, PROC *node);
void removeNode(PROC *parent, PROC *node);
void printProcessTree(PROC *node, int depth);

void cleanBuffer(void);

#endif