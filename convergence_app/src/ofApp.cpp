#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //load XML settings
    settings.loadFile("settings.xml");
    //parse the XML
    
    ofSetFrameRate( 40 );
    
    //calculate rodspacing
    visualSystemWidth = 300;
    visualSystemHeight = 240;
    
    rodMargins = 10;
    numRods = 36;
    rodSpacing = (visualSystemWidth-rodMargins*2)/numRods;
    
    cout<<"rod spacing"<<rodSpacing<<endl;
    
    vs.init(visualSystemWidth, visualSystemHeight, 10); //K particles
    vs.numRods = numRods;
    vs.rodSpacing = rodSpacing;
    vs.rodMargins = rodMargins;
    
    ds.init(visualSystemWidth, visualSystemHeight, numRods, rodSpacing, rodMargins);
    gui.setup(visualSystemWidth+20);
    serialReceiver.setup(10, numRods, rodSpacing);
    
   // ds.loadTestImage("test.png");
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
    vs.update(serialReceiver.touched);
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
    
    serialReceiver.draw(10, visualSystemHeight);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    serialReceiver.keyPressed(key);

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    serialReceiver.keyReleased(key);
    
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
