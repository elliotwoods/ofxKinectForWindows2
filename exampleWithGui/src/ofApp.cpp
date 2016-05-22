#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodyIndexSource();
	kinect.initBodySource();

	gui.init();

	//setup a gui panel for the widgets at top-left
	auto widgets = gui.addWidgets();
	widgets->addTitle("ofxKinectForWindows2");
	widgets->addFps();
	widgets->addMemoryUsage();

	//setup a gui panel for the 3D view
	auto worldView = gui.addWorld("World");
	worldView->onDrawWorld += [this](ofCamera &) {
		this->kinect.drawWorld();
	};

	//setup a gui panel for every kinect source
	auto sources = kinect.getSources();
	for(auto source : sources) {
		auto sourceWithTexture = dynamic_pointer_cast<ofBaseHasTexture>(source);
		if (sourceWithTexture) {
			auto panel = gui.add(sourceWithTexture->getTexture(), source->getTypeName());

			//if it's the colour panel, let's do something special by writing some info on top
			auto colorSource = dynamic_pointer_cast<ofxKFW2::Source::Color>(source);
			if (colorSource) {
				panel->onDraw += [colorSource] (ofxCvGui::DrawArguments &) {
					stringstream message;
					message << "Exposure : " << colorSource->getExposure() << "us" << endl;
					message << "FrameInterval : " << colorSource->getFrameInterval() << "us" << endl;
					message << "Gain : " << colorSource->getGain() << endl;
					message << "Gamma : " << colorSource->getGamma() << endl;
					ofxCvGui::Utils::drawText(message.str(), 20, 60);
				};
			}

			//if it's the depth panel, set some scaling
			auto depthSource = dynamic_pointer_cast<ofxKFW2::Source::Depth>(source);
			if (depthSource) {
				auto style = make_shared<ofxCvGui::Panels::Texture::Style>();
				style->rangeMaximum = 0.25f;
				panel->setStyle(style);
			}

			//if it's the body index panel, let's draw the joints on top
			auto bodyIndexSource = dynamic_pointer_cast<ofxKFW2::Source::BodyIndex>(source);
			if(bodyIndexSource) {
				panel->onDrawImage += [this](ofxCvGui::DrawImageArguments & args) {
					auto bodySource = this->kinect.getBodySource();
					const auto & bodies = bodySource->getBodies();

					ofPushStyle();
					{
						ofColor color(200, 100, 100);
						int index = 0;
						for (const auto & body : bodies) {
							color.setHueAngle((index * 50) % 360);
							ofSetColor(color);
							for (const auto & joint : body.joints) {
								ofDrawCircle(joint.second.getPositionInDepthMap(), 5);
							}
							index++;
						}
					}
					ofPopStyle();
				};
			}
		}

		//add an 'isFrameNew' widget to the panel
		widgets->addIndicatorBool(source->getTypeName() + " frame new", [source]() {
			return source->isFrameNew();
		});
	}

	//if we press the 'c' key on the World panel, then toggle the camera's cursor. This works best when you fullscreen that panel
	worldView->onKeyboard += [this, worldView] (ofxCvGui::KeyboardArguments & args) {
		if (args.action == ofxCvGui::KeyboardArguments::Action::Pressed && args.key =='c') {
			worldView->getCamera().toggleCursorDrawEnabled();
		}
	};
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
	mesh = kinect.getDepthSource()->getMesh(true, ofxKinectForWindows2::Source::Depth::PointCloudOptions::TextureCoordinates::ColorCamera);
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
