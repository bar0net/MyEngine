#version 330 core

layout(location = 0) out vec4 color;

in vec3 uv0;

void main() 
{    
	vec4 axis = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	if (uv0.x == 0) axis.x = 0;
	if (uv0.y == 0) axis.y = 0;
	if (uv0.z == 0) axis.z = 0;
	color = axis;

	//color = vec4(uv0,1.0);
}