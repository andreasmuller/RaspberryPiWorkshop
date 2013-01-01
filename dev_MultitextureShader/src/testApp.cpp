#include "testApp.h"

ofImage tex0Source;
ofImage tex1Source;

ofTexture tex0;
ofTexture tex1;

float drawWidth;
float drawHeight;

ofShader shader;
ofFbo fbo;

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofEnableTextureEdgeHack();
	tex0Source.loadImage("brick.jpg");
	tex1Source.loadImage("tex1.jpg");
	
	tex0 = tex0Source.getTextureReference();
	tex1 = tex1Source.getTextureReference();
	tex1.setTextureWrap(GL_REPEAT, GL_REPEAT);
	tex0.setTextureWrap(GL_REPEAT, GL_REPEAT);
	
	shader.load("Multitexture.vert", "Multitexture.frag", "");
	
	drawWidth = tex0Source.getWidth();
	drawHeight = tex0Source.getHeight();
	
	fbo.allocate(drawWidth, drawHeight);
	fbo.begin();
		ofClear(0, 0, 0, 0);
	fbo.end();
	
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::update(){
	fbo.begin();
		ofClear(0, 0, 0, 0);
		shader.begin();
			shader.setUniform1f("time", ofGetElapsedTimef());
			shader.setUniform2f("resolution", drawWidth, drawHeight);
			shader.setUniformTexture("tex0", tex0, 1);
			shader.setUniformTexture("tex1", tex1, 2);
			ofRect(0, 0, drawWidth, drawHeight);
		shader.end();
	fbo.end();
}

//--------------------------------------------------------------
void testApp::draw(){
	fbo.draw(0, 0);

}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){


}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

