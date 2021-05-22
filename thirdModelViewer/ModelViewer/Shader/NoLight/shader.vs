#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instanceMatrix;

layout (std140) uniform baseMatrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

uniform int useInstance;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    vec4 pos = vec4(aPos, 1.0f);
	vec4 normal = vec4(aNormal, 0.0f);

    mat4 modelMat;
    if(useInstance == 0) modelMat = model;
    else modelMat = instanceMatrix;

    gl_Position = projection * view * modelMat * pos;
    FragPos = vec3(view * modelMat * pos);

    Normal = (transpose(inverse(view * modelMat)) * normal).xyz;
    TexCoords = aTexCoords;
}