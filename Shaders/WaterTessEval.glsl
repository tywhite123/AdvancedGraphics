#version 400

layout(quads, cw) in;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform float time;
uniform float waterWave;

in Vertex{
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
}IN[];

out Vertex{
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
}OUT;

vec3 QuadMixVec3(vec3 a, vec3 b, vec3 c, vec3 d){
    vec3 p0 = mix(a,c,gl_TessCoord.x);
    vec3 p1 = mix(b,d,gl_TessCoord.x);
    
    
    return mix(p0, p1, gl_TessCoord.y);
}

// vec3 toBezier(vec3 a, vec3 b, vec3 c, vec3 d){
//     float t = time * 0.3f;
//     float t2 = t*t;
//     float oneMT = 1.0 - t;
//     float oneMT2 = oneMT * oneMT;
//     return(a*oneMT2 * oneMT + b * 3.0 * t * oneMT2 + c * 3.0 * t2 * oneMT + d * t2 * t);

// }

vec2 QuadMixVec2(vec2 a, vec2 b, vec2 c, vec2 d){
    vec2 p0 = mix(a,c,gl_TessCoord.x);
    vec2 p1 = mix(b,d,gl_TessCoord.x);
    
    return mix(p0, p1, gl_TessCoord.y); 
}

void main(){
     vec3 combinedPos = QuadMixVec3(
         gl_in[0].gl_Position.xyz,
         gl_in[1].gl_Position.xyz,
         gl_in[2].gl_Position.xyz,
         gl_in[3].gl_Position.xyz);

    //  vec4 worldPos = modelMatrix * vec4(
    //      combinedPos.x + sin(time/1000+10*combinedPos.y)*0.5, 
    //      combinedPos.y + sin(time/800+10*((combinedPos.x*combinedPos.z)/2)*0.5), 
    //      combinedPos.z + sin(time/900+10*combinedPos.y)*0.5,1.0);

    vec4 worldPos = modelMatrix * vec4(
            combinedPos.x, 
            combinedPos.y, 
            combinedPos.z, 1.0);


    OUT.texCoord = QuadMixVec2(
        IN[0].texCoord,
        IN[1].texCoord,
        IN[2].texCoord,
        IN[3].texCoord
    );

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    vec3 normal = QuadMixVec3(
        IN[0].normal, 
        IN[1].normal,
        IN[2].normal,
        IN[3].normal
    );

    OUT.normal = normalize(normalMatrix * normalize(normal));

    //worldPos.y += sin((time)+((worldPos.x+worldPos.z)))*cos(time * sin(time + ((worldPos.x+worldPos.z)))) * 10;
    worldPos.y += cos(sin((time * 1 + (worldPos.x / 20) * 1 + (worldPos.z / 20) * 1)/2)*2)+sin(cos(sin((time * 1 + (worldPos.x / 20) * 1 + (worldPos.z / 20) * 1)/3) * sin(cos((time * 1 + (worldPos.x / 20) * 1 + (worldPos.z / 20) * 1)/2.5) * sin((time * 1 + (worldPos.x / 20) * 1 + (worldPos.z / 20) * 1)/2)) * 2) * cos((time * 1 + (worldPos.x / 20) * 1 + (worldPos.z / 20) * 1)/2) + sin((time * 1 + (worldPos.x / 20) * 1 + (worldPos.z / 20) * 1)/1.5)) * sin(waterWave*2+(worldPos.z/20))*10;
   worldPos.x += cos(sin(((time * 1) * (worldPos.y / 10) * (worldPos.z / 10))/2)*2)+sin(cos(sin(((time * 1) * (worldPos.y / 10) * (worldPos.z / 10))/3) * sin(cos(((time * 1) * (worldPos.y / 10) * (worldPos.z / 10))/2.5) * sin(((time * 1) * (worldPos.y / 10) * (worldPos.z / 10))/2)) * 2) * cos(((time * 1) * (worldPos.y / 10) * (worldPos.z / 10))/2) + sin(((time * 1) * (worldPos.y / 11) * (worldPos.z / 14))/1.5)) * sin(waterWave*2) * 10;
    worldPos.z += cos(sin(((time * 1) * (worldPos.x / 10) * (worldPos.y / 10))/2)*2)+sin(cos(sin(((time * 1) * (worldPos.x / 10) * (worldPos.y / 10))/3) * sin(cos(((time * 1) * (worldPos.x / 10) * (worldPos.y / 10))/2.5) * sin(((time * 1) * (worldPos.x / 10) * (worldPos.y / 10))/2)) * 2) * cos(((time * 1) * (worldPos.x / 10) * (worldPos.y / 10))/2) + sin(((time * 1) * (worldPos.x / 10) * (worldPos.y / 10))/1.5)) * sin(waterWave*2) * 8;
    //worldPos.y += time;

    OUT.worldPos = vec3(worldPos);

    gl_Position = projMatrix * viewMatrix * worldPos;

}