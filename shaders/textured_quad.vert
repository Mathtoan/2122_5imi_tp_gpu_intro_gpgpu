#version 330 core
layout (location = 0) in vec3 pos;

out vec2 uv;

void main()
{
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
    uv = (gl_Position.xy + vec2(1.,1.)) * vec2(0.5, 0.5);
}
