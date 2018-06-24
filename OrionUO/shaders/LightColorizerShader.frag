#version 130

uniform sampler2D texture;
uniform int drawMode;
uniform float hue[96];

void main(void)
{
	vec4 textureColor = texture2D(texture, gl_TexCoord[0].xy);

	if (textureColor.a != 0.0 && drawMode == 1)
	{
		int index = int(textureColor.r * 7.96875) * 3;

		gl_FragColor = (textureColor * vec4(hue[index], hue[index + 1], hue[index + 2], 1.0)) * 3.0;
	}
	else
		gl_FragColor = textureColor;
}
