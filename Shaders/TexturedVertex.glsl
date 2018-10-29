#version 150

uniform mat4 modelMatrix = mat4(1);
uniform mat4 viewMatrix= mat4(1);
uniform mat4 projMatrix= mat4(1);
uniform mat4 textureMatrix= mat4(1);

in vec3 position;
in vec2 texCoord;

out Vertex {
	vec2 texCoord;
} OUT;

void main(void) {
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(position, 1.0);
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
}