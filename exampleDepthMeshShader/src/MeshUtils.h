#include "ofMesh.h"

// uses normalized texcoords
static ofMesh gridMesh(int w, int h, float scale)
{
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			float x_mesh = x * scale;
			float y_mesh = y * scale;

			mesh.addVertex(glm::vec3(x_mesh, y_mesh, 0));
			mesh.addTexCoord(glm::vec2((float)x / (float)w, (float)y / (float)h));
		}
	}

	for (int y = 0; y < h - 1; y++) {
		for (int x = 0; x < w - 1; x++) {
			mesh.addIndex(x + y * w);				// a
			mesh.addIndex((x + 1) + (y + 1) * w);	// c
			mesh.addIndex(x + (y + 1) * w);			// d

			mesh.addIndex(x + y * w);				// a
			mesh.addIndex((x + 1) + y * w);			// b
			mesh.addIndex((x + 1) + (y + 1) * w);	// c
		}
	}
	return mesh;
}
