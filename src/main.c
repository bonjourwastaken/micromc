#include <stdlib.h>

#include "renderer.h"
#include "world.h"

int main(void) {
    renderer renderer;
    renderer_create(&renderer);
    world world = world_create(0, 4);

    while(renderer_update(&renderer, &world)) {

    }

    world_destroy(&world);
    renderer_destroy(&renderer);
    return EXIT_SUCCESS;
}
