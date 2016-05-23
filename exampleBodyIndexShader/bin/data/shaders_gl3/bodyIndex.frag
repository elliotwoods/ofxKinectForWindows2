#version 150

uniform sampler2DRect uColorTex;

in float vKeep;
in vec2 vTexCoord;

out vec4 vFragColor;

void main()
{
	if (vKeep == 0.0) {
		discard;
	}

	vFragColor = texture(uColorTex, vTexCoord);
}
