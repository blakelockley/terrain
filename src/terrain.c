#include "terrain.h"

#include "linmath.h"

void init_terrain(terrain_t *terrain) {
    u_int32_t indices[6] = {0, 1, 2, 3, 2, 1};
    vec3 verices[4] = {
        {-1.0f, 0.0f, -1.0f},
        {-1.0f, 0.0f, +1.0f},
        {+1.0f, 0.0f, -1.0f},
        {+1.0f, 0.0f, +1.0f},
    };

    glGenVertexArrays(1, &terrain->vao);
    glBindVertexArray(terrain->vao);

    // Edges
    glGenBuffers(1, &terrain->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertices
    glGenBuffers(1, &terrain->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, terrain->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verices), verices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
}

void draw_terrain(terrain_t *terrain) {
    glBindVertexArray(terrain->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void free_terrain(terrain_t *terrain) {
    glDeleteVertexArrays(1, &terrain->vao);
    glDeleteBuffers(1, &terrain->ebo);
    glDeleteBuffers(1, &terrain->vbo);
}
