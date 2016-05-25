//
//  visualSystem.h
//  JRHMemorialWall
//
//  Created by Jack Kalish on 7/7/14.
//
//

#pragma once
#include "ofMain.h"
#include "ParticleSystem.h"
#include "currentColor.h"
#include "utils.hpp"
#include "ofxXmlSettings.h"

class visualSystem{
    
    public:
    void init(int w=600, int h=400, int kparticles=15);
    void reset();
    ofFbo * getFrame();
    void loadTestMovie(string path);
    void update(float touched[36]);
    void mousePressed(int x, int y);
    void mouseReleased(int x, int y, int button);
    void mouseMoved(int x, int y);
    void emitParticle(int rod, int y, float yVel);
    int getRodX(int rod);
    void movePixelBar(int dir);

    
    ofVec2f getField(ofVec2f position);
    
    ofVideoPlayer 		testMovie;
    ofFbo *             display;
    
    int width, height, midline, lastNumAbove, maskHeight, maskX, particleEmitterCounter;
    
    //noise field
    float t;
    
    //particle system
    float timeStep, particleBrightnessShift;
    int lineOpacity, pointOpacity, numRods, rodSpacing, rodMargins;
	float centerAttraction;
    
	int kParticles, fadeAmt, pCounter;
	ParticleSystem particleSystem;
    
    //params
    float particleNeighborhood, particleRepulsion, pDampening, pStartVel, pBounce, vacuumRadius, vacuumPower;
    
    float complexity, pollenMass, timeSpeed;
    float phase, hForce, vForce, repForce;
    bool resetB, mixColor, showPixelBar;
    
    //fx
   //ofxGaussianBlur blur;
    
    float blurAmount, depthCutoff, depthForce, vForceFactor, colorStickiness;
    
    currentColor currentColor;
    
    utils u;
    
    
private:
    ofPixels displayPixels, textPixels;
    ofxXmlSettings xmlSettings;


};