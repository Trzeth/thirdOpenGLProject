#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

const int MAX_BONES = 36;
uniform mat4 bones[MAX_BONES];

uniform mat4 mesh;
uniform mat4 model;
uniform mat4 view;
uniform vec3 viewPos;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 ViewPos;

mat4 getBoneTransform(){
    mat4 boneTransform = bones[BoneIDs[0]] * Weights[0];
   	boneTransform += bones[BoneIDs[1]] * Weights[1];
   	boneTransform += bones[BoneIDs[2]] * Weights[2];
   	boneTransform += bones[BoneIDs[3]] * Weights[3];

    return boneTransform;
}


vec4 getWorldBendPos(vec4 pos){
    float result = pos.z - viewPos.z;
    result = 0.001 * pow(result,2) + 0.1 * result;
    vec4 resultPos = pos;
    resultPos.y -= result;
    return resultPos;
}

void main()
{
    mat4 BoneTransform = getBoneTransform();

    vec4 PosL = BoneTransform * vec4(aPos, 1.0);
    vec4 NormalL = BoneTransform * vec4(aNormal,0.0);

    vec4 worldPos = model * mesh * PosL;
    gl_Position = projection * view * getWorldBendPos(worldPos);
    FragPos = vec3(worldPos.xyz);

    Normal = (model * mesh * NormalL).xyz;
    TexCoords = aTexCoords;
    ViewPos = viewPos;
}