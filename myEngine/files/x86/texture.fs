#version 330 core

layout(location = 0) out vec4 color;

in vec2 uv0;

uniform sampler2D texture0;
uniform vec4 albedo;

void main() 
{    
	//color = vec4(1, 0, 0, 1.0f);
	color = texture2D(texture0, uv0) * albedo;
}