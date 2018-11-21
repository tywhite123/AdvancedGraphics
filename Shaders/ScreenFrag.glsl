#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D bloomTex;

in Vertex{
    vec2 texCoord;
    vec4 colour;
}IN;

out vec4 fragColour;

void main(void){
    vec4 diffuse = texture(diffuseTex, IN.texCoord);
    vec4 bloom = texture(bloomTex, IN.texCoord);
    //vec3 specular = texture(specularTex, IN.texCoord).xyz;
    

    fragColour = diffuse + bloom;

}