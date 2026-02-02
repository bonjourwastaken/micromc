#include "renderer.h"

#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/util.h>
#include <cglm/vec3.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const unsigned int RESOLUTION_X = 800;
const unsigned int RESOLUTION_Y = 800;
const char* APP_NAME = "micromc";

// First three entries are the vertex coordinates, the last two are the texture coordinates
float vertices[] = {
    // Front
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // bottom right
    1.0f, -1.0f, 1.0f, 1.0f, 0.0f, // top right
   -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // top left
   -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // bottom left
   // Back
    1.0f, 1.0f, -1.0f, 0.0f, 1.0f, // bottom left
    1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // top left
   -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // top right
   -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, // bottom right
   // Left
   -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, // top right
   -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // bottom right
   -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // top left
   -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, // bottom left
   // Right
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // bottom left
    1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // top left
    1.0f, 1.0f, -1.0f, 1.0f, 1.0f, // bottom right
    1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // top right
   // Bottom
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // top right
   -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top left
    1.0f, 1.0f, -1.0f, 1.0f, 1.0f, // bottom right
   -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, // bottom left
   // Top
    1.0f, -1.0f, 1.0f, 1.0f, 1.0f, // bottom right
   -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, // bottom left
    1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // top right
   -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // top left
};
unsigned int indices[] = {
    // Front
    2, 3, 1,
    3, 0, 1,
    // Back
    4, 7, 6,
    6, 5, 4,
    // Left
    11, 9, 8,
    11, 8, 10,
    // Right
    12, 15, 13,
    12, 14, 15,
    // Bottom
    16, 17, 19,
    19, 18, 16,
    // Top
    20, 22, 23,
    20, 23, 21,
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

const unsigned char dirt_texture_file[] = {
#embed "assets/dirt.bmp"
};
const unsigned char* pdirt_texture_file = (const unsigned char*)dirt_texture_file;

static unsigned int create_shader_program() {
    unsigned int shader_program;
    int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &pvertsrc, nullptr);
    glCompileShader(vert_shader);
    int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &pfragsrc, nullptr);
    glCompileShader(frag_shader);
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return shader_program;
}

static unsigned int load_texture() {
    unsigned int texture;
    int width, height, channels;
    unsigned char* texdata = stbi_load_from_memory(pdirt_texture_file, sizeof(dirt_texture_file), &width, &height, &channels, 0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texdata);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texdata);

    return texture;
}

vec3 camera_position = {0.0f, 0.0f, 0.0f};
vec3 camera_up = {0.0f, 1.0f, 0.0f};
vec3 camera_front = {0.0f, 0.0f, -1.0f};
float camera_speed = 0.1f;

void key_callback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, [[maybe_unused]] int mods) {
    vec3 delta;
    switch(key) {
    case GLFW_KEY_W:
    case GLFW_KEY_UP:
        glm_vec3_copy(camera_front, delta);
        glm_vec3_scale(delta, 1.0f * camera_speed, delta);
        glm_vec3_add(camera_position, delta, camera_position);
        break;
    case GLFW_KEY_S:
    case GLFW_KEY_DOWN:
        glm_vec3_copy(camera_front, delta);
        glm_vec3_scale(delta, -1.0f * camera_speed, delta);
        glm_vec3_add(camera_position, delta, camera_position);
        break;
    case GLFW_KEY_A:
    case GLFW_KEY_LEFT:
        glm_cross(camera_front, camera_up, delta);
        glm_vec3_scale(delta, -1.0f * camera_speed, delta);
        glm_vec3_add(camera_position, delta, camera_position);
        break;
    case GLFW_KEY_D:
    case GLFW_KEY_RIGHT:
        glm_cross(camera_front, camera_up, delta);
        glm_vec3_scale(delta, 1.0f * camera_speed, delta);
        glm_vec3_add(camera_position, delta, camera_position);
        break;
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    }
}

float lastX = RESOLUTION_X / 2.0f;
float lastY = RESOLUTION_Y / 2.0f;
float pitch = 0.0f;
float yaw = -90.0f;
void mouse_callback([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) {
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;
    pitch = glm_clamp(pitch, -89.0f, 89.0f);

    vec3 look_direction = { cos(glm_rad(yaw)) * cos(glm_rad(pitch)), sin(glm_rad(pitch)), sin(glm_rad(yaw)) * cos(glm_rad(pitch)) };
    glm_vec3_copy(look_direction, camera_front);
}

void renderer_create(renderer *renderer) {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSwapInterval(1);
    renderer->window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, APP_NAME, nullptr, nullptr);
    glfwMakeContextCurrent(renderer->window);
    gladLoadGL(glfwGetProcAddress);
    glViewport(0, 0, RESOLUTION_X, RESOLUTION_Y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    glfwSetKeyCallback(renderer->window, key_callback);
    glfwSetInputMode(renderer->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(renderer->window, mouse_callback);

    glGenBuffers(1, &renderer->VBO);
    glGenBuffers(1, &renderer->EBO);
    glGenVertexArrays(1, &renderer->VAO);

    renderer->shader_program = create_shader_program();
    renderer->texture = load_texture();

    glBindVertexArray(renderer->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void renderer_destroy(renderer *renderer) {
    glDeleteTextures(1, &renderer->texture);
    glDeleteProgram(renderer->shader_program);

    glfwDestroyWindow(renderer->window);
    glfwTerminate();
}

bool renderer_update(renderer* renderer) {
    glfwPollEvents();

    glClearColor(135.0f/255.0f, 206.0f/255.0f, 235.0f/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(renderer->shader_program);
    glBindTexture(GL_TEXTURE_2D, renderer->texture);
    glBindVertexArray(renderer->VAO);

    // transformation
    mat4 projection;
    glm_perspective(glm_rad(45.0f), (float)RESOLUTION_X/(float)RESOLUTION_Y, 0.0001f, 10000.0f, projection);
    mat4 model;
    glm_mat4_identity(model);
    mat4 view;
    vec3 camera_target;
    glm_vec3_add(camera_position, camera_front, camera_target);
    glm_lookat(camera_position, camera_target, camera_up, view);
    mat4 MVP;
    glm_mat4_mul(view, model, MVP);
    glm_mat4_mul(projection, MVP, MVP);

    auto uniformloc = glGetUniformLocation(renderer->shader_program, "MVP");
    glUniformMatrix4fv(uniformloc, 1, GL_FALSE, (const float*)MVP);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(renderer->window);
    return !glfwWindowShouldClose(renderer->window);
}
