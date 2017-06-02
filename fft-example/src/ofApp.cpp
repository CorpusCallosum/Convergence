#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);	
	ofSetFrameRate(60);
	fft.setup(16384);
}

void ofApp::update() {
	fft.update();
}

void ofApp::draw() {
	ofBackground(0, 0, 0);
	
	ofPushMatrix();
	ofTranslate(16, 16);
	ofSetColor(255);
	ofDrawBitmapString("Frequency Domain", 0, 0);
	plot(fft.getBins(), 128);
	ofPopMatrix();
	
	string msg = ofToString((int) ofGetFrameRate()) + " fps";
	ofDrawBitmapString(msg, ofGetWidth() - 80, ofGetHeight() - 20);
}

void ofApp::plot(vector<float>& buffer, float scale) {
	ofNoFill();
	int n = MIN(1024, buffer.size());
	ofDrawRectangle(0, 0, n, scale);
	ofPushMatrix();
	ofTranslate(0, scale);
	ofScale(1, -scale);
	ofBeginShape();
    
    
    float af = 0;
    float aSum = 0; //sum of all amplitudes
	for (int i = 0; i < n; i++) {
		ofVertex(i, buffer[i]);
        af += i*buffer[i];
        aSum += buffer[i];
	}
    
	ofEndShape();
	ofPopMatrix();
    
    //calculate average frequency...
    af = af/aSum;
    af = af;
    ofSetColor(100, 255, 255);
    string msg = "average frequency: " + ofToString(af);
    ofDrawBitmapString(msg, ofGetWidth() - 300, 20);
    
    //draw line for average...
    ofDrawLine(af, 0, af, 100);
    ofSetColor(255);
}
