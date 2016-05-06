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
}

void displaySystem::init(int w, int h, int numRods, int rodSpacing, int rodMargins){
    //at first you must specify the Ip address of this machine
    artnet.setup("192.168.0.1"); //make sure the firewall is deactivated at this point
    width = w;
    height = h;
    
    _numRods = numRods;
    _rodSpacing = (width - rodMargins)/numRods;
    _rodMargins = rodMargins;
    numRodsOuter = numRods;
    
    strip.allocate(1, height, GL_RGB);
}

//update the display with a new image
void displaySystem::updateDisplay(ofFbo * frame){
    _frame = frame;
    
   // _frame->readToPixels(testImage.getPixels());
    
   // strip.allocate(1,height,GL_RGB);
    ofSetColor(255);
    
    for(int i=0; i< numRodsOuter; i++){
    
        //crop the LED strip
        strip.begin();
            //float x, float y, float w, float h, float sx, float sy
            _frame->getTexture().drawSubsection(0,0,1,height,i*_rodSpacing + _rodMargins, 0);
        strip.end();
        
        strip.readToPixels(stripImage.getPixels());
        
        //draw to LEDs
        
        ////IP address
        string ipStart = "192.168.0.";
        int ipEnd = (50+floor(i/16));
        char *ip = new char[12];
        sprintf(ip, "192.168.0.%d", ipEnd);
        
        ////subnet
        int subnet = (int)floor(i/8)%2;
        
        ////universe
        int universe = (i%8)*2;
        //cout<<"send dmx strip #"<<i<< " to: "<<ip<<", "<<subnet<<", "<<universe<<endl;
        artnet.sendDmx(ip, subnet, universe, stripImage.getPixels(), 500);
        //strip 1 is universes 0 and 1
        
        //cleanup!
        delete[] ip;
    }
        
}

//draw to screen
void displaySystem::draw(int x, int y){
    //render test mode
    ofSetColor(255);
    _frame->draw(x,y);
    
    for(int i=0; i< numRodsOuter; i++){
        strip.begin();
        _frame->getTexture().drawSubsection(0,0,1,height,i*_rodSpacing+_rodMargins,0);
        strip.end();
        strip.draw(x+_rodSpacing*i+_rodMargins, y+10+height);
    }
    
}

void displaySystem::clear(){
    //send clear signal to LED strips
    
}