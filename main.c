/*********** A Multitasking System ************/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"

int (*fptr[])() = {do_ps,    do_fork,   do_switch, do_exit,
                   do_sleep, do_wakeup, do_wait};

#include "do_cmd.c"
#include "kernel.c"
#include "logger.c"
#include "queue.c"
#include "tree.c"
#include "wait.c"

int findCmd(char *command) {
  char *cmds[] = {"ps", "fork", "switch", "exit", "sleep", "wakeup", "wait", 0};
  for (int i = 0; i < 7; i++) {
    if (strcmp(command, cmds[i]) == 0) return i;
  }
  return -1;
}

void findChildren(PROC *p) {
  PROC *child = p->child;
  printf("children of proc :");
  while (child != NULL) {
    printf("[%d %s]->", child->pid, pstatus[child->status]);
    child = child->sibling;
  }
  printf("NULL\n");
}

void removeZombie(PROC *p) {
  PROC *child = p->child;
  bool hasZombie = false;
  while (child != NULL) {
    if (child->status == ZOMBIE) {
      hasZombie = true;
      printf("proc %d is a zombie, remove it from child list\n", child->pid);
      removeNode(p, child);
      p->status = FREE;
      dequeue(&readyQueue);
      enqueue(&freeList, child);
    }
    child = child->sibling;
  }
  if (hasZombie) {
    printProcessTree(root, 0);
  }
}

int body(void) {
  char command[64];
  printf("proc %d start from body()\n", running->pid);

  int stop = 0;
  do {
    printf("***************************************\n");
    printf("proc %d running: Parent=%d\n", running->pid, running->ppid);
    findChildren(running);

    printList("freeList ", freeList);
    printList("readQueue", readyQueue);

    printf("input a command: [ps|fork|switch|exit] : ");
    fgets(command, 64, stdin);
    command[strlen(command) - 1] = 0;
    int id = 0;
    if (command[0] != 0) {
      id = findCmd(command);
      if (id < 0) {
        printf("invalid command, enter menu for help menu\n");
        continue;
      }

      stop = fptr[id]();
      printProcessTree(root, 0);
      removeZombie(running);
    }
  } while (!(stop > 0));
  return 0;
}

/*************** main() ***************/
int main(void) {
  printf("\nWelcome to Multitasking System\n");
  init();
  initialize();
  printf("P0 fork P1\n");
  kfork(body);

  while (1) {
    if (readyQueue) {
      printf("P0: switch task\n");
      tswitch();
    }
  }
}

/*********** scheduler *************/
int scheduler(void) {
  printf("proc %d in scheduler()\n", running->pid);
  if (running->status == READY) enqueue(&readyQueue, running);
  printList("readyQueue", readyQueue);
  running = dequeue(&readyQueue);
  printf("next running = %d\n", running->pid);
  return 0;
}
