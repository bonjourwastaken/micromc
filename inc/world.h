#pragma once

#include <cglm/cglm.h>

/* World architecture: (x, y, z) coordinates; y is the vertical axis
    World made up of 16x256x16 chunks; one drawcall per chunk
    Chunks are threedimensional arrays (fixed size) of 'block' enums (air has ID
   0, dirt has ID 1) Chunks are stored in the dynamically allocated 'chunks'
   field in the 'world' structure, the order is identical to the order of the
   corresponding chunk coordinates in the 'chunk_coords' field
*/

typedef enum block {
	BLOCK_AIR = 0,
	BLOCK_DIRT = 1,
} block;

typedef block chunk[16][256][16];
typedef struct world {
	ivec2* chunk_coords;
	chunk* chunks;
	unsigned int chunk_count;
} world;

world world_create(int seed, int size);
void world_destroy(world* world);
