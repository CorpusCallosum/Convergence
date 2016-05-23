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
    width = 300;
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
    visualSystemParams.add(fadeAmt.set("fade amount",100,0,255));
    visualSystemParams.add(blur.set("blur",1,0,10));
    //visualSystemParams.add(vForceFactor.set("v force factor",0,-20,20));
    
    //noise
    noiseParams.setName("Perlin Noise Wind");
    noiseParams.add(flowSpeed.set("turbulence",.002,0,.1));
    noiseParams.add(flowComplexity.set("density",2,0,20));
    noiseParams.add(horizontalForce.set("H force",.2,-1,1));
    noiseParams.add(verticalForce.set("V force",.2,-1,1));
    
    //particle params
    particleParams.setName("Particles");
    particleParams.add(timeSpeed.set("particle speed",.2,-1,1));
    particleParams.add(pStartVel.set("p start Velocity",-10,-20,0));
    particleParams.add(pBounce.set("p bounce",.5,0,1));
    particleParams.add(pDampening.set("p damp",.1,0,1));
    particleParams.add(particleNeighborhood.set("p neighborhood",4,0,50));
    particleParams.add(particleRepulsion.set("p repulsion",1,-1,10));
    particleParams.add(colorMixing.set("mix colors", false));
    particleParams.add(colorStickiness.set("color stickiness", 0, -2, 2));
    
    //vacuum
    vacuumParams.setName("Vacuum");
    vacuumParams.add(vacuumRadius.set("radius",100,0,300));
    vacuumParams.add(vacuumForce.set("strength",2,0,10));
    
    //create the gui
    guiPanel.setup("GUI",xmlFile, x, 10);
    guiPanel.setWidthElements(width);
    
    //add panels
    //guiPanel.add(dsParams);
    guiPanel.add(visualSystemParams);
    guiPanel.add(noiseParams);
    guiPanel.add(particleParams);
    guiPanel.add(vacuumParams);
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
