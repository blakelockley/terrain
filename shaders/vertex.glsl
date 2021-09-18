#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 view, projection;

out vec3 vecPosition;

void main()
{
    gl_Position = projection * view * vec4(position, 1.0);
    vecPosition = position;
}
