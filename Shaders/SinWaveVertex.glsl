#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec4 colour;
in vec2 texCoord;
in vec3 normal;


out Vertex{
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
}OUT;

void main(void){
    OUT.colour = colour;
    OUT.texCoord = (textureMatrix*vec4(texCoord,0.0,1.0)).xy;

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

    OUT.normal = normalize(normalMatrix * normalize(normal));

    OUT.worldPos = (modelMatrix * vec4(position,1.0)).xyz;
    gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position.x + sin(time/1000+10*position.y)*10, position.y + sin(time/800+10*((position.x*position.z)/2))*5, position.z + sin(time/900+10*position.y)*10,1.0);

}