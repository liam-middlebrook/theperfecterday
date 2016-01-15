#version 330

#extension GL_ARB_shading_language_420pack : require
varying vec2 texCoord;

layout(binding = 2) uniform sampler2D tex;
layout(binding = 3) uniform sampler2D tex2;
varying float yVal;
void main() {
    if (yVal > 0.0f)
    {
        gl_FragColor = texture(tex, texCoord);
    }
    else
    {
        gl_FragColor = texture(tex2, texCoord);
    }
}
