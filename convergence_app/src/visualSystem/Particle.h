#pragma once

#include "ofGraphics.h"
#include "ofMain.h"


class Particle {
public:
	float x, y;
	float xv, yv;
	float xf, yf;
    float _particleBrightnessShift;
    bool wasRepeled, remove;
    ofColor color;
    float red, green, blue;
    
	Particle(float _x = 0, float _y = 0,
		float _xv = 0, float _yv = 0) :
		x(_x), y(_y),
		xv(_xv), yv(_yv) {
            
            wasRepeled = false;
            remove = false;
	}
	void updatePosition(float timeStep) {
		xv += xf;
		yv += yf;
		x += xv * timeStep;
		y += yv * timeStep;
	}
    
    void updateColor(float particleBrightnessShift){
        _particleBrightnessShift = particleBrightnessShift;
    }
    
	void resetForce() {
		xf = 0;
		yf = 0;
	}
    
    //when particle reaches edge of particle field, loop is around to the other side
    void loopAround(float left, float top, float right, float bottom, float bounce = .3) {
        
		if (x > right){
			x = left;
		} else if (x < left){
			x = right;
		}
        
		if (y > bottom){
            //BOTTOM
            y=bottom;
            yv *= -.9;
		} else if (y < top){
            //TOP
            //loop around
			y = top+1;
            //bounce off top?
            yv *= -bounce;
            remove = true;
            //delete particle if there's too many
            //delete this;
		}
	}
    
	void addDampingForce(float damping = .01) {
		xf = xf - xv * damping;
        yf = yf - yv * damping;
	}
    
	void draw() {
        
        /*float r = (float)(color.r) / 255;
        float g = (float)(color.g) / 255;
        float b = (float)(color.b) / 255;*/
        
        glColor3f(red,green,blue);
		glVertex2f(x, y);
	}
    
    void applyForce(ofVec2f f){
        xv += f.x;
        yv += f.y;
    }
    
    void repel(float amt){
        //BLACK = 0, want force to be 1
        //WHITE = 1, want for to be -1
        float f = (1-amt);
        
        if(wasRepeled)
            f*=-1;
        
        xv *= f;
        yv *= f;
        
        wasRepeled = true;
    }
    
    void stop(){
        xv = 0;
        yv = 0;
        resetForce();
    }
    
    void bounce(float damp){
        xv *= -damp;
        yv = 0;
        resetForce();
    }
    
    void setColor(ofColor c){
        //cout<<"set particle color: "<<c<<endl;
        color = c;
        red = (float)(color.r) / 255;
        green = (float)(color.g) / 255;
        blue = (float)(color.b) / 255;
        //color.b = 0;
    }
};
