#version 130
uniform vec3 col;
uniform sampler2D tex;

out vec4 FragColor;

void main()
{
	float alpha = texture(tex, gl_TexCoord[0].xy).a;
	if (alpha < 0.5)
		discard;

	FragColor = vec4(col, 1.0);
}
