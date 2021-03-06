#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec2 uv;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat4 modelMatrix;
varying vec2 texCoord;
varying float yVal;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
    //vec2 noiseMix = noise(position.xz);
    //gl_Position.y = noiseMix.x * noiseMix.y;
	texCoord = uv * 4.0;
    yVal = (modelMatrix * vec4(position, 1.0)).y;
}
