#version 330 core

in vec3 clr;
in vec2 TexCoord;

//uniform sampler2D myTexture;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mix_value;

out vec4 color;
out vec4 FragColor;

uniform float texScale;
uniform float texShift;

void main()
{
	//color = vec4(clr, 1.0f);

	FragColor = mix(texture(texture0, TexCoord * texScale + vec2(texShift, 0.0)), texture(texture2, TexCoord * texScale + vec2(texShift, 0.0)), mix_value);
	//FragColor = texture(texture0, TexCoord);
}