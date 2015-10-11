#version 330

varying vec2 texCoord;

void main() {
    gl_FragColor = vec4(texCoord.x, 0.0, texCoord.y, 1.0);
}
