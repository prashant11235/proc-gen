#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Color;
out vec3 Normal;
out vec3 FragPos;
out vec2 texCoords;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Color = aColor;
    Normal = aNormal;

    gl_Position = projection * view * vec4(FragPos, 1.0);
    texCoords = vec2(aPos.x, aPos.z);
}