#version 150 core

uniform vec4 nodeColour;

in vec3 position;
in vec4 colour;

out Vertex{
    vec4 colour;
} OUT;

void main(void){
    gl_Position = vec4(position, 1.0);
    OUT.colour = nodeColour;
}