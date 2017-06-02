//
//  fft.cpp
//  convergence_app
//
//  Created by Jack Kalish on 6/2/17.
//
//

#include "fftAnalyzer.hpp"

void fftAnalyzer::setup() {
    cout<< "fftAnalyzer.setup" <<endl;
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    fft.setup(16384);
    averageFrequency = 0;
}

void fftAnalyzer::update() {
    cout<< "fftAnalyzer.update" <<endl;
    fft.update();
}

void fftAnalyzer::draw(int x, int y) {
    //ofBackground(0, 0, 0);
    cout<< "fftAnalyzer.draw" <<endl;
    ofPushMatrix();
    ofTranslate(x, y);
    ofSetColor(255);
    ofDrawBitmapString("Frequency Domain", 0, 0);
    plot(fft.getBins(), 128);
    ofPopMatrix();
    
    string msg = ofToString((int) ofGetFrameRate()) + " fps";
    ofDrawBitmapString(msg, ofGetWidth() - 80, ofGetHeight() - 20);
}

void fftAnalyzer::plot(vector<float>& buffer, float scale) {
    cout<< "fftAnalyzer.plot" <<endl;
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
    averageFrequency = af;
    ofSetColor(100, 255, 255);
    string msg = "average frequency: " + ofToString(af);
    ofDrawBitmapString(msg, ofGetWidth() - 300, 20);
    
    //draw line for average...
    ofDrawLine(af, 0, af, 100);
    ofSetColor(255);
}
