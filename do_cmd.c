int do_wait(void) {
  LOG_DEBUG("start do_wait()");
  int status = 0;
  int pid = 0;
  do {
    pid = kwait(&status);
    if (pid < 0) {
      LOG_ERROR("no child");
      return 0;
    }
    printf("proc %d wait for a ZOMBIE child P%d exitCode=%d\n", running->pid,
           pid, status);
  } while (pid != -1);

  return 0;
}
int do_sleep(void) {
  LOG_DEBUG("start do_sleep()");
  if (running->pid == 1) {
    LOG_ERROR("P1 never sleeps");
    return 0;
  }

  int event;
  printf("enter an event value to sleep on : ");
  scanf("%d", &event);
  cleanBuffer();
  ksleep(event);
  return 0;
}

int do_wakeup(void) {
  LOG_DEBUG("start do_wakeup()");
  int event;
  printf("enter an event value to wakeup : ");
  scanf("%d", &event);
  cleanBuffer();
  kwakeup(event);
  return 0;
}

int do_fork(void) {
  int child = kfork(body);
  if (child < 0) {
    LOG_ERROR("kfork failed");
    return -1;
  }
  printf("proc %d kforked a child = %d\n", running->pid, child);
  printList("readyQueue", readyQueue);

  // return child;
  return 0;
}

int do_switch(void) {
  printf("P %d switch task\n", running->pid);
  tswitch();
  printf("P%d resume\n", running->pid);
  return 0;
}

int do_exit(void) {
  int value;
  // PROC *p;
  if (running->pid == 1) {
    LOG_ERROR("P1 never dies");
    return 0;
  }
  printf("P%d in do_exit(), enter an exit value : ", running->pid);
  scanf("%d", &value);
  cleanBuffer();
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