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
}

void displaySystem::init(){
    //at first you must specify the Ip address of this machine
    artnet.setup("192.168.0.1"); //make sure the firewall is deactivated at this point
}

//update the display with a new image
void displaySystem::updateDisplay(ofFbo * frame){
    _frame = frame;
    
    _frame->readToPixels(testImage.getPixels());
    
    //draw to LEDs
    artnet.sendDmx("192.168.0.50", 0, 0, testImage.getPixels(), 512);//strip 1 is universes 0 and 1
    artnet.sendDmx("192.168.0.50", 0, 2, testImage.getPixels(), 512);//strip 2 is universes 2 and 3

}

//draw to screen
void displaySystem::draw(){
    ofSetColor(255);
    //render test mode
    ofSetColor(255);
    _frame->draw(0,0);
}

void displaySystem::clear(){
    //send clear signal to LED strips
    
}