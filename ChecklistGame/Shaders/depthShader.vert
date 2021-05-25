#version 330 core
layout (location = 0) in mat4 instanceMatrix;
layout (location = 4) in vec3 aPos;
layout (location = 7) in ivec4 BoneIDs;
layout (location = 8) in vec4 Weights;

uniform mat4 model;
uniform int useInstance;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];
uniform int hasBone;

uniform mat4 lightSpaceMatrix;

void main()
{
    vec4 pos;
    if(hasBone == 0)
    {
        pos = vec4(aPos, 1.0f);
    }
    else
    {
        mat4 bone_transform = bones[BoneIDs[0]] * Weights[0];
	    bone_transform += bones[BoneIDs[1]] * Weights[1];
	    bone_transform += bones[BoneIDs[2]] * Weights[2];
	    bone_transform += bones[BoneIDs[3]] * Weights[3];

        pos = bone_transform * vec4(aPos, 1.0f);
    }

    mat4 modelMat;
    if(useInstance == 0) modelMat = model;
    else modelMat = instanceMatrix;

    gl_Position = lightSpaceMatrix * modelMat * pos;
}