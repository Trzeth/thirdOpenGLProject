#version 330 core
in vec3 textureCoords;
out vec4 color;

uniform samplerCube cubemap;

void main()
{    
    color = texture(cubemap, textureCoords);
}
  