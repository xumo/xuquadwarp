#include "testApp.h"
//#include "ofxVectorMath.h"
#include "ofxSimpleGuiToo.h"





ofFloatColor	aColor;


// for demonstrating adding any drawable object (that extends ofBaseDraw);
ofVideoGrabber		vidGrabber;
unsigned char * 	videoInverted;
ofTexture			videoTexture;

ofPoint            puntos[4];
bool	randomizeButton = true;

ofPoint *points;
ofPoint	v[300];

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	
	// for demonstrating adding any drawable object (that extends ofBaseDraw);
	vidGrabber.initGrabber(320, 240);
	videoInverted 	= new unsigned char[int(vidGrabber.getWidth() * vidGrabber.getHeight() * 3)];
	videoTexture.allocate(vidGrabber.getWidth(), vidGrabber.getHeight(), GL_RGB);
	
    quad.setSuperficie(vidGrabber.getTextureReference());
}

//--------------------------------------------------------------
void testApp::update(){
	
	
	// from ofVideoGrabber example (
	vidGrabber.update();
	
}

//--------------------------------------------------------------
void testApp::draw(){
	//ofBackground(aColor.r * 255, aColor.g * 255.0f, aColor.b * 255.0);
    ofBackground(40);
    
    quad.draw();
	
}

//--------------------------------------------------------------
void testApp::keyPressed (int key){

}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    quad.mouseDrag(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    quad.mouseDown(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	quad.mouseUp();
}
