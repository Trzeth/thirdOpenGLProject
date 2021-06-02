#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse;
uniform int useColor;
uniform vec3 diffuseColor;

void main()
{    
    if(useColor == 1)
    {
        FragColor = vec4(diffuseColor,1.0);
    }
    else
    {
        FragColor = texture(texture_diffuse, TexCoords);
    }
}