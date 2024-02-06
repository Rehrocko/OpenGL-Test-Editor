#version 420 core

//out vec4 fragColor;

out vec4 colorOut;

//layout (binding = 0) uniform sampler2D basecolorTexture;

in vec2 TexCoord;
in vec3 WorldPos;

uniform vec3 viewPos;
uniform bool isPlayer;

void main() {
	//vec4 baseColor = texture(basecolorTexture, TexCoord);

    //fragColor = baseColor;
    float d = (distance(viewPos, WorldPos)+1)/30;
    if (isPlayer){
        d *= 2;
    }
    colorOut = vec4(d, d, d, 1.0f);

    //if (baseColor.a < 0.1) {
    //    discard;
    //}
}