#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 viewPos;
uniform mat4 model;

out vec2 TexCoord;
out vec3 WorldPos;

void main() {
	
	TexCoord = aTexCoord; 

	WorldPos = (model * vec4(aPos.x, aPos.y, aPos.z, 1.0)).xyz;	

	gl_Position = projection * view * vec4(WorldPos, 1.0);
}