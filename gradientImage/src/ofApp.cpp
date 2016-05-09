#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    nBars = 36;
    currentColor.setup( 2, nBars );

}

//--------------------------------------------------------------
void ofApp::update(){
    currentColor.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor( 255, 255, 255 );
    currentColor.draw( 10, 20 );
    
    for ( int i = 0; i < nBars; i ++ ) {
        ofSetColor( currentColor.getCurrentColors()[ i ]);
        ofDrawRectangle( 10 + 25 * i, 400, 20, 20 );
    }
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
