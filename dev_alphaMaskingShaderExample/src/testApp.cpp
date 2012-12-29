#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
    ofEnableAlphaBlending();
	brushX=0;
	brushY=20;
    srcImg.loadImage("A.jpg");
    dstImg.loadImage("B.jpg");
    brushImg.loadImage("brush.png");
    
    int width = srcImg.getWidth();
    int height = srcImg.getHeight();
    
    maskFbo.allocate(width,height);
    fbo.allocate(width,height);

    bool didLoad = shader.load("myShader.vert", "myShader.frag", "");
	
	ofLogVerbose() << "didLoad: " << didLoad;
    // Let«s clear the FBO«s
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
    
    // MASK (frame buffer object)
	brushX+=5;
	if (brushX>srcImg.getWidth()) {
		brushY+=20;
		brushX=0;
	}
	if (brushY>srcImg.getHeight()) {
		maskFbo.begin();
		ofClear(0,0,0,255);
		maskFbo.end();
		brushY = 20;
	}
	
	
    maskFbo.begin();
    brushImg.draw(brushX-25,brushY-25,50,50);
    maskFbo.end();
    
    // HERE the shader-masking happends
    //
    fbo.begin();
    // Cleaning everthing with alpha mask on 0 in order to make it transparent for default
    ofClear(0, 0, 0, 0); 
    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
    
    srcImg.draw(0,0);
	
    shader.end();
    fbo.end();
	
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255,255);
    dstImg.draw(0,0);
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