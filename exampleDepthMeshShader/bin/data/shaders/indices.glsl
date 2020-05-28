/*
	Author: Lesley van Hoek
	https://github.com/lshoek
*/

#version 450
#define inv(x) 1.0-x

layout(std430, binding=1) buffer inVerts {
	vec4 verts[];
};
layout(std430, binding=3) buffer outInds {
	int o_inds[];
};

uniform vec2 size;
uniform float faces_maxlen;

const int PRIMITIVE_RESTART_INDEX = 0xffffffff;
const int QUAD_INDICES = 6;
const float EPSILON = 0.001;

layout(local_size_x = 32, local_size_y = 32) in;

void main()
{
	ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
	int width = int(size.x);

	int sample_index = uv.y * width + uv.x;
	
	int topLeft = sample_index;
	int topRight = topLeft + 1;
	int bottomLeft = topLeft + width;
	int bottomRight = bottomLeft + 1;

	vec3 vTL = verts[topLeft].xyz;
	vec3 vTR = verts[topRight].xyz;
	vec3 vBL = verts[bottomLeft].xyz;
	vec3 vBR = verts[bottomRight].xyz;

	//upper left triangle
	if (vTL.z > EPSILON && vBR.z > EPSILON && vBL.z > EPSILON && 
		vTL.z < inv(EPSILON) && vBR.z < inv(EPSILON) && vBL.z < inv(EPSILON) && 
		abs(vTL.z - vBL.z) < faces_maxlen && 
		abs(vTL.z - vBR.z) < faces_maxlen)
	{
		o_inds[sample_index*QUAD_INDICES+0] = topLeft;
		o_inds[sample_index*QUAD_INDICES+1] = bottomRight;
		o_inds[sample_index*QUAD_INDICES+2] = bottomLeft;
	} 
	else {
		o_inds[sample_index*QUAD_INDICES+0] = PRIMITIVE_RESTART_INDEX;
		o_inds[sample_index*QUAD_INDICES+1] = PRIMITIVE_RESTART_INDEX;
		o_inds[sample_index*QUAD_INDICES+2] = PRIMITIVE_RESTART_INDEX;
	}

	//bottom right triangle
	if (vTL.z > EPSILON && vTR.z > EPSILON && vBR.z > EPSILON && 
		vTL.z < inv(EPSILON) && vTR.z < inv(EPSILON) && vBR.z < inv(EPSILON) && 
		abs(vBR.z - vTL.z) < faces_maxlen && 
		abs(vBR.z - vTR.z) < faces_maxlen)
	{
		o_inds[sample_index*QUAD_INDICES+3] = topLeft;
		o_inds[sample_index*QUAD_INDICES+4] = topRight;
		o_inds[sample_index*QUAD_INDICES+5] = bottomRight;
	}
	else {
		o_inds[sample_index*QUAD_INDICES+3] = PRIMITIVE_RESTART_INDEX;
		o_inds[sample_index*QUAD_INDICES+4] = PRIMITIVE_RESTART_INDEX;
		o_inds[sample_index*QUAD_INDICES+5] = PRIMITIVE_RESTART_INDEX;
	}
}
