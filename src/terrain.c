#include "terrain.h"

#include <stdio.h>
#include <stdlib.h>

#define CHUNKS 9
#define CHUNK_SIZE 128
#define CHUNK_SIZE_1 (CHUNK_SIZE + 1)
#define CHUNK_SIZE_SQ (CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_SIZE_1_SQ (CHUNK_SIZE_1 * CHUNK_SIZE_1)

float noise(float x, float y);

void init_terrain(terrain_t *terrain) {
    u_int32_t indices[CHUNK_SIZE_SQ * CHUNKS * 6];

    for (int n = 0; n < CHUNKS; n++)
        for (int x = 0; x < CHUNK_SIZE; x++)
            for (int z = 0; z < CHUNK_SIZE * 1; z++) {
                int i = ((n * CHUNK_SIZE_SQ) + (x * CHUNK_SIZE + z)) * 6;
                int offset = CHUNK_SIZE_1_SQ * n;

                indices[i + 0] = (x + 0) + (z + 0) * CHUNK_SIZE_1 + offset;
                indices[i + 1] = (x + 1) + (z + 0) * CHUNK_SIZE_1 + offset;
                indices[i + 2] = (x + 0) + (z + 1) * CHUNK_SIZE_1 + offset;
                indices[i + 3] = (x + 1) + (z + 1) * CHUNK_SIZE_1 + offset;
                indices[i + 4] = (x + 0) + (z + 1) * CHUNK_SIZE_1 + offset;
                indices[i + 5] = (x + 1) + (z + 0) * CHUNK_SIZE_1 + offset;
            }

    glGenVertexArrays(1, &terrain->vao);
    glBindVertexArray(terrain->vao);

    // Edges
    glGenBuffers(1, &terrain->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertices
    glGenBuffers(1, &terrain->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, terrain->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * CHUNK_SIZE_1_SQ * CHUNKS, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);

    update_terrain(terrain, (vec3){0, 0, 0});
}

void draw_terrain(terrain_t *terrain) {
    glBindVertexArray(terrain->vao);
    glDrawElements(GL_TRIANGLES, CHUNK_SIZE_SQ * CHUNKS * 6, GL_UNSIGNED_INT, 0);
}

void free_terrain(terrain_t *terrain) {
    glDeleteVertexArrays(1, &terrain->vao);
    glDeleteBuffers(1, &terrain->ebo);
    glDeleteBuffers(1, &terrain->vbo);
}

float noise(float x, float z) {
    return rand() / (float)RAND_MAX;
}

void generate_chunk(terrain_t *terrain, int chunk_x, int chunk_z, int offset) {
    vec3 vertices[CHUNK_SIZE_1 * CHUNK_SIZE_1];

    float min_x = chunk_x * CHUNK_SIZE;
    float min_z = chunk_z * CHUNK_SIZE;

    for (int x = 0; x < CHUNK_SIZE_1; x++)
        for (int z = 0; z < CHUNK_SIZE_1; z++) {
            float x_ = min_x + x;
            float z_ = min_z - z;

            vec3_set(vertices[x + z * CHUNK_SIZE_1], x_, noise(x_, z_), z_);
        }

    glBindBuffer(GL_ARRAY_BUFFER, terrain->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(vertices), sizeof(vertices), vertices);
}

void update_terrain(terrain_t *terrain, vec3 pos) {
    int chunk_x = (int)floor(pos[0] / CHUNK_SIZE);
    int chunk_z = (int)ceil(pos[2] / CHUNK_SIZE);

    if (chunk_x != terrain->center_chunk_x || chunk_z != terrain->center_chunk_z) {
        terrain->center_chunk_x = chunk_x;
        terrain->center_chunk_z = chunk_z;

        for (int x = -1; x <= 1; x++)
            for (int z = -1; z <= 1; z++)
                generate_chunk(terrain, chunk_x + x, chunk_z + z, (x + 1) + (z + 1) * 3);
    }
}
