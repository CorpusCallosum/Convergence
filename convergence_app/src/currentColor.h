//
//  currentColor.h
//  
//
//  Created by curry on 3/10/16.
//
//

#ifndef currentColor_h
#define currentColor_h

#include <stdio.h>
#include "ofMain.h"

class currentColor {
    
public:
    
    //methods
    void setup( float minutes, int n );
    void update();
    void draw( int x, int y );
    vector <ofColor> getCurrentColors();
    ofColor getCurrentColor(float i = 0);
    void loadGradientImage(string path);
    ofColor getCurrentColorFromImage(float offset = 0);
    
    //
    ofImage gradientImage;
    ofPixels gradientImagePixels;
    
    //variables
    vector<ofColor> colorPalette;
    vector <ofMesh> gradientZone;
    vector <float> zoneSize;
    
    int imgWidth, imgHeight;
    int nBars;
    
    ofImage colorPaletteImage;
    ofPixels colorPalettePixels;
    ofFbo fbo;
    
    vector <ofColor> currentColors;
    vector <ofColor> fromColors;
    vector <ofColor> toColors;
    vector <int> lastColors;
    vector <int> nextColors;
    
    float transSpeed; // moves 0 - 1 to transition fromColor toColor
    
    //color timers
    long startTime;
    long currentTime;
    long lastTime;
    long colorTime;
    int colorDuration; //how long each color lasts in ms
    int cycles;
    
    int cycleDuration;
    float colorPosition;
    
    
};

#endif
