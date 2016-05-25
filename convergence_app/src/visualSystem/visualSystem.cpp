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
    pCounter = 0;
    midline =  1.828 * 60; //6' from the top
    particleEmitterCounter = 0;
    maskX = 0;
    showPixelBar = false;
    
    currentColor.setup( 2, 100 );
    currentColor.loadGradientImage("gradient.png");
    
    //blur.allocate(width, height);
    display = new ofFbo();
    display->allocate( width, height, GL_RGB );
    
    //particle system
    // this number describes how many bins are used
	// on my machine, 2 is the ideal number (2^2 = 4x4 pixel bins)
	// if this number is too high, binning is not effective
	// because the screen is not subdivided enough. if
	// it's too low, the bins take up so much memory as to
	// become inefficient.
	int binPower = 2;
    
	particleSystem.setup(width, height, binPower);
    particleSystem.midline = midline;
    
    //INSTANTIATE PARTICLES AT START
	//kParticles = 15;
	float padding = 0;
	float maxVelocity = 5;
	for(int i = 0; i < kParticles * 1024; i++) {
		float x = ofRandom(padding, width - padding);
		//float y = ofRandom(padding, height - padding);
        float y = ofRandom(height-10, height);
		Particle particle(x, y);
        
        //color it
        //int cNum = i % currentColor.colorPalette.size();
        //ofColor c = currentColor.colorPalette[cNum];
        ofColor c; //start color is white
        particle.setColor(c);
        
        //add it
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
    
    //load settings
    xmlSettings.loadFile("settings.xml");
    
    //load pixel bar pos from XML
    xmlSettings.pushTag("PixelPatcher");
    maskHeight = xmlSettings.getAttribute("PatchBar", "y", 0);
    xmlSettings.popTag();

    
}

void visualSystem::reset(){
    
}

void visualSystem::update(float touched[36]){
    
    currentColor.update();
    
    particleSystem.setTimeStep(timeStep);
    t = ofGetFrameNum() * timeSpeed;
    
    //draw to FBO
    display->begin();

    ofFill();
    
    //fade out BG by drawing a rectangle
    ofSetColor(0, 0, 0, fadeAmt);
    ofDrawRectangle( 0, 0, width,height);

    //PARTICLE SYSTEM DRAWING STARTS HERE
	ofSetColor(lineOpacity, lineOpacity, lineOpacity, 255);
	particleSystem.setupForces();
    
    //PARTICLE EMITER*******************************************************
    for(int i=0; i < numRods; i++){
        if(touched[i]>2){
             emitParticle(i, midline,  touched[i]);
            //one comes down
            //  emitParticle(i, midline, -touched[i]);
        }
    
        int x = i*rodSpacing+rodMargins;
        if(touched[i]>0)
            particleSystem.addVacuumForce(x, midline, vacuumRadius, touched[i]*vacuumPower);
    }
    
	// apply per-particle forces
	glBegin(GL_LINES);
    ofVec2f pos;
    
    int numAbove = 0;
    
	for(int i = 0; i < particleSystem.size(); i++) {
		Particle& cur = particleSystem[i];
        
		// particle force on other particles
        //float f = 1-cur.y/height;
      //  f = (vForceFactor*f) / f;
		particleSystem.addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        
        //particleSystem.addColorAttractionForce(cur, particleNeighborhood, particleRepulsion);
        
        cur.loopAround(0,0,width,height, pBounce);
		cur.addDampingForce(pDampening); //slows the particle down
        
        //apply noise field force to the particle
        pos.set(cur.x,cur.y);
        ofVec2f fieldForce = getField(pos);
        if(cur.y > midline){
            fieldForce.y *= -1;
            //make it white again?
            ofColor white;
            int brightness = 200;
            white.r = brightness;
            white.g = brightness;
            white.b = brightness;
            cur.setColor(white);
        }
        else {
            if(cur.prevY > midline){
                //change particle color if previous y position was below midline only
                cur.setColor(currentColor.getCurrentColor(cur.x/width));
                //cur.setColor(currentColor.getCurrentColorFromImage(cur.x/width));
            }
            numAbove++;
        }
        
        //cout<<"num above: "<<numAbove<<endl;
        
        cur.applyForce(fieldForce);

        //move particles back down when top overflows
        if(lastNumAbove > particleSystem.size()/2){
            if(cur.y <= 1){
                //remove a particle
                /* particleSystem.erase(0);
                 
                 //This method creates a new particle
                 Particle particle(0,0); //CREATE NEW PARTICLE
                 
                 ofColor white;
                 particle.setColor(white);
                 particle.x = ofRandom(width);
                 particle.y = height;
                 particleSystem.add(particle);*/
                
                cur.y = height;
            }
        }
        
        //remove particles?
       /* if(particleSystem.size()>kParticles*1024)
          if(cur.y==0)
              cur.remove = true;
        */
        
        if(mixColor){
        //particle color mix!
        vector<Particle*> neighbors = particleSystem.getNeighbors(cur, particleNeighborhood/2);
        
        for(int n = 0; n < neighbors.size(); n++) {
            //average colors
            ofColor c;
            c.r = ((neighbors[n]->red + cur.red)/2)*255;
            c.g = ((neighbors[n]->green + cur.green)/2)*255;
            c.b = ((neighbors[n]->blue + cur.blue)/2)*255;
            
            neighbors[n]->setColor(c);
        }
        }
    
        //DELETE PARTICLES
        if(cur.remove)
            particleSystem.erase(i);
        
	}
    
    lastNumAbove = numAbove;
    
	glEnd();
	
	particleSystem.update();
    
    ofSetColor(pointOpacity, pointOpacity, pointOpacity, 255);
    particleSystem.draw();
    
    //draw the pixel patches...
    xmlSettings.pushTag("PixelPatcher");
    for(int i=0; i < xmlSettings.getNumTags("Patch"); i++){
        int rodNum = xmlSettings.getAttribute("Patch", "rod", 0, i);
        int pixelNum = xmlSettings.getAttribute("Patch", "pixel", 0, i);
        int ppX = getRodX(rodNum);
        int ppY = pixelNum;
        //draw black box there
        ofSetColor(0);
        ofDrawRectangle( ppX, ppY, 1, 1);
    }
    xmlSettings.popTag();

    
    //draw the mask cover line
    if(showPixelBar){
        ofSetColor(0);
        ofDrawRectangle( maskX, maskHeight, width, 1);
    }
    
    display->end();

    
   // if(isOn){
        //apply the blur
   // blur.setTexture(display->getTextureReference());
   // blur.setRadius(blurAmount);
   // blur.update();

    //Apply blur FX
    display->begin();
        
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

void visualSystem::emitParticle(int rod, int y, float yVel){
    //remove a particle
    //particleSystem.erase(0);
    particleEmitterCounter ++;
    if(particleEmitterCounter>particleSystem.size())
        particleEmitterCounter = 0;
    Particle& particle = particleSystem[0];
    
    //This method creates a new particle
   // Particle particle(0,0); //CREATE NEW PARTICLE
    
    particle.x = rod*rodSpacing+rodMargins;
    particle.setColor( currentColor.getCurrentColor(particle.x/width ));
    particle.y = y;
    particle.yv = pStartVel*yVel; //initial velocity
    particle.xv = 0;
   // particleSystem.add(particle);
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

int visualSystem::getRodX(int rod){
    return rod*rodSpacing+rodMargins;
}

void visualSystem::movePixelBar(int dir){
    maskHeight += dir;
    //save new pos to XML
    xmlSettings.pushTag("PixelPatcher");
   // string s = to_string(maskHeight);
    xmlSettings.setAttribute("PatchBar", "y", maskHeight, 0);
    xmlSettings.popTag();
    xmlSettings.save("settings.xml");
}
