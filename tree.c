

void initialize() {
  root = running;
  root->parent = root;
  root->child = NULL;
  root->sibling = NULL;
  running = root;
}

PROC *createNode(PROC *parent, PROC *node) {
  // PROC *node = malloc(sizeof(PROC));
  // strcpy(node->name, name);
  //  node->type = type;
  node->parent = parent;
  node->child = NULL;
  node->sibling = NULL;

  if (parent->child == NULL) {
    parent->child = node;
  } else {
    PROC *temp = parent->child;
    while (temp->sibling != NULL) {
      temp = temp->sibling;
    }
    temp->sibling = node;
  }

  return node;
}

void removeNode(PROC *parent, PROC *node) {
  if (parent->child == node) {
    parent->child = node->sibling;
  } else {
    PROC *temp = parent->child;
    while (temp->sibling != node) {
      temp = temp->sibling;
    }
    temp->sibling = node->sibling;
  }
}

void printProcessTree(PROC *node, int depth) {
  printf("\033[0;32;34m");
  if (node == NULL) return;
  for (int i = 0; i < depth; i++) {
    printf("    ");
  }
  printf("├─ P%d: %s\n", node->pid,
         node == running ? "RUNNING" : pstatus[node->status]);
  printProcessTree(node->child, depth + 1);
  printProcessTree(node->sibling, depth);
  printf("\033[0m");
}