#include "testApp.h"
#include "MSAInterpolator.h"


msa::Interpolator2D				spline2D;
msa::Interpolator3D				spline3D;

msa::InterpolationType			interpolationType	= msa::kInterpolationCubic;
bool							useLength			= false;

float							currentRot;
bool							rotateView;

float							spherePosPerc	= 0;			// 0....1 percentage of how far along the 3D path the sphere is
float							sphereSpeed = 0.005f;


//--------------------------------------------------------------
void testApp::setup(){	
	ofSetVerticalSync(true);
	rotateView = true;
	currentRot = 0;
	
	// create a 2D spline with ofVec2f's
	int numItems = 10;
	int padding = 30;
	float len = (ofGetWidth() - padding*2.0f) / numItems;
	
	spline2D.reserve(numItems);		// not essential, but good habit if you know how big its gonna be
	for(int i=0; i<numItems; i++) {
		ofVec2f v = ofVec2f(i * len + padding - ofGetWidth()/2, ofGetHeight()/2 + ofGetHeight()*0.2f * cos(i*0.9));
		spline2D.push_back(v);
	}	
	
	
	spline3D.verbose = true;
	
	glEnable(GL_DEPTH_TEST);
}


//--------------------------------------------------------------
void testApp::draw() {
	glPushMatrix();
	glTranslatef(ofGetWidth()/2, 0, 0);		// move to center of screen horizontally (so we can rotate)
	
	if(rotateView) {
		currentRot += 0.2;
		glRotatef(currentRot, 0, 1, 0);		// rotate view
	} else {
		currentRot = 0;
	}
	
	
	int numSteps = floor(mouseX / (float)ofGetWidth() * 1000);
	if(numSteps<10) numSteps = 10;
	float spacing = 1.0/numSteps;	
	
	// draw spline2D
	glColor3f(1, 1, 1);
	drawInterpolatorRaw(spline2D);
	
	// draw interpolated spline2D
	glColor3f(0, 0, 1);
	drawInterpolatorSmooth(spline2D, numSteps);
	
	// draw raw spline3D
	glColor3f(1, 1, 1);
	drawInterpolatorRaw(spline3D);
	
	// draw interpolated spline3D
	glColor3f(0.2f, 0.2f, 0.2f);
	drawInterpolatorSmooth(spline3D, numSteps);
	
	
	
	// draw sphere moving along 3D path
	ofVec3f spherePos = spline3D.sampleAt(spherePosPerc);
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(spherePos.x, spherePos.y, spherePos.z);
    ofSphere(5);
	glPopMatrix();
	
	// move sphere
	// if it reaches the edges, bounce back
	spherePosPerc += sphereSpeed;
	if(spherePosPerc > 1) {
		spherePosPerc = 1;
		sphereSpeed *= -1;
	} else if(spherePosPerc < 0) {
		spherePosPerc = 0;
		sphereSpeed *= -1;
	}
	

	glPopMatrix();

	ofSetColor(0);
	string uiLin = interpolationType == msa::kInterpolationLinear ? "* " : "  ";
	string uiCub = interpolationType == msa::kInterpolationCubic ? "* " : "  ";
	string uiDist = spline3D.getUseLength() ? "* " : "  ";
	ofDrawBitmapString( ofToString(ofGetFrameRate(), 2) + "\n"
					   + "numSteps (resampling resolution - mouseX to change): " + ofToString(numSteps) + "\n"
					   + "mouse click around the area to draw a 3D spline (length = " + ofToString(spline3D.getLength()) + "\n"
					   + "\n"
					   + uiLin + "'1' to use linear interpolation\n"
					   + uiCub + "'2' to use cubic (catmull rom) interpolation\n"
					   + "\n"
					   + uiDist + "'d' to toggle 'using Length in interpolation'\n"
					   + "\n"
					   + "'c' to clear 3D spline\n"
					   , 20, 20);
	
}

void testApp::keyPressed(int key) {
	switch(key) {
		case '1':
			interpolationType = msa::kInterpolationLinear;
			spline3D.setInterpolation(interpolationType);
			spline2D.setInterpolation(interpolationType);
			break;
		case '2':
			interpolationType = msa::kInterpolationCubic;
			spline3D.setInterpolation(interpolationType);
			spline2D.setInterpolation(interpolationType);
			break;
			
		case 'd':
			useLength ^=true;
			spline3D.setUseLength(useLength);
			spline2D.setUseLength(useLength);
			break;
			
		case 'c':
		case 'C':
			spline3D.clear();
			break;
			
		case 'r':
		case 'R':
			rotateView ^= true;
			break;
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	// when you add a new point, the length of the spline increases
	// so the sphere will jump to a new position because it's position is based on percentage
	// so a little bit of maths to calculate what the new position percentage should be to stay at the same physical location
	// (not precise, but close enough)
	int numPoints = spline3D.size();
	spherePosPerc = spherePosPerc * numPoints / (numPoints + 1);
	
	
	ofVec3f pt(x-ofGetWidth()/2, y, 0);
	pt.rotate(-currentRot, ofVec3f(0, 1, 0));
	spline3D.push_back(pt);
}

