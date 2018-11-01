#version 330 core

uniform vec3 albedo;
out vec4 color;

in vec2 uv0;

uniform sampler2D texture0;

void main() 
{    
	//color = vec4(1, 0, 0, 1.0f);
	color = texture2D(texture0, uv0);
}