#include "Engine.h"
#include <stdio.h>

int main(int argc, const char **argv) {
  struct engine engine;
  engine_init(&engine);
  engine_run(&engine);
  engine_shutdown(&engine);

  printf("Done\n");
  getchar();
  return 0;
}
