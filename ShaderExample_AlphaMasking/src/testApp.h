#pragma once

//GLES2 version of examples/gl/alphaMaskingShaderExample

#include "ofMain.h"

class testApp : public ofBaseApp{
public:
    
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	
    ofImage     sourceImage;
    ofImage     targetImage;
    ofImage     brushImage;
    
    ofFbo       maskFbo;
    ofFbo       fbo;
    
    ofShader    shader;
    
    bool        bBrushDown;
	int brushX;
	int brushY;
};
