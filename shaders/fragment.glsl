#version 330 core

in vec3 vecPosition;
out vec4 FragColor;

const vec3 bottomColor = vec3(0.3, 0.2, 0.0);
const vec3 topColor = vec3(0.0, 1.0, 0.2);

void main()
{
    vec3 color = mix(bottomColor, topColor, vecPosition.y);
    FragColor = vec4(color, 1.0f);
}