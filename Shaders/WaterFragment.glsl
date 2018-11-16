#version 150 core

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;
uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

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
    //vec3 incident = normalize(lightPos-IN.worldPos);
    //vec3 incident = normalize(IN.worldPos-cameraPos);
    incident = normalize(-lightPos);
    float lambert = max(0.0, dot(incident, IN.normal));

    float dist = length(lightPos - IN.worldPos);
    //float atten = 1.0 - clamp(dist/lightRadius,0.0,1.0);
     atten = 0.6;

    if(atten < 0.2){
        atten = 0.2;
    }

    vec3 viewDir = normalize(IN.worldPos - cameraPos);
    //vec3 viewDir = normalize(cameraPos - IN.worldPos);
    vec3 halfDir = normalize(incident + viewDir);

    //float rFactor = max(0.0, dot(halfDir, IN.normal));
    //float sFactor = pow(rFactor, 50.0);
    vec4 reflection = texture(cubeTex, reflect(halfDir, normalize(IN.normal)));
    //vec3 colour = (diffuse.rgb * lightColour.rgb);

    


    // colour += (lightColour.rgb * sFactor) * 0.33;    
     
    
    // fragColour = vec4(colour + halfDir * sFactor,1);
    // return;
    //fragColour = vec4(colour*atten*lambert, diffuse.a);
    //fragColour.rgb += (diffuse.rgb * lightColour.rgb) * 0.25;
   //fragColour = (reflection);

    fragColour = (lightColour*diffuse*atten)*(diffuse+(reflection*2));
    //fragColour = (lightColour*diffuse)*(diffuse+(reflection*2));
}