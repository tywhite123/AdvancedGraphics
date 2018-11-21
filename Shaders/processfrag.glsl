#version 150 core

uniform sampler2D diffuseTex;
uniform vec2 pixelSize;
uniform int isVertical;

uniform int effectType;


in Vertex{
    vec2 texCoord;
    vec4 colour;
}IN;

out vec4 fragColour;

const float weights[5] = float[](0.12,0.22,0.32,0.22,0.12);

void main(void){
    vec2 values[5];


    vec2 sobel[9];
    sobel[0] = vec2(-pixelSize.x,-pixelSize.y);
    sobel[1] = vec2(0.0,-pixelSize.y);
    sobel[2] = vec2(pixelSize.x,-pixelSize.y);
    sobel[3] = vec2(-pixelSize.x,0.0);
    sobel[4] = vec2(0.0,0.0);
    sobel[5] = vec2(pixelSize.x,0.0);
    sobel[6] = vec2(-pixelSize.x,pixelSize.y);
    sobel[7] = vec2(0.0,pixelSize.y);
    sobel[8] = vec2(pixelSize.x,pixelSize.y);
    vec4 sobellol[9];

    if(effectType == 0){

        if(isVertical == 1){
            values = vec2[](vec2(0.0, -pixelSize.y*2),
            vec2(0.0, -pixelSize.y*1), vec2(0.0,0.0),
            vec2(0.0, pixelSize.y*1), vec2(0.0, pixelSize.y*2));
        }
        else{
        values = vec2[](vec2(-pixelSize.x*2, 0.0),
            vec2(-pixelSize.x*1, 0.0), vec2(0.0,0.0),
            vec2(pixelSize.x*1, 0.0), vec2(pixelSize.x*2, 0.0));
        }

        for(int i = 0; i < 5; i++){
            vec4 tmp = texture2D(diffuseTex, IN.texCoord.xy + values[i]);
            //sobellol[i] = tmp;
            fragColour += tmp * weights[i];
            //fragColour += vec4(1.0-tmp.rgb,1.0)*tmp;
        }

    }
    else if(effectType == 1){
         for(int i = 0; i < 9; i++){
            vec4 tmp = texture2D(diffuseTex, IN.texCoord.xy + sobel[i]);
            sobellol[i] = tmp;
            //fragColour += tmp * weights[i];
        }
        vec4 sobelH = sobellol[2] + (2.0*sobellol[5]) + sobellol[8] - (sobellol[0]+(2.0*sobellol[3]) + sobellol[6]);
        vec4 sobelV = sobellol[0] + (2.0*sobellol[1]) + sobellol[2] - (sobellol[6]+(2.0*sobellol[7]) + sobellol[8]);
        vec4 effect = sqrt((sobelH*sobelH) + (sobelV*sobelV));

        fragColour = vec4(1.0-effect.rgb, 1.0);
    }
    //fragColour = vec4(1,1,1,1);
    
    else if(effectType == 2){
        vec4 tex = texture2D(diffuseTex, IN.texCoord);
        float avg = (tex.x * 0.2126) + (tex.y * 0.7152) + (tex.z * 0.0722);
        //float avg = (tex.x + tex.y + tex.z)/3;
        fragColour = tex * avg;
        //if(avg > 0.7){
        //    fragColour = tex;
        //}
        //else{
        //    fragColour = vec4(0.0, 0.0, 0.0, 1.0);
        //}
    }

    else if(effectType == 3){
        fragColour = texture2D(diffuseTex, IN.texCoord);
        fragColour = vec4(1.0-fragColour.x, 1.0-fragColour.y, 1.0-fragColour.z, 1.0);
    }

    else if(effectType == 4){
        vec4 tex = texture2D(diffuseTex, IN.texCoord);
        float avg = (tex.x * 0.2126) + (tex.y * 0.7152) + (tex.z * 0.0722);
        fragColour = vec4(avg, avg, avg, 1.0);

    }

}