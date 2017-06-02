#pragma once

#include "ofMain.h"
#include "visualSystem.h"
#include "displaySystem.h"
#include "gui.hpp"
#include "serialReceiver.h"
#include "ofxXmlSettings.h"
#include "fftAnalyzer.hpp"

class ofApp : public ofBaseApp{
    
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
    void exit();
    
    void audioIn(float * input, int bufferSize, int nChannels);
    
    ofxArtnet artnet;
    
    displaySystem ds;
    visualSystem  vs;
    ofFbo * frame;
    
    gui gui;
    ofxXmlSettings settings;
    
    ofSoundStream soundStream;
    fftAnalyzer fft;
    
    
    int visualSystemWidth, visualSystemHeight, numRods, rodMargins, maskHeight;
    float rodSpacing;
    
    serialReceiver serialReceiver;
    

};
