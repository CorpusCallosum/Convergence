//
//  gui.cpp
//  convergence_app
//
//  Created by Jack Kalish on 5/5/16.
//
//

#include "gui.hpp"

gui::gui(){
    xmlFile = "gui_settings.xml";
}

void gui::setup(int x){
    
    //display system
    dsParams.setName("Display System");
        
    dsParams.add(ledStripsColor.set("Strip Color",ofColor(255),ofColor(0, 0),ofColor(255)));
    dsParams.add(ledStripHue.set("Strip Hue",0,0,255));
    dsParams.add(ledStripBrightness.set("Strip Brightness",0,0,255));
    dsParams.add(ledStripSaturation.set("Strip Saturation",0,0,255));
    
    //add event listeners here to update color when hue changes, and vice-versus
    
    //visualSystem
    visualSystemParams.setName("Visual System");
    visualSystemParams.add(flowSpeed.set("turbulence",.002,0,.1));
    visualSystemParams.add(timeSpeed.set("particle speed",.2,-1,1));
    visualSystemParams.add(flowComplexity.set("density",2,0,20));
    visualSystemParams.add(horizontalForce.set("H force",.2,-1,1));
    visualSystemParams.add(verticalForce.set("V force",.2,-1,1));
    visualSystemParams.add(fadeAmt.set("fade amount",100,0,255));
    
    visualSystemParams.add(pStartVel.set("p start Velocity",-10,-20,0));
    visualSystemParams.add(vForceFactor.set("v force factor",0,-20,20));
    visualSystemParams.add(pBounce.set("p bounce",.5,0,1));
    visualSystemParams.add(pDampening.set("p damp",.1,0,1));
    visualSystemParams.add(particleNeighborhood.set("p neighborhood",4,0,50));
    visualSystemParams.add(particleRepulsion.set("p repulsion",1,-1,10));
    visualSystemParams.add(blur.set("blur",1,0,10));
    visualSystemParams.add(colorMixing.set("mix colors", false));
    
    //create the gui
    guiPanel.setup("GUI",xmlFile, x, 10);
    guiPanel.setWidthElements(200);
    
    guiPanel.add(dsParams);
    guiPanel.add(visualSystemParams);
    guiPanel.loadFromFile(xmlFile);
}

void gui::update(){
    //update led panel color
    ofColor c = ledStripsColor;
    c.setHue(ledStripHue);
    c.setSaturation(ledStripSaturation);
    c.setBrightness(ledStripBrightness);
    ledStripsColor.set(c);
}

void gui::draw(){
    guiPanel.draw();
}
