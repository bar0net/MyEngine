#version 330 

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 uv;

layout(std140) uniform GlobalMatrices
{
    mat4 proj;
    mat4 view;
};

uniform mat4 model;

out vec2 uv0;

void main() 
{    
	gl_Position = proj*view*model*vec4(vertex_position, 1.0); 

	uv0 = uv;
}