#version 120

uniform sampler2DRect uColorTex;

void main()
{
	if (gl_Color.a == 0.0) {
		discard;
	}

	vec4 color = texture2DRect(uColorTex, gl_TexCoord[0].st);
	gl_FragColor = color;
}
