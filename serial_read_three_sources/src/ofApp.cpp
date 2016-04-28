#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    
    ofBackground( 0 );
    ofSetLogLevel(OF_LOG_NOTICE);
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    
    serial.setup("/dev/tty.usbmodem1411", 9600); // serial port of board 1
    //serial[ 1 ].setup("/dev/tty.usbmodem1242", 9600); // serial port of board 2
    //serial[ 2 ].setup("/dev/tty.usbmodem1243", 9600); // serial port of board 3
    //serial[ 3 ].setup("/dev/tty.usbmodem1244", 9600); // serial port of board 4
    
    nPins[ 0 ] = 12;
    nPins[ 1 ] = 12;
    //nPins[ 2 ] = 10;
    //nPins[ 3 ] = 9;
    
    boxX = ofGetWindowWidth() / NPINS;
    boxY = ofGetWindowHeight() / 3;

    serial.flush( true, true );

    for( int i = 0; i < NBOARDS * NPINS; i ++ ) {
        touched[ i ] = false;
        pos_touched[ i ] = false;
        lastTouched[ i ] = false;
        color[ i ].set( 255, 0, 0 );
        touch_time[ i ] = ofGetElapsedTimeMillis();

        
    }
    
    current_time = ofGetElapsedTimeMillis();
    false_touch_timeout = 200;
    
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    serialFunction();

    
    for ( int i = 0; i < NBOARDS * NPINS; i ++ ) {
        
        if ( pos_touched[ i ] ) {
            if ( current_time - touch_time[ i ] > false_touch_timeout ) {
                touched[ i ] = true;
            }
        }
        
        if ( touched[ i ] ) {
            if ( !pos_touched[ i ] ) {
                touched[ i ] = false;
            }
        }

        
        if ( lastTouched[ i ] != touched[ i ] ) {
            
            //touch_time[ i ] = ofGetElapsedTimeMillis();

            if ( touched[ i ] ) {
                color[ i ].set( 0, 255, 0 );
                
            }
            else {
                color[ i ].set( 255, 0, 0 );
            }
        }
        
    }
    
    for ( int i = 0; i < NBOARDS * NPINS; i ++ ) {
        lastTouched[ i ] = touched[ i ];
    }
    
    
    current_time = ofGetElapsedTimeMillis();
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground( 0 );
    
    for ( int i = 0; i < NBOARDS * NPINS; i ++ ) {
        int j;
        if ( i >= 0 && i < 12 ) {
            j = 0;
        }
        if ( i >= 12 && i < 24 ) {
            j = 1;
        }
        if ( i >= 24 && i < 36 ) {
            j = 2;
        }
        ofSetColor( color[ i ] );
        ofDrawRectangle(( i % 12 ) * boxX + 10, boxY * j, 75, 200 );
    }

}

//--------------------------------------------------------------
void ofApp::serialFunction(){
    
    while ( serial.available() > 0 ){
        int myByte = 0;
        
        myByte = serial.readByte();
        cout << myByte << endl;
        for ( int i = 0; i < NBOARDS * NPINS; i ++ ) {
            if ( myByte == i * 2 ){
                pos_touched[ i ] = true;
                touch_time[ i ] = ofGetElapsedTimeMillis();
                //touched[ i ] = true;
                //cout << i << "on" <<endl;
                
            }
            if ( myByte == i * 2 + 1 ){
                pos_touched[ i ] = false;
                //touched[ i ] = false;
                //cout << i << "off" <<endl;
            }
        }
       
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
