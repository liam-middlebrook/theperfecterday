#version 330

in vec2 UV;

void main() {
    gl_FragColor = vec4(UV, 0.5, 1.0);
}
