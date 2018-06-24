#version 130

out vec3 light;
out vec3 normal;

uniform int drawMode;

void main(void)
{
	if (drawMode > 5)
	{
		light = normalize(vec3(gl_LightSource[0].position));
		normal = normalize(gl_NormalMatrix * gl_Normal);
	}

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}
