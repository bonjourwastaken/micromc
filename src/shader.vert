#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 in_tex_coords;

out vec2 tex_coords;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(pos.x, pos.y, pos.z, 1.0);
    tex_coords = in_tex_coords;
}
