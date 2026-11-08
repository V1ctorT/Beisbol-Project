#version 330 core
layout (location = 0) in vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out float alpha;
void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    alpha = mix(0.25, 1.0, 1.0 + position.y);   // 1 en la punta, 0.25 en la base
}