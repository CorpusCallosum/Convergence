//
//  visualSystem.h
//  JRHMemorialWall
//
//  Created by Jack Kalish on 6/25/14.
//
//

#pragma once
#include "ofMain.h"

class displaySystem{
    public:
    
    displaySystem();
    void init();
    void draw();
    void updateDisplay(ofFbo * frame);
    void clear();
    
    int width, height;
    
    private:
    ofFbo * _frame;

};