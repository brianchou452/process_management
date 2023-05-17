int do_wait(void) {
  printf("do_wait() : wait for a ZOMBIE child\n");
  return 0;
};
int do_sleep(void) {
  printf("do_sleep() : sleep for a few seconds\n");
  return 0;
};
;
int do_wakeup(void) {
  printf("do_wakeup() : wakeup a sleeping proc\n");
  return 0;
};

int do_fork(void) {
  int child = kfork(body);
  if (child < 0)
    printf("kfork failed\n");
  else {
    printf("proc %d kforked a child = %d\n", running->pid, child);
    printList("readyQueue", readyQueue);
  }
  return child;
}

int do_switch(void) {
  printf("proc %d switch task\n", running->pid);
  tswitch();
  printf("proc %d resume\n", running->pid);
  return 0;
}

int do_exit(void) {
  int value;
  // PROC *p;
  if (running->pid == 1) {
    printf("P1 never dies\n");
    return 0;
  }
  printf("proc %d in do_exit(), enter an exit value : ", running->pid);
  scanf("%d", &value);
  kexit(value);
  return 0;
}

int do_ps(void) {
  int i;
  PROC *p;
  printf("pid   ppid    status\n");
  printf("--------------------\n");
  for (i = 0; i < NPROC; i++) {
    p = &proc[i];
    printf(" %d      %d     ", p->pid, p->ppid);
    if (p == running)
      printf("%s\n", pstatus[4]);
    else
      printf("%s\n", pstatus[p->status]);
  }
  return 0;
}