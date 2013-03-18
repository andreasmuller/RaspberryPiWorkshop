#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(0);
	previousInputX = 0;
	previousInputY = 0;
	inputX = 0;
	inputY = 0;
	
	#ifndef TARGET_OPENGLES
		ofSetVerticalSync(true);
	#endif
	
	// billboard particles
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		#ifdef TARGET_OPENGLES
			pos.push_back(ofVec3f(ofRandomWidth(), ofRandomHeight(), 0));
		#else
			pos[i].x = ofRandomWidth();
			pos[i].y = ofRandomHeight();
		#endif
		vel[i].x = ofRandomf();
		vel[i].y = ofRandomf();
		home[i] = pos[i];
		pointSizes[i] = ofNextPow2(ofRandom(2, 40));
		rotations[i] = ofRandom(0, 360);
	}
	
	#ifdef TARGET_OPENGLES
		shader.load("BillboardGLES2");
		mesh.setUsage(GL_DYNAMIC_DRAW);
		// set the vertex data
		mesh.addVertices(pos);
	#else
		// set the vertex data
		vbo.setVertexData(pos, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
		shader.load("Billboard");
	#endif
	
	ofDisableArbTex();
	texture.loadImage("snow.png");
}

//--------------------------------------------------------------
void testApp::update() {
	#ifdef TARGET_OPENGLES
	previousInputX = inputX;
	previousInputY = inputY;
		inputX = ofRandom(ofGetWidth());
		inputY = ofRandom(ofGetHeight());				  
		ofVec2f mouse(inputX, inputY);
		ofVec2f mouseVec(previousInputX-inputX, previousInputY-inputY);
	#else
		ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
		ofVec2f mouseVec(ofGetPreviousMouseX()-ofGetMouseX(), ofGetPreviousMouseY()-ofGetMouseY());
	#endif
	
	mouseVec.limit(10.0);
	
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		#ifdef TARGET_OPENGLES
			ofVec3f &point = mesh.getVertices()[i];
		#else
			ofSeedRandom(i);
			ofVec2f &point = pos[i];
		#endif
		
		
		if(mouse.distance(point) < ofRandom(100, 200)) {
			vel[i] -= mouseVec; 
		}
		
		point += vel[i];
		vel[i] *= 0.84f;
		
		if(point.x < 0) point.x = ofGetWidth();
		if(point.x > ofGetWidth()) point.x = 0;
		if(point.y < 0) point.y = ofGetHeight();
		if(point.y > ofGetHeight()) point.y = 0;
		
		ofVec2f center(ofGetWidth()/2, ofGetHeight()/2);
		ofVec2f frc = home[i] - point;
		if(frc.length() > 20.0) {
			frc.normalize();
			frc *= 0.84;
			vel[i] += frc;
		}
		
		// get the 2d heading
		float angle = (float)atan2(-vel[i].y, vel[i].x) + PI;
		rotations[i] = (angle * -1.0);
	}
	//ofSeedRandom(1);//set random back
}

//--------------------------------------------------------------
void testApp::draw() {
	ofEnableAlphaBlending();
	ofSetColor(255);
	
	ofEnablePointSprites();
	shader.begin();
	
	// we are getting the location of the point size attribute
	// we then set the pointSizes to the vertex attritbute
	// we then bind and then enable
	int pointAttLoc = shader.getAttributeLocation("pointSize");
	glVertexAttribPointer(pointAttLoc, 1, GL_FLOAT, false, 0, pointSizes);
	glBindAttribLocation(shader.getProgram(), pointAttLoc, "pointSize");
	glEnableVertexAttribArray(pointAttLoc);
	
	// rotate the snow based on the velocity
	int angleLoc = shader.getAttributeLocation("angle");
	glVertexAttribPointer(angleLoc, 1, GL_FLOAT, false, 0, rotations);
	glBindAttribLocation(shader.getProgram(), angleLoc, "angle");
	glEnableVertexAttribArray(angleLoc);
	
	texture.getTextureReference().bind();
	#ifdef TARGET_OPENGLES
		mesh.drawVertices();
	#else
		vbo.updateVertexData(pos, NUM_BILLBOARDS);
		vbo.draw(GL_POINTS, 0, NUM_BILLBOARDS);
	#endif
	
	texture.getTextureReference().unbind();
	
	shader.end();
	ofDisablePointSprites();
	
	// disable vertex attributes
	glDisableVertexAttribArray(pointAttLoc); 
	glDisableVertexAttribArray(angleLoc);
	
	ofSetColor(ofColor::whiteSmoke);
	ofCircle(inputX, inputY, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'f') {
		ofToggleFullscreen();
	}
}
void testApp::exit()
{
	shader.unload();
	ofLogVerbose() << "dumping shader before we leave";
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		home[i].x = ofRandomWidth();
		home[i].y = ofRandomHeight();
	}
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
