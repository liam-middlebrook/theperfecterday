#version 330

#extension GL_ARB_shading_language_420pack : require
varying vec2 texCoord;

layout(binding = 2) uniform sampler2D tex;
void main() {
    gl_FragColor = texture(tex, texCoord);
}
