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
    artnet.verbose = true;
    width = w;
    height = h;
    
    _numRods = numRods;
    _rodSpacing = rodSpacing;
    _rodMargins = rodMargins;
    numRodsOuter = numRods;
    
    //init and allocate all the FBOs
    for(int i = 0; i<=_numRods; i++)
    {
        ofFbo stripFBO = * new ofFbo();
        stripFBO.allocate(1, height, GL_RGB);
        stripFBOs.push_back(stripFBO);
    }
    
    strip.allocate(1, height, GL_RGB);
    
    frameCount = 0;
    
    for(int i=0; i<_numRods; i++){
        ofFbo * newFbo = new ofFbo;
        newFbo->allocate(1, height, GL_RGB);
        stripFBOs.push_back(*newFbo);
        
        stripImages.push_back(*new ofImage);
    }
    
}

//update the display with a new image
void displaySystem::updateDisplay(ofFbo * frame){
    _frame = frame;
    
   // _frame->readToPixels(testImage.getPixels());
    
   // strip.allocate(1,height,GL_RGB);
    ofSetColor(255);
    
    
    for(int i=0; i< _numRods; i++){
    //ADDRESS 1 strip per frame?
    //int i = frameCount;
        //draw to LEDs
        
        ////IP address
        string ipStart = "192.168.0.";
        int ipEnd = (51+((int)floor(i/16)%3));
        
      //  if(ipEnd == 50){
        
        char *ip = new char[12];
        sprintf(ip, "192.168.0.%d", ipEnd);
        
        ////subnet
        int subnet = (int)floor(i/8)%2;
        
        ////universe
        int universe = (i%8)*2;
        cout<<"send dmx strip #"<<i<< " to: "<<ip<<", "<<subnet<<", "<<universe<<endl;
        
        //int ofxArtnet::sendDmx( string targetIp, int targetSubnet, int targetUniverse, const unsigned char* data512, int size )
        //first half
        //crop the LED strip
        stripFBOs.at(i).begin();
        //float x, float y, float w, float h, float sx, float sy
        _frame->getTexture().drawSubsection(0,0,1,height/2,i*_rodSpacing + _rodMargins, 0);
        stripFBOs.at(i).end();
        
        stripFBOs.at(i).readToPixels(stripImages.at(i));
        
        //if(i==0)
          //  stripImage.setColor(0);

        artnet.sendDmx(ip, subnet, universe, stripImages.at(i).getPixels(), 512);
        
        
        //second half
        //crop the LED strip
       /* strip.begin();
        //float x, float y, float w, float h, float sx, float sy
        _frame->getTexture().drawSubsection(0,0,1,height/2,i*_rodSpacing + _rodMargins, height/2);
        strip.end();
        
        strip.readToPixels(stripImage.getPixels());
        
        //if(i!=0)
          //  stripImage.setColor(0);

        
        artnet.sendDmx(ip, subnet, universe+1, stripImage.getPixels(), 512);
        //strip 1 is universes 0 and 1*/
        
        //cleanup!
        delete[] ip;
       // }
    }
    frameCount++;
    if(frameCount>=_numRods)
        frameCount = 0;
}

//draw to screen
void displaySystem::draw(int x, int y){
    //render test mode
    ofSetColor(255);
    _frame->draw(x,y);
    
    for(int i=0; i< numRodsOuter; i++){
       // strip.begin();
       // _frame->getTexture().drawSubsection(0,0,1,height,i*_rodSpacing+_rodMargins,0);
       // strip.end();
        stripImages.at(i).draw(x+_rodSpacing*i+_rodMargins, y+10+height);
    }
    
}

void displaySystem::clear(){
    //send clear signal to LED strips
    
}