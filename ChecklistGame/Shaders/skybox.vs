#version 330 core
layout (location = 4) in vec3 position;
out vec3 textureCoords;

layout (std140) uniform baseMatrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
	mat4 viewNoTransl = mat4(mat3(view));
    vec4 pos = projection * viewNoTransl * vec4(position, 1.0);  
	// Repeat w so we get a depth buffer of 1.0 for the skybox
	gl_Position = pos.xyww;
    textureCoords = position;
}  
