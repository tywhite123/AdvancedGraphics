#version 150 core

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;
uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;
uniform int lightType;

in Vertex{
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
}IN;

out vec4 fragColour;

void main(void){

    vec4 diffuse = texture(diffuseTex, IN.texCoord) * IN.colour;
    
    vec3 incident;
    float atten;
    
    if(lightType == 0){
        incident = normalize(-lightPos);
        atten = 0.6;
    }
    else{
        incident = normalize(IN.worldPos-cameraPos);
        float dist = length(lightPos - IN.worldPos);
        atten = 1.0 - clamp(dist/lightRadius,0.0,1.0);
    }

    float lambert = max(0.0, dot(incident, IN.normal));

    
    

    if(atten < 0.2){
        atten = 0.2;
    }

    vec3 viewDir = normalize(IN.worldPos - cameraPos);
    vec3 halfDir = normalize(incident + viewDir);

    //float rFactor = max(0.0, dot(halfDir, IN.normal));
    //float sFactor = pow(rFactor, 50.0);
    vec4 reflection = texture(cubeTex, reflect(halfDir, normalize(IN.normal)));
    //vec3 colour = (diffuse.rgb * lightColour.rgb);

    


    // colour += (lightColour.rgb * sFactor) * 0.33;    
     
    

    fragColour = (lightColour*diffuse*atten)*(diffuse+(reflection*2));
    //fragColour = (lightColour*diffuse)*(diffuse+(reflection*2));
}