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

class visualSystem{
    
    public:
    void init(int w=600, int h=400, int kparticles=15);
    void reset();
    ofFbo * getFrame();
    void loadTestMovie(string path);
    void update();
    void mousePressed(int x, int y);
    void mouseReleased(int x, int y, int button);
    void mouseMoved(int x, int y);
    ofVec2f getField(ofVec2f position);
    
    ofVideoPlayer 		testMovie;
    ofFbo *             display;
    
    int width, height;
    
    //noise field
    float t;
    
    //particle system
    float timeStep, particleBrightnessShift;
	int lineOpacity, pointOpacity;
	float particleNeighborhood, particleRepulsion;
	float centerAttraction;
    
	int kParticles, fadeAmt;
	ParticleSystem particleSystem;
    
    float complexity, pollenMass, timeSpeed;
    float phase, hForce, vForce, repForce;
    bool resetB;
    
    
    //fx
   //ofxGaussianBlur blur;
    
    float blurAmount, depthCutoff, depthForce;
    
    
private:
    ofPixels displayPixels, textPixels;

};