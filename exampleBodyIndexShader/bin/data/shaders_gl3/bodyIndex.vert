#version 150

// OF default uniforms and attributes
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;
    	
// App uniforms and attributes
uniform sampler2DRect uBodyIndexTex;
uniform int uWidth;

out float vKeep;
out vec2 vTexCoord;

void main()
{ 
	gl_Position = modelViewProjectionMatrix * position;
	vTexCoord = texcoord;

	vec2 idxTexCoord = vec2(gl_VertexID % uWidth, gl_VertexID / uWidth);
	vec4 idxColor = texture(uBodyIndexTex, idxTexCoord);

	if (idxColor.r == 1.0) {
		vKeep = 0.0;
	}
	else {
		vKeep = 1.0;
	}
}
