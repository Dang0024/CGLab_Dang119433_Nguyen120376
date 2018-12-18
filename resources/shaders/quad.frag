#version 150

in vec2 pass_TexCoord;  // texture coordinates

out vec4 out_Color;

// uniform values

uniform sampler2D ColorTex; // sampler variable

void main() {

    float x = pass_TexCoord.x;
    float y = pass_TexCoord.y;



    out_Color = texture(ColorTex, vec2(x, y));

}
