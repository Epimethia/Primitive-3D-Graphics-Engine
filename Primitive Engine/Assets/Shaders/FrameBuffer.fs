#version 330 core
out vec4 color;
  
in vec2 fragTexCoords;

uniform sampler2D tex;
uniform float dTime;
uniform int DisplayMode;



void main()
{ 

    if (DisplayMode == 0) {
        color = texture(tex, fragTexCoords);
    }
   
    else if (DisplayMode == 1) {
        float offset = 1.0 / 300.0;
        vec2 offsets[9] = vec2[](
            vec2(-offset,  offset), // top-left
            vec2( 0.0f,    offset), // top-center
            vec2( offset,  offset), // top-right
            vec2(-offset,  0.0f),   // center-left
            vec2( 0.0f,    0.0f),   // center-center
            vec2( offset,  0.0f),   // center-right
            vec2(-offset, -offset), // bottom-left
            vec2( 0.0f,   -offset), // bottom-center
            vec2( offset, -offset)  // bottom-right    
        );
        //#################################################################
        //Edge Detect Kernel 
        //taken from https://learnopengl.com/Advanced-OpenGL/Framebuffers
        //#################################################################

        //kernel taken from 
        //http://matlabtricks.com/post-5/3x3-convolution-kernels-with-online-demo#demo
        float edgeDetectKernel[9] = float[](
            0, 1, 0,
            1, -3.5, 1,
            0, 1, 0
        );
        
        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++) {
            sampleTex[i] = vec3(texture(tex, fragTexCoords.st + offsets[i]));
        }

        vec3 col = vec3(0.0);

        for(int i = 0; i < 9; i++) {
            col += sampleTex[i] * edgeDetectKernel[i];
        }
        
        color = vec4(col, 1.0);
        return;
    }

    else {
        //#################################################################
        //Scan lines + blur + chromatic aberration effect 
        //taken from https://www.shadertoy.com/view/XdXXD4
        //#################################################################

        float d = length(fragTexCoords - vec2(0.5,0.5));
        
        // blur amount
        float blur = 0.0;	
        blur = (1.0 + sin(dTime*6.0)) * 0.5;
        blur *= 1.0 + sin(dTime*16.0) * 0.5;
        blur = pow(blur, 3.0);
        blur *= 0.05;
        // reduce blur towards center
        blur *= d;
        
        // final color
        vec3 col;
        col.r = texture(tex, vec2(fragTexCoords.x + blur,fragTexCoords.y)).r;
        col.g = texture(tex, fragTexCoords).g;
        col.b = texture(tex, vec2(fragTexCoords.x - blur,fragTexCoords.y)).b;
        
        // scanline
        float scanline = sin(fragTexCoords.y * 400.0) * 0.2;
        col -= scanline;
        
        // vignette
        col *= 1.0 - d * 0.5;
        
        color = vec4(col,1.0);
        return;
    }
    
}