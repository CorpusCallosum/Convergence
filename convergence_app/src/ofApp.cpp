#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //load XML settings
    settings.loadFile("settings.xml");
    //parse the XML
    
    ofSetFrameRate( 40 );
    fbo.allocate(512, 1, GL_RGB);
    
    rainbow = false;
    
    //calculate rodspacing
    
    visualSystemWidth = 300;
    visualSystemHeight = 300;
    
    rodMargins = 10;
    numRods = 36;
    rodSpacing = (visualSystemWidth-rodMargins*2)/numRods;
    
    cout<<"rod spacing"<<rodSpacing<<endl;
    
    vs.init(visualSystemWidth, visualSystemHeight, 5); //K particles
    vs.numRods = numRods;
    vs.rodSpacing = rodSpacing;
    vs.rodMargins = rodMargins;
    
    ds.init(visualSystemWidth, visualSystemHeight, numRods, rodSpacing, rodMargins);
    gui.setup(visualSystemWidth+20);
    serialReceiver.setup(10, numRods, rodSpacing);
    
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
    //vs.particleBrightnessShift = gui.particleBrightnessShift;
    
    frame = vs.getFrame();
    ds.updateDisplay(frame);
    
    //list nodes for sending
    //with subnet / universe
    //first half of alphapix 1, subnet 0, universes 0-15
    //artnet.sendDmx("192.168.0.50", 0, 0, testImage.getPixels(), 512);//strip 1 is universes 0 and 1
    //artnet.sendDmx("192.168.0.50", 0, 2, testImage.getPixels(), 512);//strip 2 is universes 2 and 3
    //artnet.sendDmx("192.168.0.50", 0, 4, testImage.getPixels(), 512);//strip 3 is universes 4 and 5
    //artnet.sendDmx("192.168.0.50", 0, 6, testImage.getPixels(), 512);//strip 4 is universes 6 and 7
    //artnet.sendDmx("192.168.0.50", 0, 8, testImage.getPixels(), 512);//strip 5 is universes 8 and 9
    //artnet.sendDmx("192.168.0.50", 0, 10, testImage.getPixels(), 512);//strip 6 is universes 10 and 11
    //artnet.sendDmx("192.168.0.50", 0, 12, testImage.getPixels(), 512);//strip 7 is universes 12 and 13
    //artnet.sendDmx("192.168.0.50", 0, 14, testImage.getPixels(), 512);//strip 8 is universes 14 and 15
    
    //second half of alphapix 1, subnet 1, universed 0-15
    //artnet.sendDmx("192.168.0.50", 1, 0, testImage.getPixels(), 512);//strip 9 is universes 0 and 1
    //artnet.sendDmx("192.168.0.50", 1, 2, testImage.getPixels(), 512);//strip 10 is universes 2 and 3
    //artnet.sendDmx("192.168.0.50", 1, 4, testImage.getPixels(), 512);//strip 11 is universes 4 and 5
    //artnet.sendDmx("192.168.0.50", 1, 6, testImage.getPixels(), 512);//strip 12 is universes 6 and 7
    //artnet.sendDmx("192.168.0.50", 1, 8, testImage.getPixels(), 512);//strip 13 is universes 8 and 9
    //artnet.sendDmx("192.168.0.50", 1, 10, testImage.getPixels(), 512);//strip 14 is universes 10 and 11
    //artnet.sendDmx("192.168.0.50", 1, 12, testImage.getPixels(), 512);//strip 15 is universes 12 and 13
    //artnet.sendDmx("192.168.0.50", 1, 14, testImage.getPixels(), 512);//strip 16 is universes 14 and 15
    
    //first half of alphapix 2, subnet 0, universes 0-15
    //artnet.sendDmx("192.168.0.51", 0, 0, testImage.getPixels(), 512);//strip 17 is universes 0 and 1
    //artnet.sendDmx("192.168.0.51", 0, 2, testImage.getPixels(), 512);//strip 18 is universes 2 and 3
    //artnet.sendDmx("192.168.0.51", 0, 4, testImage.getPixels(), 512);//strip 19 is universes 4 and 5
    //artnet.sendDmx("192.168.0.51", 0, 6, testImage.getPixels(), 512);//strip 20 is universes 6 and 7
    //artnet.sendDmx("192.168.0.51", 0, 8, testImage.getPixels(), 512);//strip 21 is universes 8 and 9
    //artnet.sendDmx("192.168.0.51", 0, 10, testImage.getPixels(), 512);//strip 22 is universes 10 and 11
    //artnet.sendDmx("192.168.0.51", 0, 12, testImage.getPixels(), 512);//strip 23 is universes 12 and 13
    //artnet.sendDmx("192.168.0.51", 0, 14, testImage.getPixels(), 512);//strip 24 is universes 14 and 15
    
    //second half of alphapix 2, subnet 1, universed 0-15
    //artnet.sendDmx("192.168.0.51", 1, 0, testImage.getPixels(), 512);//strip 25 is universes 0 and 1
    //artnet.sendDmx("192.168.0.51", 1, 2, testImage.getPixels(), 512);//strip 26 is universes 2 and 3
    //artnet.sendDmx("192.168.0.51", 1, 4, testImage.getPixels(), 512);//strip 27 is universes 4 and 5
    //artnet.sendDmx("192.168.0.51", 1, 6, testImage.getPixels(), 512);//strip 28 is universes 6 and 7
    //artnet.sendDmx("192.168.0.51", 1, 8, testImage.getPixels(), 512);//strip 29 is universes 8 and 9
    //artnet.sendDmx("192.168.0.51", 1, 10, testImage.getPixels(), 512);//strip 30 is universes 10 and 11
    //artnet.sendDmx("192.168.0.51", 1, 12, testImage.getPixels(), 512);//strip 31 is universes 12 and 13
    //artnet.sendDmx("192.168.0.51", 1, 14, testImage.getPixels(), 512);//strip 32 is universes 14 and 15
    
    //first half of alphapix 3, subnet 0, universes 0-15
    //artnet.sendDmx("192.168.0.52", 0, 0, testImage.getPixels(), 512);//strip 33 is universes 0 and 1
    //artnet.sendDmx("192.168.0.52", 0, 2, testImage.getPixels(), 512);//strip 34 is universes 2 and 3
    //artnet.sendDmx("192.168.0.52", 0, 4, testImage.getPixels(), 512);//strip 35 is universes 4 and 5
    //artnet.sendDmx("192.168.0.52", 0, 6, testImage.getPixels(), 512);//strip 36 is universes 6 and 7
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(20);
    /*float scalex = ofGetWidth() / fbo.getWidth();
    float scaley = ofGetHeight() / fbo.getHeight();
    ofScale(scalex, scaley);
    fbo.draw(0, 0);*/
    
    ofSetColor(255,255,255,255);
    ds.draw(10,10);
    
    gui.draw();
    
    serialReceiver.draw(10, visualSystemHeight);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if ( key == 'a') {
        rainbow = !rainbow;
    }
    
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
