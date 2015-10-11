#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat4 modelMatrix;

varying vec3 vertex;
varying vec2 texCoord;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
	vertex = position;
	texCoord = texcoord;
}
