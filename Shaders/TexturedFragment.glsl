#version 150
uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColor;


void main(void){
	//fragColor = vec4(1,1,0,1);
	// fragColor = IN.colour;
	fragColor = texture(diffuseTex, IN.texCoord) * IN.colour;
}