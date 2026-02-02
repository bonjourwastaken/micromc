#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

typedef struct renderer {GLFWwindow* window; unsigned int VBO, VAO, EBO, shader_program, texture;} renderer;
void renderer_create(renderer* renderer);
void renderer_destroy(renderer* renderer);
bool renderer_update(renderer* renderer);
