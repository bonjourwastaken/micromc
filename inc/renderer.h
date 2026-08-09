#pragma once

#define GLFW_INCLUDE_NONE
#include "world.h"
#include <GLFW/glfw3.h>
#include <glad/gl.h>

typedef struct renderer {
	GLFWwindow* window;
	unsigned int VBO, VAO, EBO, shader_program, texture;
} renderer;
void renderer_create(renderer* renderer);
void renderer_destroy(renderer* renderer);
bool renderer_update(renderer* renderer, world* world);
