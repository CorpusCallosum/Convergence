//
//  visualSystem.cpp
//  JRHMemorialWall
//
//  Created by Jack Kalish on 7/7/14.
//
//



#include "visualSystem.h"



void visualSystem::init(int w, int h, int kParticles){
    width=w;
    height=h;
    particleBrightnessShift = 10;
    mixColor = true;
    pCounter = 0;

    
    //blur.allocate(width, height);
    display = new ofFbo();
    display->allocate(width,height,GL_RGB);
    
    //particle system
    // this number describes how many bins are used
	// on my machine, 2 is the ideal number (2^2 = 4x4 pixel bins)
	// if this number is too high, binning is not effective
	// because the screen is not subdivided enough. if
	// it's too low, the bins take up so much memory as to
	// become inefficient.
	int binPower = 2;
    
	particleSystem.setup(width, height, binPower);
    
	//kParticles = 15;
	float padding = 0;
	float maxVelocity = 5;
	for(int i = 0; i < kParticles * 1024; i++) {
		float x = ofRandom(padding, width - padding);
		float y = ofRandom(padding, height - padding);
		//float xv = ofRandom(-maxVelocity, 0);
		//float yv = ofRandom(-maxVelocity, maxVelocity);
        
		Particle particle(x, y);
        ofColor c;
        particle.setColor(c);
		particleSystem.add(particle);
	}
    
	ofBackground(0, 0, 0);
    
	timeStep = .3;
	lineOpacity = 100;
	pointOpacity = 255;
	particleNeighborhood = 4;
	particleRepulsion = 0;
	centerAttraction = .01;
    
    /*
     All these settings control the behavior of the app. In general it's a better
     idea to keep variables in the .h file, but this makes it easy to set them at
     the same time you declare them.
     */
    complexity = 2; // wind complexity
    pollenMass = .8; // pollen mass
    timeSpeed = .002; // wind variation speed
    phase = TWO_PI; // separate u-noise from v-noise
    hForce = .2;
    vForce = .2;
    
    currentColor.setup();
    
}

void visualSystem::reset(){
    
}

void visualSystem::update(bool touched[36]){
    
    currentColor.update();
    
    ofColor c;
    c.r = 255;
    c.g = 100;
    c.b = 50;
    
    
    //PARTICLE EMITER*******************************************************
    for(int i=0; i < numRods; i++){
        if(touched[i] == true){
            
                //emit a particle
                //This method takes a partiicle we already have and moves them around, rather than creating new particles
              /*  Particle& particle = particleSystem[pCounter];
                //iterate the particle counter
                pCounter ++;
                if(pCounter >= particleSystem.size())
                    pCounter = 0;
                }*/
            
            
            particleSystem.erase(0);

            //This method creates a new particle
            Particle particle(0,0); //CREATE NEW PARTICLE
            
            particle.setColor(currentColor.getCurrentColor());
            particle.x = i*rodSpacing+rodMargins;
            particle.y = height - 2*64; //2 meters from the top
            particle.yv = pStartVel; //initial velocity
            particle.xv = 0;
            particleSystem.add(particle);
            
            
        }
    }
    
    particleSystem.setTimeStep(timeStep);
    t = ofGetFrameNum() * timeSpeed;
    
    // display->readToPixels(displayPixels);
    
    //draw to FBO
    display->begin();

    //ofEnableAlphaBlending();
    //ofBackground(0,0,0,100);
    ofFill();
    
    //fade out BG by drawing a rectangle
    ofSetColor(0, 0, 0, fadeAmt);
    ofRect(0,0,width,height);

    //PARTICLE SYSTEM DRAWING STARTS HERE
    // if(isOn){
	ofSetColor(lineOpacity, lineOpacity, lineOpacity, 255);
	particleSystem.setupForces();
    
	// apply per-particle forces
	glBegin(GL_LINES);
    ofVec2f pos;
    
	for(int i = 0; i < particleSystem.size(); i++) {
		Particle& cur = particleSystem[i];
        
		// particle force on other particles
        float f = 1-cur.y/height;
		particleSystem.addRepulsionForce(cur, particleNeighborhood*f, particleRepulsion*f);
        
        cur.loopAround(0,0,width,height, pBounce);
		cur.addDampingForce(pDampening); //slows the particle down
        
        //apply noise field force to the particle
        pos.set(cur.x,cur.y);
        cur.applyForce(getField(pos));
        //cur.updateColor(particleBrightnessShift);
        
        if(mixColor){
        //particle color mix!
        vector<Particle*> neighbors = particleSystem.getNeighbors(cur, particleNeighborhood/10);
        
        for(int n = 0; n < neighbors.size(); n++) {
            //average colors
            ofColor c;
            c.r = ((neighbors[n]->red + cur.red)/2)*255;
            c.g = ((neighbors[n]->green + cur.green)/2)*255;
            c.b = ((neighbors[n]->blue + cur.blue)/2)*255;
            
            float r = round(random());
            
            /*if(r)
               neighbors[n]->setColor(cur.color);
            else
               cur.setColor(neighbors[n]->color);*/
            
            neighbors[n]->setColor(c);
        }
        }
        
        //DELETE PARTICLES
        if(cur.remove)
            particleSystem.erase(i);
        
	}
	glEnd();
	
	particleSystem.update();
    
    ofSetColor(pointOpacity, pointOpacity, pointOpacity, 255);
    particleSystem.draw();
    
    display->end();

    
   // if(isOn){
        //apply the blur
   // blur.setTexture(display->getTextureReference());
   // blur.setRadius(blurAmount);
   // blur.update();

    //Apply blur FX
    display->begin();
        
        //need to enable these special blame functions in order to properly blend apha PNGs
      /*  glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);*/
        
        ofClearAlpha();
        ofSetColor(255,255,255,255);
        
    //draw the blurred particle system
   // blur.draw(0,0);
    
       // ofDisableAlphaBlending();
       /* glDisable(GL_BLEND);
        glPopAttrib();*/
    display->end();
    //}
    
    resetB = false;
}

ofFbo * visualSystem::getFrame(){    
    return display;
}

/*
 This is the magic method that samples a 2d slice of the 3d noise field. When
 you call this method with a position, it returns a direction (a 2d vector). The
 trick behind this method is that the u,v values for the field are taken from
 out-of-phase slices in the first dimension: t + phase for the u, and t - phase
 for the v.
 */
//--------------------------------------------------------------
ofVec2f visualSystem::getField(ofVec2f position) {
	float normx = ofNormalize(position.x, 0, width);
	float normy = ofNormalize(position.y, 0, height);
    
	float h = -ofNoise(t + phase, normx * complexity + phase, normy * complexity + phase);
	float v = .5-ofNoise(t - phase, normx * complexity - phase, normy * complexity + phase);
    
    h*=hForce;
    v+=vForce;
    
	return ofVec2f(h, v);
}
