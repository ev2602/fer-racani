#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture;
uniform float alpha;

void main()
{
    color = texture2D(texture, TexCoords);
    //color = vec4(1.0, 0.0, 0.0, 1.0);
    color.a = alpha;
}