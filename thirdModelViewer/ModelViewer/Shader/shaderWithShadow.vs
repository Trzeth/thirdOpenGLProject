#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 mesh;
uniform mat4 transformToOrigin;
uniform mat4 model;
uniform mat4 view;
uniform vec3 viewPos;
uniform mat4 projection;
uniform bool bend;

uniform mat4 lightSpaceMatrix;

out vec3 FragPos;
out vec3 FragPosLightSpace;
out vec3 Normal;
out vec2 TexCoords;
out vec3 ViewPos;

vec4 getWorldBendPos(vec4 pos){
    if(!bend)return pos;
    
    float result = pos.z - viewPos.z;
    if(result > -50) result = 0.001 * pow(result,2) + 0.1 * result;
    else result = 0.01 * pow(result,2) + 1 * result + 22.5;
    vec4 resultPos = pos;
    resultPos.y -= result;

    result = pos.x - viewPos.x;
    result = 0.005 * pow(result,2);

    resultPos.y -= result;

    return resultPos;
}

void main()
{
    vec4 worldPos = model * transformToOrigin * mesh * vec4(aPos, 1.0);
    gl_Position = projection * view * getWorldBendPos(worldPos);

    FragPos = getWorldBendPos(worldPos).xyz;
    FragPosLightSpace = vec3(lightSpaceMatrix * worldPos);
    
    Normal = (model * transformToOrigin * mesh * vec4(aNormal,0.0)).xyz ;
    TexCoords = aTexCoords;
    ViewPos = viewPos;
}