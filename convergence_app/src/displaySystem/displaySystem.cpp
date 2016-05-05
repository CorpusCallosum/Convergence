//
//  visualSystem.cpp
//  JRHMemorialWall
//
//  Created by Jack Kalish on 6/25/14.
//
//

#include "displaySystem.h"


displaySystem::displaySystem(){
    
    width = height = 0;
    numRodsOuter = 36;
    numRodsInner = 9;
    rodSpacing = 10;
}

void displaySystem::init(int w, int h){
    //at first you must specify the Ip address of this machine
    artnet.setup("192.168.0.1"); //make sure the firewall is deactivated at this point
    width = w;
    height = h;
    
    /*for(int i=0; i< numRodsOuter; i++){
        stripFBOs[i].allocate(1, height, GL_RGB);
    }*/
    
    strip.allocate(1, height, GL_RGB);
}

//update the display with a new image
void displaySystem::updateDisplay(ofFbo * frame){
    _frame = frame;
    
   // _frame->readToPixels(testImage.getPixels());
    
   // strip.allocate(1,height,GL_RGB);
    
    for(int i=0; i< numRodsOuter; i++){
    
        strip.begin();
        _frame->getTexture().drawSubsection(0,0,1,height,i*rodSpacing,0);
        strip.end();
        
        strip.readToPixels(stripImage.getPixels());
        
        //draw to LEDs
        artnet.sendDmx("192.168.0.50", 0, i*2, stripImage.getPixels(), 500);//strip 1 is universes 0 and 1
    }
        
}

//draw to screen
void displaySystem::draw(){
    ofSetColor(255);
    //render test mode
    ofSetColor(255);
    _frame->draw(10,0);
    
    for(int i=0; i< numRodsOuter; i++){
        strip.begin();
        _frame->getTexture().drawSubsection(0,0,1,height,i*rodSpacing,0);
        strip.end();
        strip.draw(10+rodSpacing*i, 10+height);
    }
    
}

void displaySystem::clear(){
    //send clear signal to LED strips
    
}