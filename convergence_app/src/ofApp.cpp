#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //at first you must specify the Ip address of this machine
    artnet.setup("192.168.0.1"); //make sure the firewall is deactivated at this point
    
    ofSetFrameRate( 40 );
    fbo.allocate(512, 1, GL_RGB);
    
    rainbow = false;
    
    visualSystemWidth = 500;
    vs.init(visualSystemWidth, 500, 10);
    gui.setup(visualSystemWidth+20);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
    
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
    vs.update();
    vs.blurAmount = gui.blur;
    vs.particleBrightnessShift = gui.particleBrightnessShift;
    
    
    vs.update();
    frame = vs.getFrame();
    ds.updateDisplay(frame);
    
    //create send buffer by ofFbo
    {
        fbo.begin();
        
        ofClear(0);
        float colorR = (sin(ofGetElapsedTimeMillis() / 1000.f) / 2.f + 0.5f) * 255.f;
        float colorG = (sin((ofGetElapsedTimeMillis() / 1000.f) + PI / 3.f) / 2.f + 0.5f) * 255.f;
        float colorB = (sin((ofGetElapsedTimeMillis() / 1000.f)  + PI * 2.f / 3.f) / 2.f + 0.5f) * 255.f;
        ofColor currentColor = ofColor( colorR, colorG, colorB );
        
        
        if ( !rainbow ){
            //solid rectangle
            for ( int i = 0; i < 512; i ++ ) {
                color_array[ i ] = currentColor;
            }
            ofSetColor( currentColor );
            ofDrawRectangle(0, 0, 512, 1);
        }
        
        else {
            //rainbow rectangle
            for ( int i = 512; i > 0; i -- ) {
                color_array[ i ] = color_array[ i - 1 ];
            }
            color_array[ 0 ] = currentColor;
            
            for ( int i = 0; i < 512; i ++ ) {
                ofSetColor( color_array[ i ] );
                ofDrawRectangle( i, 0, 1, 1 );
            }
        }
        
        fbo.end();
        fbo.readToPixels(testImage.getPixels());
        
        
    }
    
    //list nodes for sending
    //with subnet / universe
    //first half of alphapix 1, subnet 0, universes 0-15
    artnet.sendDmx("192.168.0.50", 0, 0, testImage.getPixels(), 512);//strip 1 is universes 0 and 1
    artnet.sendDmx("192.168.0.50", 0, 2, testImage.getPixels(), 512);//strip 2 is universes 2 and 3
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
    ofBackground(0);
    /*float scalex = ofGetWidth() / fbo.getWidth();
    float scaley = ofGetHeight() / fbo.getHeight();
    ofScale(scalex, scaley);
    fbo.draw(0, 0);*/
    

    ofSetColor(255,255,255,255);
    ds.draw();
    
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if ( key == 'a') {
        rainbow = !rainbow;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
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
