#include <stdlib.h>

#include "renderer.h"

int main(void) {
    renderer renderer;
    renderer_create(&renderer);

    while(renderer_update(&renderer)) {

    }

    renderer_destroy(&renderer);
    return EXIT_SUCCESS;
}
