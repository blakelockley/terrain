#ifndef TERRAIN_H
#define TERRAIN_H

#include "glfw.h"
#include "linmath.h"

struct _terrain_t {
    int center_chunk_x, center_chunk_z;

    GLuint vao, vbo, ebo;
};

typedef struct _terrain_t terrain_t;

void init_terrain(terrain_t *terrain);
void draw_terrain(terrain_t *terrain);
void free_terrain(terrain_t *terrain);

void update_terrain(terrain_t *terrain, vec3 pos);

#endif  // TERRAIN_H
