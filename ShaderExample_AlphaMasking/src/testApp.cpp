#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofHideCursor();
    ofEnableAlphaBlending();
	
	brushX=0;
	brushY=20;
	
    sourceImage.loadImage("A.jpg");
    targetImage.loadImage("B.jpg");
    brushImage.loadImage("brush.png");
    
    int width = sourceImage.getWidth();
    int height = sourceImage.getHeight();
    
    maskFbo.allocate(width,height);
    fbo.allocate(width,height);

    bool didLoad = shader.load("AlphaMasking_GLES.vert", "AlphaMasking_GLES.frag", "");
	
	ofLogVerbose() << "didLoad: " << didLoad;
	
    // Clear the FBOs
    // otherwise it will bring some junk with it from the memory    
    maskFbo.begin();
		ofClear(0,0,0,255);
    maskFbo.end();
    
    fbo.begin();
		ofClear(0,0,0,255);
    fbo.end();
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    //No mouse so move the brush manually
	brushX+=5;
	
	if (brushX>sourceImage.getWidth())
	{
		brushY+=20;
		brushX=0;
	}
	if (brushY>sourceImage.getHeight())
	{
		maskFbo.begin();
			ofClear(0,0,0,255);
		maskFbo.end();
		brushY = 20;
	}
	
	
    maskFbo.begin();
		brushImage.draw(brushX-25,brushY-25,50,50);
    maskFbo.end();
    
    // HERE the shader-masking happens
    fbo.begin();
		// Clear everthing with alpha mask of 0 in order to make it transparent
		ofClear(0, 0, 0, 0); 
		shader.begin();
			shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
			sourceImage.draw(0,0);
		shader.end();
    fbo.end();
	
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255,255);
    targetImage.draw(0,0);
	fbo.draw(0,0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
   
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