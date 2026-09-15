#include "Core/Memory/Memory.h"
#include "Engine.h"

int main() {
  Engine engine;
  engine_init(&engine);
  engine_run(&engine);
  mem_print_usage();
  engine_shutdown(&engine);
  return 0;
}
