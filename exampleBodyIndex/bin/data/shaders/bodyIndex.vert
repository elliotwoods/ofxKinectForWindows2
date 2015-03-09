#version 120
#extension GL_EXT_gpu_shader4 : require

uniform sampler2DRect uBodyIndexTex;
uniform int uWidth;

void main()
{ 
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord1;

	vec2 bIdxTexCoord = vec2(gl_VertexID % uWidth, gl_VertexID / uWidth);
	vec4 bIdxColor = texture2DRect(uBodyIndexTex, bIdxTexCoord);

	if (bIdxColor.rgb == vec3(1.0, 1.0, 1.0)) {
		gl_FrontColor = vec4(0.0, 0.0, 0.0, 0.0);
	}
	else {
		gl_FrontColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
}
