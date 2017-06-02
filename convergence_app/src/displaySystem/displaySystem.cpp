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
    artnet.verbose = false;
    width = w;
    height = h;
    clearControl = false;
    
    _numRods = numRods;
    _rodSpacing = rodSpacing;
    _rodMargins = rodMargins;
    numRodsOuter = numRods;
    
    strip.allocate(1, height, GL_RGB);
    
    frameCount = 0;
    
    
    for(int i=0;i<36;i++){
        stripImages.push_back(new ofImage);
        ofFbo *newFBO = new ofFbo();
        newFBO->allocate(1, height, GL_RGB);
        stripFBOs.push_back(newFBO);
        
        stripImages2.push_back(new ofImage);
        ofFbo *newFBO2 = new ofFbo();
        newFBO2->allocate(1, height, GL_RGB);
        stripFBOs2.push_back(newFBO2);
    }
    
}

//update the display with a new image
void displaySystem::updateDisplay(ofFbo * frame){
    
    if(clearControl){
        //clear the FBO
        _frame->begin();
        ofClear(0);
        ofSetColor(0);
        ofDrawRectangle(0, 0, width, height);
        _frame->end();
    }
    else{
        _frame = frame; 
    }
    //sleep(1);
    // _frame->readToPixels(testImage.getPixels());
    
    // strip.allocate(1,height,GL_RGB);
    ofSetColor(255);
    
    
    for(int i=0; i< _numRods; i++){
        //fix for A5 -> C9
        int addressCount = i;
        if(i == 4)
            addressCount = 40;
        
        //draw to LEDs
        ////IP address
        //string ipStart = "192.168.0.";
        int ipEnd = (51+(int)floor(addressCount/16)%3);
        char *ip = new char[12];
        sprintf(ip, "192.168.0.%d", ipEnd);
        
        ////subnet
        int subnet = (int)floor(addressCount/8)%2;
        
        ////universe
        int universe = (addressCount%8)*2;
        
        //cout<<"send dmx strip #"<<i<< " to: ip:"<<ip<<", subnet:"<<subnet<<", universe:"<<universe<<endl;
        
        //int ofxArtnet::sendDmx( string targetIp, int targetSubnet, int targetUniverse, const unsigned char* data512, int size )
        //first half
        //crop the LED strip
        //strip.clear();
        int numChannels = 510;
        int universeHeight = numChannels/3;
        
        stripFBOs.at(i)->begin();
        ofClear(0);
        //float x, float y, float w, float h, float sx, float sy
        _frame->getTexture().drawSubsection(0,1,1,height,i*_rodSpacing + _rodMargins, 0);
        stripFBOs.at(i)->end();
        
        stripFBOs.at(i)->readToPixels(stripImages.at(i)->getPixels());
        
        artnet.sendDmx("192.168.0.50", subnet, universe, stripImages.at(i)->getPixels().getData(), numChannels);
        
        //second half
      /*  stripFBOs2.at(i)->begin();
        ofClear(0);
        //float x, float y, float w, float h, float sx, float sy
        _frame->getTexture().drawSubsection(0,0,1,height/2,i*_rodSpacing + _rodMargins, universeHeight);
        stripFBOs2.at(i)->end();
        
        stripFBOs2.at(i)->readToPixels(stripImages2.at(i)->getPixels());
        
        artnet.sendDmx(ip, subnet, universe+1, stripImages2.at(i)->getPixels(), numChannels);

        */
       /* //crop the LED strip
        stripFB.begin();
        ofClear(0);
        //float x, float y, float w, float h, float sx, float sy
        strip.end();
        
        strip.readToPixels(stripImage.getPixels());*/
        
        //if(i!=0)
        //  stripImage.setColor(0);
        
        
        //strip 1 is universes 0 and 1
        
        //cleanup!
        delete[] ip;
        // }
    }
    /*frameCount++;
    if(frameCount>=_numRods)
        frameCount = 0;*/
}

//draw to screen
void displaySystem::draw(int x, int y){
    //render test mode
    ofSetColor(255);
    _frame->draw(x,y);
    
    for(int i=0; i< numRodsOuter; i++){
       /* strip.begin();
        _frame->getTexture().drawSubsection(0,0,1,height,i*_rodSpacing+_rodMargins,0);
        strip.end();
        strip.draw(x+_rodSpacing*i+_rodMargins, y+10+height);*/
         //stripImages.at(i)->draw(x+_rodSpacing*i+_rodMargins, y+10+height);
        stripFBOs.at(i)->draw(x+_rodSpacing*i+_rodMargins, y+10+height);
        stripFBOs2.at(i)->draw(x+_rodSpacing*i+_rodMargins, y+15+height*1.5);
    }
    
}

void displaySystem::clear(){
    //send clear signal to LED strips
    clearControl = true;
    _frame->begin();
    ofClear(0);
    _frame->end();
    updateDisplay(_frame);
}
