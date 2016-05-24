#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //load XML settings
    settings.loadFile("settings.xml");
    //parse the XML
    
    ofSetFrameRate( 40 );
    maskHeight = 0;
    
    //calculate rodspacing
    visualSystemWidth = 300;
    visualSystemHeight = 240;
    
    rodMargins = 10;
    numRods = 36;
    rodSpacing = (visualSystemWidth-rodMargins)/numRods;
    
    cout<<"rod spacing"<<rodSpacing<<endl;
    
    vs.init(visualSystemWidth, visualSystemHeight, 10); //K particles
    vs.numRods = numRods;
    vs.rodSpacing = rodSpacing;
    vs.rodMargins = rodMargins;
    
    ds.init(visualSystemWidth, visualSystemHeight, numRods, rodSpacing, rodMargins);
    gui.setup(visualSystemWidth+200);
    serialReceiver.setup(0, numRods, rodSpacing);
    
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
    
    serialReceiver.draw(10, visualSystemHeight);
    
    //ofDrawBitmapString("Naughty Pixel Counter - ", 10, ofGetHeight()-10);
    ofSetColor(255);
    ofDrawBitmapString(vs.maskHeight, 0, vs.maskHeight+15);
    
    int colWidth = 40;
    int margin = 15;
    
    ofDrawBitmapString("rod#", visualSystemWidth+margin, 10);
    ofDrawBitmapString("base", visualSystemWidth+margin+colWidth, 10);
    ofDrawBitmapString("val", visualSystemWidth+margin+colWidth*2, 10);
    ofDrawBitmapString("diff", visualSystemWidth+margin+colWidth*3, 10);

    
    for ( int i = 0; i < numRods; i ++ ) {
        
        int yPos = (i*margin)+margin*2;
        ofSetColor(255);
        //rod #
        ofDrawBitmapString(i, visualSystemWidth+margin, yPos);
        //baseline
        ofDrawBitmapString(serialReceiver.averages[i], visualSystemWidth+margin+colWidth, yPos);
        //value
        ofDrawBitmapString(serialReceiver.readings[i], visualSystemWidth+margin+colWidth*2, yPos);
        //difference
        float d = serialReceiver.diffs[i];
        
        int baseC = 100;
        
        if(d>0)
            ofSetColor(baseC,d*10,baseC);
        else if (d==0)
            ofSetColor(baseC);
        else
            ofSetColor(d*10, baseC, baseC);
        
        ofDrawBitmapString(serialReceiver.diffs[i], visualSystemWidth+margin+colWidth*3, yPos);

    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    serialReceiver.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    serialReceiver.keyReleased(key);
    
    if(key == OF_KEY_DOWN)
        vs.maskHeight++;
    else if(key == OF_KEY_UP)
        vs.maskHeight--;
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
    serialReceiver.exit();
}