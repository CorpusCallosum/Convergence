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
   // midline =  1.828 * 60; //6' from the top
    midline = 0;
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

    //audio stuff
    ofSetVerticalSync(true);
    ofSetCircleResolution(80);
    ofBackground(0, 0, 0);
    
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // 256 samples per buffer
    // 4 num buffers (latency)
    
    soundStream.printDeviceList();
    
    //if you want to set a different device id
    //soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    
    int bufferSize = 256;
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    //soundStream.setup( 0, 2, 44100, bufferSize, 4);
//    ofSoundStreamSetup(0, 2, 44100, bufferSize, 4);
}

void visualSystem::reset(){
    
}

void visualSystem::update(float touched[36]){
    
    //sound stuff******************************
    //lets scale the vol up to a 0-1 range
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    //lets record the volume into an array
    volHistory.push_back( scaledVol );
    
    //if we are bigger the the size we want to record - lets drop the oldest value
    if( volHistory.size() >= 400 ){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }
    //*****************************************
    
    currentColor.update();
    
    particleSystem.setTimeStep(timeStep);
    t = ofGetFrameNum() * timeSpeed;
    
    //draw to FBO
    display->begin();

    ofFill();
    
    //fade out BG by drawing a rectangle
    int brightness = 0; //bg color
    ofSetColor(brightness, brightness, brightness, fadeAmt);
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
        //if(cur.y > midline){
            fieldForce.y *= -1;
            //make it white again?
            ofColor white;
            int brightness = 10 + round(cur.yv);
            white.r = brightness;
            white.g = brightness;
            white.b = brightness;
            cur.setColor(white);
        /*}
        else {
            if(cur.prevY > midline){
                //change particle color if previous y position was below midline only
                cur.setColor(currentColor.getCurrentColor(cur.x/width));
                //cur.setColor(currentColor.getCurrentColorFromImage(cur.x/width));
            }
            numAbove++;
        }*/
        
        //cout<<"num above: "<<numAbove<<endl;
        
        cur.applyForce(fieldForce);

        //move particles back down when top overflows
       /* if(lastNumAbove > particleSystem.size()/2){
            if(cur.y <= 1){
     
                cur.y = height;
            }
        }*/
        
        
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
    
    //AUDIO(**********************************
    //apply audio forces tp particles
    for (unsigned int i = 0; i < left.size(); i++){
        int x = i;
        int y = left[i]*180.0f;
        //ofVertex(x, y);
        //add forces
         particleSystem.addRepulsionForce(x, 0, 50, y);
    }
	
	particleSystem.update();
    
    ofSetColor(pointOpacity, pointOpacity, pointOpacity, 255);
    particleSystem.draw();
    
    //draw the pixel patches...
    xmlSettings.pushTag("PixelPatcher");
    for(int i=0; i < xmlSettings.getNumTags("Patch"); i++){
        int rodNum = xmlSettings.getAttribute("Patch", "rod", 0, i) - 1;
        int pixelNum = xmlSettings.getAttribute("Patch", "pixel", 0, i);
        int ppX = getRodX(rodNum);
        int ppY = pixelNum;
        int ppS = xmlSettings.getAttribute("Patch", "size", 0, i);
        //draw black box there
        ofSetColor(0);
        ofDrawRectangle( ppX-1, ppY, 3, ppS);
    }
    xmlSettings.popTag();

    
    //draw the mask cover line
    if(showPixelBar){
        ofSetColor(0);
        ofDrawRectangle( maskX, maskHeight, width, 1);
    }
    
    drawAudio();
    
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
    particleEmitterCounter ++;
    if(particleEmitterCounter>particleSystem.size())
        particleEmitterCounter = 0;
    Particle& particle = particleSystem[0];
    
    particle.x = rod*rodSpacing+rodMargins;
    particle.setColor( currentColor.getCurrentColor(particle.x/width ));
    particle.y = y;
    particle.yv = pStartVel*yVel; //initial velocity
    particle.xv = 0;
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

//SOUND STUFF
void visualSystem::drawAudio(){
    //DRAW sound stuff
    ofFill();
    
    // draw the left channel:
    ofPushStyle();
    ofPushMatrix();
    
    //define colors
   // ofColor colorA, colorB, colorX;
    //colorA.set(0, 0, 255);
    //colorB.set(0, 255, 0);
    
    float colorTween = averageFrequency/2;
    int brightness = 100;
    ofSetColor(0, colorTween+brightness, 255-colorTween+brightness); //line color
    ofSetLineWidth(3);
    
    ofBeginShape();
    
    //DRAW THE AUDIO WAVE
    ofVertex(0, 0);
    for (unsigned int i = 0; i < left.size(); i++){
        int x = i;
        int y = left[i]*180.0f;
        ofVertex(x, y);
    }
    ofVertex(left.size()*2, 0);
    
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    

}
//--------------------------------------------------------------
void visualSystem::audioIn(float * input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
}
