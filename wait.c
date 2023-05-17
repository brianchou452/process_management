
// 1. Modify kfork() to implement process tree as a BINARY tree
//    For the running process, print its children list, as in
//    proc 1 running, parent=0, child=[pid, status]->[ ]->...-> NULL
//      Section 3.5.2: process family tree

// In init(): Set P0's child = 0, sibling = 0

// In kfork():

// PROC *p = dequeue(&freeList);
// initialize p's status, priority, ppid as before
// Implement process BINARY tree:
// p->child = 0;
// p->sibling = 0;
// p->parent = running;
// insert p to END of running's child list
// if (running->child == 0)
//   running->child = p;
// else {
//   PROC *q = running->child;
//   while (q->sibling) q = q->sibling;
//   q->sibling = p;
// }

// Write your own code to print child list of a process.

// 2. Implement ksleep() as the algorithm in 3.4.1
// int ksleep(int event) {
//   // Algorithm 3.4.1
// }

// // 3. Implement kwakeup as the algorithm in 3.4.2
// int kwakeup(int event) {
//   // Algorithm 3.4.2
// }

/******* test sequence **********
1. startup
2. P1: sleep: Reject, P1 should never sleep by the sleep command
       fork; switch ==> fork P2, switch to P2
3. P2: sleep:  enter a value 123 ==> P2 should be SLEEP in sleepList
4. P1: wakeup: enter a value 124: should NOT wakeup P2
       wakeup: enter a value 123: should wakeup P2 ==> P2 in readyQueue
*******************************/

// 4. Modify kexit(), give away children to P1, becomes ZOMBIE, wakeup parent:
// ensure P1 never exit !!!!
// int kexit(int exitValue) {
//   // Algorithm 3.5.1
// }

// 5. Implement kwait() as the algorithm in 3.5.3
// int kwait(int *status) {
//   // Algorithm 3.5.3
// }

// 6. Add a "wait" command to let proc wait for ZOMBIE child
// Test the wait command to verify YOUR kwait() works
/*************** test sequence *********************
1. startup: P0 switch to P1
2. P1: fork; switch
3. P2: fork; fork; exit ==> P3, P4 are children of P2; P2 exit ==> switch to P1
4. P1: MUST show P2, P3, P4 are children of P1, P2 is a ZOMBIE
       wait: ==> P1 finds a ZOMBIE child P2 and set it FREE
       wait: ==> P1 will SLEEP in kwait() ==> switch to P3
5. P3: wait: no child error
       exit: P3 becomes a ZOMBIE and wakeup P1, but P4 will run
6. P4: exit: P4 becomes a ZOMBIE ==> switch to P1
7. P1: clean up ZOMBIE child p3
       wait: clean up ZOMBIE P4
**************************************************/
