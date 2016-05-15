#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //at first you must specify the Ip address of this machine
    artnet.setup("192.168.0.1"); //make sure the firewall is deactivated at this point

    ofSetFrameRate( 40 );
    fbo.allocate(512, 1, GL_RGB);

    rainbow = false;
    frame = 0;

}

void ofApp::update(){
    frame++;
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));

    string ipStart = "192.168.0.";
    sleep(1);

    for (int i = 0; i < 36; i++) {
        ofClear(0);
        float phi = (float)(frame + i) * (PI / 36.0f);
        float colorR = (sin(phi) / 2.f + 0.5f) * 255.f;
        float colorG = (sin(phi + PI / 3.f) / 2.f + 0.5f) * 255.f;
        float colorB = (sin(phi + PI * 2.f / 3.f) / 2.f + 0.5f) * 255.f;
        ofColor currentColor = ofColor( colorR, colorG, colorB );

        fbo.begin();

            ofSetColor( currentColor );
            ofDrawRectangle(0, 0, 512, 1);

        fbo.end();
        fbo.readToPixels(testImage.getPixels());

        int ipEnd = 51 + ((int)floor(i/16) % 3);
        char *ip = new char[12];
        sprintf(ip, "192.168.0.%d", ipEnd);
        int subnet = (int)floor(i/8) % 2;
        int universe = (i % 8) * 2;

        if (i < 3) {
            cout << "send dmx strip #" << i << " to: " << ip << ", " << subnet << ", universes:" << universe << " and " << (universe + 1) << endl;
            cout << "color: " << currentColor << endl;
        }

        artnet.sendDmx(ip, subnet, universe, testImage.getPixels(), 512);
        artnet.sendDmx(ip, subnet, universe + 1, testImage.getPixels(), 512);
    }
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
