//
//  gui.hpp
//  convergence_app
//
//  Created by Jack Kalish on 5/5/16.
//
//

#include "ofMain.h"
#include "ofxGui.h"

#ifndef gui_hpp
#define gui_hpp

#include <stdio.h>

#endif /* gui_hpp */


class gui{
    
public:
    gui();
    void setup(int x=0);
    void update();
    void draw();
    
    ofxPanel guiPanel;
    ofParameterGroup visualSystemParams, dsParams;
    ofParameter<float> flowSpeed, timeSpeed, flowComplexity, horizontalForce, verticalForce, particleNeighborhood, particleRepulsion;
    ofParameter<int> fadeAmt, blur, ledStripHue, ledStripBrightness, ledStripSaturation, particleBrightnessShift;
    ofParameter<ofColor> ledStripsColor;
    
    string xmlFile;
};