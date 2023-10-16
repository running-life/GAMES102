#version 330 core
layout (location = 0) in vec2 position;
uniform vec3 color;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(position,1.0f,1.0f);
    ourColor = color;
}