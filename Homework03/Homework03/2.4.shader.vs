#version 330 core
layout (location = 0) in vec3 aPos;
uniform float posX=0.0f;
uniform float posY=0.0f;

void main()
{
    gl_Position = vec4(aPos[0]+posX, aPos[1]+posY, aPos[2], 1.0);
}

