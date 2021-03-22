#version 330                                                                        
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;                                     
                                                                                    
uniform mat4 wvp;                                                                  
                                                                                    
out vec3 TexCoord0;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
    vec4 WVP_Pos = wvp * vec4(aPos, 1.0);                                      
    gl_Position = WVP_Pos.xyww;                                                     
    TexCoord0   = aPos;                                                         
}
