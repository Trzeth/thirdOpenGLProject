#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

const int MAX_BONES = 100;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bones[MAX_BONES];

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	mat4 bone_transform = bones[BoneIDs[0]] * Weights[0];
	bone_transform += bones[BoneIDs[1]] * Weights[1];
	bone_transform += bones[BoneIDs[2]] * Weights[2];
	bone_transform += bones[BoneIDs[3]] * Weights[3];

    vec4 pos_anim = bone_transform * vec4(aPos, 1.0f);
	vec4 normal_anim = bone_transform * vec4(aNormal, 0.0f);

    gl_Position = projection * view * model * pos_anim;
    FragPos = vec3(view * model * pos_anim);

    Normal = (transpose(inverse(view * model)) * normal_anim).xyz;
    TexCoords = aTexCoords;
}