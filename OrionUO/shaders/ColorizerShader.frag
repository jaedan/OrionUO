#version 130

in vec3 light;
in vec3 normal;

uniform sampler2D texture;
uniform float hue[96];

uniform int drawMode;

// Draw Modes
#define DM_UNMODIFIED 0
#define DM_HUE 1
#define DM_PARTIAL_HUE 2
#define DM_HUE_TEXT_NO_BLACK 3
#define DM_HUE_TEXT 4
#define DM_LAND 6
#define DM_LAND_HUED 7
#define DM_SPECTRAL 10
#define DM_SPECIAL_SPECTRAL 11
#define DM_SHADOW 12

void main(void)
{
	vec4 textureColor = texture2D(texture, gl_TexCoord[0].xy);

	if (textureColor.a == 0.0) {
		// Fully transparent
		gl_FragColor = textureColor * gl_Color;
		return;
	}

	switch (drawMode) {
	case DM_HUE: {
		// Convert from a scale of 0 to 1 to a scale of 0 to 32. Multiply by 3
		// to get index into hue array.
		int index = int(textureColor.r * 31.875) * 3;
		gl_FragColor = vec4(hue[index], hue[index + 1], hue[index + 2], textureColor.a) * gl_Color;
		break;
	}
	case DM_PARTIAL_HUE: {
		// Convert only grayscale pixels
		if (textureColor.r == textureColor.g && textureColor.r == textureColor.b) {
			// Convert from a scale of 0 to 1 to a scale of 0 to 32. Multiply by 3
			// to get index into hue array.
			int index = int(textureColor.r * 31.875) * 3;
			gl_FragColor = vec4(hue[index], hue[index + 1], hue[index + 2], textureColor.a) * gl_Color;
		} else {
			gl_FragColor = textureColor * gl_Color;
		}
		break;
	}
	case DM_HUE_TEXT_NO_BLACK: {
		break;
	}
	case DM_HUE_TEXT: {
		break;
	}
	case DM_LAND: {
		float lightingScalar = max(dot(normalize(normal), normalize(light)), 0.0);
		gl_FragColor = (textureColor * lightingScalar * gl_Color * 0.5) + (textureColor * gl_Color * 0.5);
		break;
	}
	case DM_LAND_HUED: {
		float lightingScalar = max(dot(normalize(normal), normalize(light)), 0.0);
		int index = int(textureColor.r * 31.875) * 3;
		vec4 baseColor = vec4(hue[index], hue[index + 1], hue[index + 2], textureColor.a);
		gl_FragColor = (baseColor * lightingScalar * gl_Color * 0.5) + (baseColor * gl_Color * 0.5);
		break;
	}
	case DM_SPECTRAL: {
		float red = textureColor.r * 1.5;
		gl_FragColor = vec4(red, red, red, textureColor.a) * gl_Color;
		break;
	}
	case DM_SPECIAL_SPECTRAL: {
		float red = textureColor.r * 0.5;
		gl_FragColor = vec4(red, red, red, textureColor.a) * gl_Color;
		break;
	}
	case DM_SHADOW: {
		gl_FragColor = vec4(0.6, 0.6, 0.6, textureColor.a) * gl_Color;
		break;
	}
	case DM_UNMODIFIED:
	default:
		gl_FragColor = textureColor * gl_Color;
		break;
	}
}
