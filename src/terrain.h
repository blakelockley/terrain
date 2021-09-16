#ifndef TERRAIN_H
#define TERRAIN_H

#include "glfw.h"

struct _terrain_t {
    GLuint vao, vbo, ebo;
};

typedef struct _terrain_t terrain_t;

void init_terrain(terrain_t *terrain);
void draw_terrain(terrain_t *terrain);
void free_terrain(terrain_t *terrain);

#endif  // TERRAIN_H
