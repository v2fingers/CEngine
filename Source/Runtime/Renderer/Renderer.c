#include "Renderer.h"

void renderer_init(struct renderer *renderer) {
#ifdef DEBUG
  renderer->vkcontext.enablevalidationlayers = true;
#else
  renderer->vkcontext.enablevalidationlayers = false;
#endif
  renderer->vkcontext.physical_device = VK_NULL_HANDLE;
}
