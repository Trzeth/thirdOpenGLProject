#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 mesh;
uniform mat4 model;
uniform mat4 view;
uniform vec3 viewPos;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

vec4 getWorldBendPos(vec4 pos){
    float result = pos.z - viewPos.z;
    result = 0.001 * pow(result,2) + 0.1 * result;
    vec4 resultPos = pos;
    resultPos.y -= result;
    return resultPos;
}


void main()
{
    vec4 worldPos = model * mesh * vec4(aPos, 1.0);
    gl_Position = projection * view * getWorldBendPos(worldPos);
    FragPos = vec3(worldPos.xyz);
    Normal = (model * mesh * vec4(aNormal,0.0)).xyz ;
    TexCoords = aTexCoords;
}