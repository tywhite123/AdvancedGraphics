#version 150
uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColor;

void main(void){
	fragColor = texture(diffuseTex, IN.texCoord);
}