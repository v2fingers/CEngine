#include "Core/Memory/Memory.h"
#include "Engine.h"

int main() {
  Engine engine;
  engine_init(&engine);
  engine_run(&engine);
  engine_shutdown(&engine);
  mem_print_usage();
  return 0;
}
