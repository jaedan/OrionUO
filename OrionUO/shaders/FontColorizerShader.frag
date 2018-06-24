#version 130

uniform sampler2D texture;
uniform int drawMode;
uniform float hue[96];

void main(void)
{
	vec4 textureColor = texture2D(texture, gl_TexCoord[0].xy);

	if (textureColor.a != 0.0)
	{
		if (drawMode == 1 || (drawMode == 2 && textureColor.r == textureColor.g && textureColor.r == textureColor.b))
		{
			int index = int(textureColor.r * 31.875) * 3;

			gl_FragColor = vec4(hue[index], hue[index + 1], hue[index + 2], textureColor.a) * gl_Color;
		}
		else if (drawMode == 4 || (drawMode == 3 && textureColor.r > 0.04))
		{
			gl_FragColor = vec4(hue[90], hue[91], hue[92], textureColor.a) * gl_Color;
		}
		else
			gl_FragColor = textureColor * gl_Color;
	}
	else
		gl_FragColor = textureColor * gl_Color;
}
