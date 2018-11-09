#version 150

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform float time;
uniform vec4 nodeColour;

in vec3 position;
in vec2 texCoord;
in vec4 colour;
in vec3 normal;

out Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
    vec3 worldPos;
} OUT;

void main(void) {


	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(position.x + sin(time/1000+10*position.y)*10, position.y + sin(time/800+10*((position.x*position.z)/2))*5, position.z + sin(time/900+10*position.y)*10, 1.0);
	//gl_Position = mvp * vec4(position, 1.0);
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	OUT.colour = nodeColour;

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

    OUT.normal = normalize(normalMatrix * normalize(normal));

    OUT.worldPos = (modelMatrix * vec4(position,1.0)).xyz;
}

//position.x + sin(time/1000+10*position.y)*10, position.y + sin(time/800+10*((position.x*position.z)/2))*5, position.z + sin(time/900+10*position.y)*10,