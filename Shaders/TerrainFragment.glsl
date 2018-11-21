#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform sampler2D grassTex;
uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;
uniform int lightType;
uniform float time;

in Vertex{
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 binormal;
    vec3 worldPos;
}IN;

out vec4 fragColour;

void main(void){


    float grad = dot(normalize(IN.normal), vec3(0,1,0))-0.5;

    float mixVal = clamp(grad, 0.0, 1.0);

    //vec4 diffuse = texture(diffuseTex, IN.texCoord);
   
    vec4 diffuse = mix(texture(diffuseTex, IN.texCoord), texture(grassTex, IN.texCoord), mixVal);
    //vec4 diffuse = mix(texture(diffuseTex, IN.texCoord), vec4(1,1,1,1), mixVal); 
    mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);

    vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));

    vec3 incident;
    float atten;
    if(lightType == 0){
        incident = normalize(-lightPos);
        atten = 1.0;
    }
    else{
        incident = normalize(lightPos-IN.worldPos);
        float dist = length(lightPos - IN.worldPos);
        atten = 1.0 - clamp(dist/lightRadius,0.0,1.0);
    }

    //vec3 incident = normalize(lightPos-IN.worldPos);
    float lambert = max(0.0, dot(incident, normal));

    //float dist = length(lightPos - IN.worldPos);
    //float atten = 1.0 - clamp(dist/lightRadius,0.0,1.0);

    vec3 viewDir = normalize(cameraPos - IN.worldPos);
    vec3 halfDir = normalize(incident + viewDir);

    float rFactor = max(0.0, dot(halfDir, normal));
    float sFactor = pow(rFactor, 50.0);

    vec3 colour = (diffuse.rgb * lightColour.rgb);


    colour += (lightColour.rgb * sFactor) * 0.33;    
    
   // fragColour = vec4(colour + halfDir * sFactor,1);
   // return;
    fragColour = vec4(colour*atten*lambert, diffuse.a);
    fragColour.rgb += (diffuse.rgb * lightColour.rgb) * 0.5;
}