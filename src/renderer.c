#include "renderer.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

const unsigned int RESOLUTION_X = 800;
const unsigned int RESOLUTION_Y = 600;
const char* APP_NAME = "micromc";

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f,
};

const char vert_shader_src[] = {
#embed "shader.vert"
,'\0'
};
const char* pvertsrc = (const char*)vert_shader_src;
const char frag_shader_src[] = {
#embed "shader.frag"
,'\0'
};
const char* pfragsrc = (const char*)frag_shader_src;

void renderer_create(renderer *renderer) {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    renderer->window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, APP_NAME, nullptr, nullptr);
    glfwMakeContextCurrent(renderer->window);
    gladLoadGL(glfwGetProcAddress);
    glViewport(0, 0, RESOLUTION_X, RESOLUTION_Y);

    glGenBuffers(1, &renderer->VBO);
    glGenVertexArrays(1, &renderer->VAO);

    int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &pvertsrc, nullptr);
    glCompileShader(vert_shader);
    int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &pfragsrc, nullptr);
    glCompileShader(frag_shader);
    renderer->shader_program = glCreateProgram();
    glAttachShader(renderer->shader_program, vert_shader);
    glAttachShader(renderer->shader_program, frag_shader);
    glLinkProgram(renderer->shader_program);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    glBindVertexArray(renderer->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void renderer_destroy(renderer *renderer) {
    glfwDestroyWindow(renderer->window);
    glfwTerminate();
}

bool renderer_update(renderer* renderer) {
    glfwPollEvents();

    glClearColor(135.0f/255.0f, 206.0f/255.0f, 235.0f/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderer->shader_program);
    glBindVertexArray(renderer->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(renderer->window);
    return !glfwWindowShouldClose(renderer->window);
}
