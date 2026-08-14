#include "Engine.h"

void engine_run(struct engine *engine) {
  while (true) {
    engine_update(engine);
  }
}
