int init(void) {
  int i;
  for (i = 0; i < NPROC; i++) {
    proc[i].pid = i;
    proc[i].status = FREE;
    proc[i].priority = 0;
    proc[i].next = (PROC *)&proc[(i + 1)];
  }
  proc[NPROC - 1].next = 0;

  freeList = &proc[0];
  readyQueue = 0;

  // create P0 as the initial running process
  running = dequeue(&freeList);
  running->ppid = 0;
  running->status = READY;
  running->priority = 0;
  running->parent = running;
  printList("freeList", freeList);
  printf("init complete: P0 running\n");
  return 0;
}

int ksleep(int event) {
  // Algorithm 3.4.1
}

// 3. Implement kwakeup as the algorithm in 3.4.2
int kwakeup(int event) {
  // Algorithm 3.4.2
}
int kwait(int *status) {
  // Algorithm 3.5.3
}

/*******************************************************
kfork() creates a child process; returns child pid.
When scheduled to run, child PROC resumes to func();
********************************************************/
int kfork(int (*func)(void)) {
  PROC *p;
  int i;
  /*** get a proc from freeList for child proc: ***/
  p = dequeue(&freeList);
  if (!p) {
    printf("no more proc\n");
    return (-1);
  }

  /* initialize the new proc and its stack */
  p->status = READY;
  p->priority = 1;  // for ALL PROCs except P0
  p->ppid = running->pid;
  p->parent = running;

  //                                                    proc.ksp
  //                                                       |
  //                    -1   -2  -3  -4  -5  -6  -7  -8   -9
  // kstack contains: |retPC|eax|ebx|ecx|edx|ebp|esi|edi|eflag|
  for (i = 1; i < 10; i++)  // zero out kstack cells
    p->kstack[SSIZE - i] = 0;

  p->kstack[SSIZE - 1] = (intptr_t)func;  // retPC -> func()
  p->saved_sp = &(p->kstack[SSIZE - 9]);  // PROC.ksp -> saved eflag

  enqueue(&readyQueue, p);

  createNode(running, p);

  return p->pid;
}

int kexit(int value) {
  if (running->pid == 1) {
    printf("P1 never dies\n");
    return 0;
  } else if (running->child != NULL) {
    PROC *p = running->child;
    while (p) {
      p->ppid = 1;
      p->parent = &proc[1];
      // p->status = ZOMBIE;
      p = p->sibling;
    }
    PROC *p1 = &proc[1];
    if (p1->child == NULL) {
      p1->child = running->child;
    } else {
      p = p1->child;
      while (p->sibling != NULL) {
        p = p->sibling;
      }
      p->sibling = running->child;
    }
    running->child = NULL;
  }

  running->exitCode = value;
  running->status = ZOMBIE;
  running->priority = 0;
  // enqueue(&freeList, running);
  // printList("freeList", freeList);
  // removeNode(running->parent, running);
  tswitch();
  return 0;
}
