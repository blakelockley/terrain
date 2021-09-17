#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glfw.h"
#include "linmath.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define ENGINE_INCLUDES
#include "shader.h"
#include "terrain.h"

GLFWwindow *window;

vec3 pos = {0.0f, 20.0f, 5.0f}, dir = {0.0f, 0.0f, 0.0f};

void init();
void deinit();

int main() {
    init();

    char title[16];

    double time_elapsed = 0, last_second = 0;
    int frames = 0;

    terrain_t terrain;
    init_terrain(&terrain);

    int shader = load_shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)) {
        double current_time = glfwGetTime();
        double delta = current_time - time_elapsed;
        time_elapsed = current_time;

        frames++;
        if (current_time - last_second > 1.0) {
            double fps = frames / (current_time - last_second);

            sprintf(title, "FPS: %.2f", fps);
            glfwSetWindowTitle(window, title);

            frames = 0;
            last_second = current_time;
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4x4 view, projection;

        vec3 tmp, ahead;
        vec3_scale(tmp, dir, delta * 20.0f);
        vec3_add(pos, tmp, pos);

        vec3_add(ahead, pos, (vec3){0.0f, -0.5f, -1.0f});
        mat4x4_look_at(view, pos, ahead, (vec3){0, 1, 0});

        mat4x4_perspective(projection, 45.0f, (float)width / (float)height, 0.1f, 100.0f);

        GLint view_loc = glGetUniformLocation(shader, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float *)view);

        GLint projection_loc = glGetUniformLocation(shader, "projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)projection);

        draw_terrain(&terrain);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deinit();
    return EXIT_SUCCESS;
}

void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        dir[2] = -1.0f;

    if (key == GLFW_KEY_W && action == GLFW_RELEASE && dir[2] < 0)
        dir[2] = 0.0f;

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        dir[2] = 1.0f;

    if (key == GLFW_KEY_S && action == GLFW_RELEASE && dir[2] > 0)
        dir[2] = 0.0f;

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        dir[0] = -1.0f;

    if (key == GLFW_KEY_A && action == GLFW_RELEASE && dir[0] < 0)
        dir[0] = 0.0f;

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        dir[0] = 1.0f;

    if (key == GLFW_KEY_D && action == GLFW_RELEASE && dir[0] > 0)
        dir[0] = 0.0f;

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        dir[1] = 1.0f;

    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE && dir[1] > 0)
        dir[1] = 0.0f;

    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        dir[1] = -1.0f;

    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE && dir[1] < 0)
        dir[1] = 0.0f;
}

void init() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "GLFW Window", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);

    glClearColor(0.2f, 0.3f, 1.0f, 1.0f);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void deinit() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
