/*
	Author: Lesley van Hoek
	https://github.com/lshoek
*/

#version 450
#define inv(x) 1.0-x

layout(std430, binding=0) buffer dctable {
	vec2 dc_coord[];
};
layout(std430, binding=1) buffer outVerts {
	vec4 o_verts[];
};
layout(r16, binding=2) uniform readonly image2D depth;

uniform int steps;
uniform vec2 size;
uniform vec2 size_full;

layout(local_size_x = 32, local_size_y = 32) in;

void main()
{
	// coord of the output mesh
	ivec2 uv = ivec2(gl_GlobalInvocationID.xy);

	// coord of uv in full depth texture
	ivec2 uv_full = ivec2(uv*steps);

	// get depth value from texture
	float d = imageLoad(depth, uv_full).r;

	// calculate sample indices
	int sample_index = uv.y * int(size.x) + uv.x;
	int sample_index_full = uv_full.y * int(size_full.x) + uv_full.x;

	// get coord at current vertex
	vec2 depthcoord_raw = dc_coord[sample_index_full].xy;

	// coords are proportional to depth
	vec2 depthcoord = depthcoord_raw * d;
	//d = mix(d, 1.0, step(d, 0.01));
	
	// update the vertex position
	o_verts[sample_index] = vec4(depthcoord.x, depthcoord.y, d, 1.0);
}
