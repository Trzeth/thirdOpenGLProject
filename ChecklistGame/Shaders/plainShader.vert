#version 330 core
layout (location = 0) in mat4 instanceMatrix;
layout (location = 4) in vec3 aPos;
layout (location = 5) in vec3 aNormal;
layout (location = 6) in vec2 aTexCoords;
layout (location = 7) in ivec4 BoneIDs;
layout (location = 8) in vec4 Weights;

layout (std140) uniform baseMatrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform int useInstance;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];
uniform int hasBone;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    vec4 pos;
    vec4 normal;
    if(hasBone == 0)
    {
        pos = vec4(aPos, 1.0f);
        normal = vec4(aNormal, 0.0f);
    }
    else
    {
        mat4 bone_transform = bones[BoneIDs[0]] * Weights[0];
	    bone_transform += bones[BoneIDs[1]] * Weights[1];
	    bone_transform += bones[BoneIDs[2]] * Weights[2];
	    bone_transform += bones[BoneIDs[3]] * Weights[3];

        pos = bone_transform * vec4(aPos, 1.0f);
	    normal = bone_transform * vec4(aNormal, 0.0f);
    }

    mat4 modelMat;
    if(useInstance == 0) modelMat = model;
    else modelMat = instanceMatrix;

    gl_Position = projection * view * modelMat * pos;
    FragPos = vec3(view * modelMat * pos);

    Normal = (transpose(inverse(view * modelMat)) * normal).xyz;
    TexCoords = aTexCoords;
}