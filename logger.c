#include <stdio.h>

void log_debug(char *function_name, char *msg) {
  printf("\033[0;32;34mDEBUG (%s): %s\033[0m\n", function_name, msg);
}

void log_error(char *function_name, char *msg) {
  printf("\033[0;32;31mERROR (%s): %s\033[0m\n", function_name, msg);
}