#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //at first you must specify the Ip address of this machine
    artnet.setup("192.168.0.1"); //make sure the firewall is deactivated at this point
    
    ofSetFrameRate( 40 );
    fbo.allocate(512, 1, GL_RGB);
    
    rainbow = false;
    
    /*for ( int i = 0; i < 512; i ++ ) {
        uni1[ i ] = 0;
        uni2[ i ] = 0;
    }*/

}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
    
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
    
    //uni1 = testImage.getPixels();
    
    //list nodes for sending
    //with subnet / universe
    artnet.sendDmx("192.168.0.50", 0xf, 0xf, testImage.getPixels(), 512);
    //artnet.sendDmx("10.0.0.149", testImage.getPixels(), 512);

    
    //artnet.sendDmx("2.0.0.10",0,0, uni1, 512);
    //artnet.sendDmx("2.0.0.10",0,1, uni2, 512);

    

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    float scalex = ofGetWidth() / fbo.getWidth();
    float scaley = ofGetHeight() / fbo.getHeight();
    ofScale(scalex, scaley);
    fbo.draw(0, 0);

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
