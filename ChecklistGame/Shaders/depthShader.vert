#version 330 core
layout (location = 0) in mat4 instanceMatrix;
layout (location = 4) in vec3 position;

uniform mat4 model;
uniform int useInstance;

uniform mat4 lightSpaceMatrix;

void main()
{
    
    mat4 modelMat;
    if(useInstance == 0) modelMat = model;
    else modelMat = instanceMatrix;

    gl_Position = lightSpaceMatrix * modelMat * vec4(position, 1.0f);
}