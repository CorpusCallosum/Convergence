#pragma once

#include "ofMain.h"
#include "ofxArtNet.h"
#include "visualSystem.h"
#include "displaySystem.h"
#include "gui.hpp"

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
    
    ofxArtnet artnet;
    ofImage testImage;
    ofFbo fbo;
    ofColor color_array[ 512 ];
    
    bool rainbow;
    
    displaySystem ds;
    visualSystem  vs;
    ofFbo * frame;
    
    gui gui;
    
    int visualSystemWidth;

};