#include "terrain.h"

#define CHUNK_SIZE 16
#define CHUNK_SIZE_1 (CHUNK_SIZE + 1)

void init_terrain(terrain_t *terrain) {
    u_int32_t indices[CHUNK_SIZE * CHUNK_SIZE * 6];

    for (int x = 0; x < CHUNK_SIZE; x++)
        for (int z = 0; z < CHUNK_SIZE; z++) {
            int i = (x * CHUNK_SIZE + z) * 6;
            indices[i + 0] = (x + 0) + (z + 0) * CHUNK_SIZE_1;
            indices[i + 1] = (x + 1) + (z + 0) * CHUNK_SIZE_1;
            indices[i + 2] = (x + 0) + (z + 1) * CHUNK_SIZE_1;
            indices[i + 3] = (x + 1) + (z + 1) * CHUNK_SIZE_1;
            indices[i + 4] = (x + 0) + (z + 1) * CHUNK_SIZE_1;
            indices[i + 5] = (x + 1) + (z + 0) * CHUNK_SIZE_1;
        }

    vec3 vertices[CHUNK_SIZE_1 * CHUNK_SIZE_1];

    for (int x = 0; x < CHUNK_SIZE_1; x++)
        for (int z = 0; z < CHUNK_SIZE_1; z++)
            vec3_set(vertices[x + z * CHUNK_SIZE_1], x, 0.0f, -z);

    glGenVertexArrays(1, &terrain->vao);
    glBindVertexArray(terrain->vao);

    // Edges
    glGenBuffers(1, &terrain->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertices
    glGenBuffers(1, &terrain->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, terrain->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
}

void draw_terrain(terrain_t *terrain) {
    glBindVertexArray(terrain->vao);
    glDrawElements(GL_TRIANGLES, CHUNK_SIZE * CHUNK_SIZE * 6, GL_UNSIGNED_INT, 0);
}

void free_terrain(terrain_t *terrain) {
    glDeleteVertexArrays(1, &terrain->vao);
    glDeleteBuffers(1, &terrain->ebo);
    glDeleteBuffers(1, &terrain->vbo);
}
