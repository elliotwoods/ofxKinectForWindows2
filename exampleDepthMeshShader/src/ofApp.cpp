/*
	Author: Lesley van Hoek
	https://github.com/lshoek
*/

#include "ofApp.h"
#include "MeshUtils.h"

void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofLogToConsole();

	ofBackground(0x222222);
	ofSetHexColor(0xffffff);
	ofFill();

	kinect.open();
	bIsKinectOpen = kinect.isOpen();
	ofLog() << (bIsKinectOpen ? "Kinect found!" : "Kinect not found!");

	if (bIsKinectOpen) {
		kinect.initColorSource();
		kinect.initDepthSource();
		kinect.getSensor()->get_CoordinateMapper(&coordMapper);
	}
	else {
		OF_EXIT_APP(0);
	}

	// pointcloud options
	pointCloudOpts.textureCoordinates = ofxKFW2::Source::Depth::PointCloudOptions::TextureCoordinates::ColorCamera;
	pointCloudOpts.facesMaxLength = 0.0003f;
	pointCloudOpts.stitchFaces = true;
	pointCloudOpts.steps = 1;

	// init cam
	cam.setNearClip(0.0001f);
	cam.setFarClip(10.0f);
	cam.setPosition(0, 0, -.01f);
	cam.lookAt(glm::vec3(0));
	cam.setUpAxis(glm::vec3(0, 1, 0));
	cam.setAutoDistance(false);

	meshShader.setupShaderFromFile(GL_COMPUTE_SHADER, "shaders/mesh.glsl");
	meshShader.linkProgram();

	indexShader.setupShaderFromFile(GL_COMPUTE_SHADER, "shaders/indices.glsl");
	indexShader.linkProgram();
}

void ofApp::update()
{
	kinect.update();
	if (kinect.isFrameNew()) {

		auto depthSrc = kinect.getDepthSource();
		auto colorSrc = kinect.getColorSource();

		// update textures
		colorTexture = colorSrc->getTexture();
		depthTexture = depthSrc->getTexture();

		const int width = depthSrc->getWidth();
		const int height = depthSrc->getHeight();
		auto frameSize = width * height;
		
		const int w = depthSrc->getWidth() / pointCloudOpts.steps;
		const int h = depthSrc->getHeight() / pointCloudOpts.steps;
		meshWidth = w;
		meshHeight = h;

		// do all this once
		if (!bMeshDataInitialized) {

			// get depth-to-camera transform table
			depthSrc->getDepthToWorldTable(depthToCameraSpaceTable);

			// start with a base grid
			depthVboMesh = gridMesh(w, h, 1.0f);

			// allocate buffers
			depthToCameraSpaceBuffer.allocate();
			depthToCameraSpaceBuffer.setData(frameSize * sizeof(glm::vec2), depthToCameraSpaceTable.getData(), GL_STATIC_READ);

			vertexBuffer.allocate(w * h * sizeof(glm::vec4), GL_DYNAMIC_DRAW);
			indexBuffer.allocate(w * h * 6 * sizeof(int), GL_DYNAMIC_DRAW);

			// set buffers to vbo mesh (vec4 vertices)
			depthVboMesh.getVbo().setVertexBuffer(vertexBuffer, 4, 0);
			depthVboMesh.getVbo().setIndexBuffer(indexBuffer);

			// bind buffers (binding 2 is for depth tex)
			depthToCameraSpaceBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
			depthVboMesh.getVbo().getVertexBuffer().bindBase(GL_SHADER_STORAGE_BUFFER, 1);
			depthVboMesh.getVbo().getIndexBuffer().bindBase(GL_SHADER_STORAGE_BUFFER, 3);

			ofLog() << 
				"\nverts: " << depthVboMesh.getNumVertices() << 
				"\nindices:" << depthVboMesh.getNumIndices();

			bMeshDataInitialized = true;
		}

		// start timing here
		uint64_t start;
		start = ofGetElapsedTimeMillis();

		// update color-in-depth table
		depthSrc->getColorInDepthFrameMapping(colorToDepthSpaceTable);
		depthVboMesh.getVbo().updateTexCoordData((glm::vec2*) &colorToDepthSpaceTable[0], w * h);

		// update points
		depthTexture.bindAsImage(2, GL_READ_ONLY);

		meshShader.begin();
		meshShader.setUniform1i("steps", pointCloudOpts.steps);
		meshShader.setUniform2f("size", glm::vec2(w, h));
		meshShader.setUniform2f("size_full", glm::vec2(width, height));
		meshShader.dispatchCompute(w/32, h/32, 1);
		meshShader.end();

		depthTexture.unbind();

		// update indices
		indexShader.begin();
		indexShader.setUniform1f("faces_maxlen", pointCloudOpts.facesMaxLength);
		indexShader.setUniform2f("size", glm::vec2(w, h));
		indexShader.dispatchCompute(w/32, h/32, 1);
		indexShader.end();

		// end timer
		gpuMeshUpdateTimer = ofGetElapsedTimeMillis() - start;

		// Compare other approaches
		if (bCompareMeshUpdates) {
			start = ofGetElapsedTimeMillis();
			depthMesh = kinect.getDepthSource()->getMesh(pointCloudOpts);
			cpuMeshUpdateTimer = ofGetElapsedTimeMillis() - start;
		}
	}
}

void ofApp::draw()
{
	ofSetHexColor(0xffffff);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPrimitiveRestartIndex(0xffffffff);

	cam.begin();
	ofPushMatrix();

	// with texture
	if (bFull) {
		bool bTex = colorTexture.isAllocated();
		if (bTex) {
			colorTexture.setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
			colorTexture.bind();
		}
		depthVboMesh.draw();

		if (bTex) {
			colorTexture.unbind();
		}
	}
	// wireframe and points
	if (bWireFrame) {
		depthVboMesh.draw(ofPolyRenderMode::OF_MESH_WIREFRAME);
	}
	if (bPoints) {
		depthVboMesh.draw(ofPolyRenderMode::OF_MESH_POINTS);
	}
	ofPopMatrix();
	cam.end();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	ofSetColor(ofColor::fromHex(0x222222), 0x88);;
	ofDrawRectangle(0, 0, 512, 128);

	ofSetHexColor(0x00ffff);
	ofDrawBitmapString("[f] full mesh, [w] wireframe, [p] points, [t] time cpu mesh", margin, margin);
	ofDrawBitmapString("cam: " + ofToString(cam.getPosition()), margin, margin * 2);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()), margin, margin * 3);
	ofDrawBitmapString("gpu: " + ofToString(gpuMeshUpdateTimer) + "ms", margin, margin * 4);

	if (bCompareMeshUpdates) {
		ofDrawBitmapString("cpu: " + ofToString(cpuMeshUpdateTimer) + "ms", margin, margin * 5);
	}
}

void ofApp::keyPressed(int key)
{
	if (key == 'f') {
		bFull = !bFull;
	}
	if (key == 'w') {
		bWireFrame = !bWireFrame;
	}
	if (key == 'p') {
		bPoints = !bPoints;
	}
	if (key == 't') {
		bCompareMeshUpdates = !bCompareMeshUpdates;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
