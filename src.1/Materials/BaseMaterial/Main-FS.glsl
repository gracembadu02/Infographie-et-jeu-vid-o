#version 460

layout(location = 0) out vec4 Color;

//visualiser la normale
in vec3 couleur;

void main() 
{
    Color = vec4(couleur, 1.0);
} 