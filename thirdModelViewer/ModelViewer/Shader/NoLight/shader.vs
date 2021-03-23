#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

uniform mat4 model;
uniform mat4 view;
uniform vec3 viewPos;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    vec4 worldPos = model * vec4(aPos,1.0);
    gl_Position = projection * view * model * worldPos;
    FragPos = worldPos.xyz;
    Normal = (model * vec4(aNormal,0.0)).xyz;
    TexCoords = aTexCoords;
}