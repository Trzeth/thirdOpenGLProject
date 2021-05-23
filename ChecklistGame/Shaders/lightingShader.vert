#version 330 core
layout (location = 0) in mat4 instanceMatrix;
layout (location = 4) in vec3 aPos;
layout (location = 5) in vec3 aNormal;
layout (location = 6) in vec2 aTexCoords;

layout (std140) uniform baseMatrices
{
    mat4 projection;
    mat4 view;
    mat4 lightSpace;
    vec3 viewPos;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform mat4 model;

uniform int useInstance;

out VS_OUT{
    vec3 viewPos;
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;

    // Shadow
    vec4 fragPosLightSpace;

    // Lighting
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} vs_out;


void main()
{
    vec4 pos = vec4(aPos, 1.0f);

    mat4 modelMat;
    if(useInstance == 0) modelMat = model;
    else modelMat = instanceMatrix;

    gl_Position = projection * view * modelMat * pos;

    vs_out.viewPos = viewPos;
    vs_out.fragPos = vec3(view * modelMat * pos);
    vs_out.fragPosLightSpace = lightSpace * vec4(vs_out.fragPos, 1.0);

    vs_out.normal = mat3(transpose(inverse(modelMat))) * aNormal;
    vs_out.texCoords = aTexCoords;
    vs_out.direction = direction;
    vs_out.ambient = ambient;
    vs_out.diffuse = diffuse;
    vs_out.specular = specular;
}