#version 330

varying vec2 texCoord;

void main() {
    gl_FragColor = vec4(texCoord, 0.0, 1.0);
}
