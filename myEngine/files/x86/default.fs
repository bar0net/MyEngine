#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 albedo;

void main()
{
	color = vec4(albedo.x, albedo.y, albedo.z, albedo.w);
}