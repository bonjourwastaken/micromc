#include "world.h"
#include "cglm/types.h"
#include <stdlib.h>

static void generate_chunk(chunk* chunk, int seed, int chunk_i, int chunk_j) {
    for(int x = 0; x < 16; x++) {
        for(int z = 0; z < 16; z++) {
            for(int y = 0; y < 256; y++) {
                (*chunk)[x][y][z] = BLOCK_AIR;
            }
        }
    }
    // dirt floor
    for(int x = 0; x < 16; x++) {
        for(int z = 0; z < 16; z++) {
            (*chunk)[x][0][z] = BLOCK_DIRT;
        }
    }
}

world world_create(int seed, int size) {
    world world;
    world.chunk_count = size * size * 4;
    world.chunk_coords = malloc(sizeof(ivec2) * world.chunk_count);
    world.chunks = malloc(sizeof(chunk) * world.chunk_count);
    unsigned int chunks_generated = 0;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            generate_chunk(&(world.chunks[chunks_generated]), seed, i, j);
            world.chunk_coords[chunks_generated][0] = i;
            world.chunk_coords[chunks_generated][1] = j;
            chunks_generated++;
            generate_chunk(&(world.chunks[chunks_generated]), seed, -1 * i, j);
            world.chunk_coords[chunks_generated][0] = -1 * i;
            world.chunk_coords[chunks_generated][1] = j;
            chunks_generated++;
            generate_chunk(&(world.chunks[chunks_generated]), seed, i, -1 * j);
            world.chunk_coords[chunks_generated][0] = i;
            world.chunk_coords[chunks_generated][1] = -1 * j;
            chunks_generated++;
            generate_chunk(&(world.chunks[chunks_generated]), seed, -1 * i, -1 * j);
            world.chunk_coords[chunks_generated][0] = -1 * i;
            world.chunk_coords[chunks_generated][1] = -1 * j;
            chunks_generated++;
        }
    }
    return world;
}

void world_destroy(world *world) {
    free(world->chunk_coords);
    free(world->chunks);
    world->chunk_count = 0;
}
