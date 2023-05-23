/****************** queue.c file ********************/
int enqueue(PROC **queue, PROC *p) {
  PROC *q = *queue;
  if (q == NULL || p->priority > q->priority) {
    *queue = p;
    p->next = q;
  } else {
    while (q->next && p->priority <= q->next->priority) q = q->next;
    p->next = q->next;
    q->next = p;
  }

  LOG_DEBUG("enqueue P%d", p->pid);

  return 0;
}

// remove and return first PROC in queue
PROC *dequeue(PROC **queue) {
  PROC *p = *queue;
  if (p) *queue = (*queue)->next;
  LOG_DEBUG("dequeue P%d", p->pid);
  return p;
}

void removeProc(PROC **queue, PROC *p) {
  PROC *q = *queue;
  if (q == p) {
    *queue = p->next;
  } else {
    while (q->next != p) q = q->next;
    q->next = p->next;
  }
  LOG_DEBUG("remove P%d", p->pid);
  printList("debug sleepList", sleepList);
}

void printList(char *name, PROC *p) {
  printf("%s = ", name);
  while (p) {
    printf("[%d %d]->", p->pid, p->priority);
    p = p->next;
  }
  printf("NULL\n");
}
