#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //load XML settings
    settings.loadFile("settings.xml");
    //parse the XML
    
    ofSetFrameRate( 40 );
    //maskHeight = 0;
    
    //calculate rodspacing
    visualSystemWidth = 256;
    visualSystemHeight = 240;
    
    rodMargins = 10;
    numRods = 16;
    rodSpacing = (visualSystemWidth-rodMargins)/numRods;
    
    cout<<"rod spacing"<<rodSpacing<<endl;
    
    vs.init(visualSystemWidth, visualSystemHeight, 5); //K particles
    vs.numRods = numRods;
    vs.rodSpacing = rodSpacing;
    vs.rodMargins = rodMargins;
    
    ds.init(visualSystemWidth, visualSystemHeight, numRods, rodSpacing, rodMargins);
    gui.setup(visualSystemWidth+20);
    serialReceiver.setup(0, numRods, rodSpacing);
    
   // ds.loadTestImage("test.png");
    
    //audio input
    ofSoundStreamSetup(0, 2, 44100, 256, 4);

}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
    
    serialReceiver.update();
    
    //UPDATE GUI
    gui.update();
    vs.timeSpeed = gui.flowSpeed;
    vs.timeStep = gui.timeSpeed;
    vs.hForce = gui.horizontalForce;
    vs.vForce = gui.verticalForce;
    vs.fadeAmt = gui.fadeAmt;
    vs.complexity = gui.flowComplexity;
    vs.particleNeighborhood = gui.particleNeighborhood;
    vs.particleRepulsion = gui.particleRepulsion;
    vs.update(serialReceiver.diffs);
    
    vs.blurAmount = gui.blur;
    vs.pStartVel = gui.pStartVel;
    vs.pBounce = gui.pBounce;
    vs.pDampening = gui.pDampening;
    vs.mixColor = gui.colorMixing;
    vs.vacuumRadius = gui.vacuumRadius;
    vs.vacuumPower = gui.vacuumForce;
    vs.colorStickiness = gui.colorStickiness;
    //vs.vForceFactor = gui.vForceFactor;
    //vs.particleBrightnessShift = gui.particleBrightnessShift;
    
    frame = vs.getFrame();
    ds.updateDisplay(frame);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(20);
    
    ofSetColor(255,255,255,255);
    ds.draw(10,10);
    
    gui.draw();
    
    //serialReceiver.draw(10, visualSystemHeight);
    
    //ofDrawBitmapString("Naughty Pixel Counter - ", 10, ofGetHeight()-10);
    ofSetColor(255);
    ofDrawBitmapString(vs.maskHeight, 0, vs.maskHeight+15);
    
    int colWidth = 60;
    int margin = 15;
    int yOffset = 15;
    
   /* ofDrawBitmapString("CAP TOUCH READINGS", visualSystemWidth+margin, yOffset);
    yOffset+=15;
    ofDrawBitmapString("rod#", visualSystemWidth+margin, yOffset);
    ofDrawBitmapString("diff", visualSystemWidth+margin+colWidth*1, yOffset);
    ofDrawBitmapString("smooth", visualSystemWidth+margin+colWidth*2, yOffset);
    ofDrawBitmapString("val", visualSystemWidth+margin+colWidth*3, yOffset);
    ofDrawBitmapString("base", visualSystemWidth+margin+colWidth*4, yOffset);
    
    for ( int i = 0; i < numRods; i ++ ) {
        int yPos = (i*margin)+margin+yOffset;
        ofSetColor(255);
        
        //rod #
        ofDrawBitmapString(i+1, visualSystemWidth+margin, yPos);
        
        //difference
        float d = serialReceiver.diffs[i];
        int baseC = 200;
        if(d>0)
            ofSetColor(50,255,50);
        else
            ofSetColor(baseC);
        ofDrawBitmapString(serialReceiver.diffs[i], visualSystemWidth+margin+colWidth*1, yPos);
        
        ofSetColor(255);
        //smoothy
        ofDrawBitmapString(serialReceiver.smoothedReadings[i], visualSystemWidth+margin+colWidth*2, yPos);
        
        //value
        ofDrawBitmapString(serialReceiver.readings[i], visualSystemWidth+margin+colWidth*3, yPos);
        
        //baseline
        ofDrawBitmapString(serialReceiver.averages[i], visualSystemWidth+margin+colWidth*4, yPos);
    
    }*/

}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    //pass the audio input to the visual system for processing...
    vs.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    serialReceiver.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    serialReceiver.keyReleased(key);
    
    if(key == OF_KEY_DOWN)
        vs.movePixelBar(1);
    else if(key == OF_KEY_UP)
        vs.movePixelBar(-1);
    else if(key == OF_KEY_RIGHT)
        vs.maskX++;
    else if(key == OF_KEY_UP)
        vs.maskX--;
    else if(key == ' ')
        vs.showPixelBar = !vs.showPixelBar;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
//--------------------------------------------------------------
void ofApp::exit() {
    ds.clear();
    serialReceiver.exit();
}
