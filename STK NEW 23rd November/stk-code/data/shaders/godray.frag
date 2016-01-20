#version 130
uniform sampler2D tex;
uniform vec2 sunpos;

#define SAMPLES 12

const float decaystep = 0.88;

out vec4 FragColor;

void main()
{
	vec2 texc = gl_TexCoord[0].xy;
	vec2 tosun = sunpos - texc;

	if (dot(tosun, tosun) > 0.49) discard;

	vec2 dist = tosun * 1.0/(float(SAMPLES) * 1.12);

	vec3 col = texture(tex, texc).xyz;
	float decay = 1.0;

	for (int i = 0; i < SAMPLES; i++) {
		texc += dist;
		vec3 here = texture(tex, texc).xyz;
		here *= decay;
		col += here;
		decay *= decaystep;
	}

	FragColor = vec4(col, 1.0) * 0.8;
}
