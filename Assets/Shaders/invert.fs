#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;

uniform vec2 centers[4];
uniform float radius;

void main() {
    vec4 texelColor = texture(texture0, fragTexCoord);

    vec2 pos = gl_FragCoord.xy;

    bool invert = false;

    for(int i=0; i<4; i++) {
        if (length(pos - centers[i]) < radius) {
            invert = !invert;
        }
    }

    if (invert && radius > 0) {
        finalColor = vec4(1 - texelColor.r, 1 - texelColor.g, 1 - texelColor.b, texelColor.a);
    } else {
        finalColor = texelColor;
    }
}