//
//  currentColor.cpp
//  
//
//  Created by curry on 3/10/16.
//
//

#include "currentColor.h"

//--------------------------------------------------------------
void currentColor::setup() {
        
    //color timers
    startTime = ofGetSystemTime();
    currentTime = startTime;
    lastTime = 0;
    colorTime = 0;
    colorDuration = 1000 * 20; //how long each color lasts in ms
    
    colorPalette.resize( 6 );
    gradientZone.resize( 5 );

    
    //AWESOME
    colorPalette[ 0 ] = ofColor( 237, 237, 237 ); //79, 211, 73 ); // 101, 252, 99 ); //wite
    colorPalette[ 1 ] = ofColor( 186, 255, 74 );   // 200, 23, 178 );   //
    colorPalette[ 2 ] = ofColor( 28, 255, 238 ); //14, 229, 184 ); //21, 70, 228 );   //ice blue green
    colorPalette[ 3 ] = ofColor( 40, 45, 250 ); //64, 52, 95 );    //blue
    colorPalette[ 4 ] = ofColor( 255, 74, 213 ); // 203, 103, 10 );  //magenta
    colorPalette[ 5 ] = ofColor( 52, 11, 64 );   // 200, 23, 178 );   //
  
    
    
    lastColor = 0;
    nextColor = 1;
    cycles = 1;
    
    currentColor = colorPalette[ lastColor ];
    fromColor = colorPalette[ lastColor ];
    toColor = colorPalette[ nextColor ];
    
    
}

//--------------------------------------------------------------
void currentColor::update() {
    
    transSpeed = (float) colorTime / colorDuration;
    currentColor = fromColor.getLerped( toColor, transSpeed );
    
    currentTime = ofGetSystemTime() - startTime;//how long the sketch has been running in m
    
    if ( colorTime >= colorDuration ) {
        cycles ++;
        lastColor = nextColor;
        nextColor ++;
        if ( nextColor > colorPalette.size() - 1 ) {
            nextColor = 0;
        }
        fromColor = colorPalette[ lastColor ];
        toColor = colorPalette[ nextColor ];
    }
    if ( cycles > 1 ) {
        colorTime = currentTime - ((cycles - 1 ) * colorDuration );
    }
    else {
        colorTime = currentTime;
    }


}

//--------------------------------------------------------------
void currentColor::draw(){
    
    for ( int i = 0; i < gradientZone.size(); i ++ ) {
        // first triangle
        gradientZone[ i ].addVertex(ofVec3f( ofGetWidth() / gradientZone.size() * i, ofGetHeight()/2 - 200, 1 ));
        gradientZone[ i ].addColor( colorPalette[ i ] );
        gradientZone[ i ].addVertex(ofVec3f( ofGetWidth() / gradientZone.size() * (i + 1), ofGetHeight() / 2 - 200, 1 ));
        gradientZone[ i ].addColor( colorPalette[ i + 1 ] );
        gradientZone[ i ].addVertex(ofVec3f( ofGetWidth() / gradientZone.size() * ( i + 1 ), ofGetHeight() / 2 + 200, 1 ));
        gradientZone[ i ].addColor( colorPalette[ i + 1] );
        
        
        // second triangle
        gradientZone[ i ].addVertex( ofVec3f( ofGetWidth() / gradientZone.size() * (i + 1), ofGetHeight() / 2 + 200, 1 ));
        gradientZone[ i ].addColor( colorPalette[ i + 1 ] );
        gradientZone[ i ].addVertex( ofVec3f( ofGetWidth() / gradientZone.size() * i, ofGetHeight() / 2 + 200, 1 ));
        gradientZone[ i ].addColor( colorPalette[ i ] );
        gradientZone[ i ].addVertex( ofVec3f( ofGetWidth() / gradientZone.size() * i, ofGetHeight()/2 - 200, 1 ));
        gradientZone[ i ].addColor( colorPalette[ i ] );
        
        // first triangle
        gradientZone[ i ].addTexCoord(ofVec2f( ofGetWidth() / gradientZone.size() * i, ofGetHeight()/2 - 200));
        gradientZone[ i ].addColor( colorPalette[ i ] );
        gradientZone[ i ].addTexCoord(ofVec2f( ofGetWidth() / gradientZone.size() * (i + 1), ofGetHeight() / 2 - 200 ));
        gradientZone[ i ].addColor( colorPalette[ i + 1 ] );
        gradientZone[ i ].addTexCoord(ofVec2f( ofGetWidth() / gradientZone.size() * ( i + 1 ), ofGetHeight() / 2 + 200));
        gradientZone[ i ].addColor( colorPalette[ i + 1 ] );
        
        // second triangle
        gradientZone[ i ].addTexCoord(ofVec2f( ofGetWidth() / gradientZone.size() * (i + 1), ofGetHeight() / 2 + 200 ));
        gradientZone[ i ].addColor( colorPalette[ i + 1 ] );
        gradientZone[ i ].addTexCoord(ofVec2f( ofGetWidth() / gradientZone.size() * i, ofGetHeight() / 2 + 200 ));
        gradientZone[ i ].addColor( colorPalette[ i ] );
        gradientZone[ i ].addTexCoord(ofVec2f( ofGetWidth() / gradientZone.size() * i, ofGetHeight()/2 - 200 ));
        gradientZone[ i ].addColor( colorPalette[ i ] );
        gradientZone[ i ].draw();
    }
    
    
}
//--------------------------------------------------------------
ofColor currentColor::getCurrentColor() {
    
    return currentColor;
    
}

ofColor currentColor::getCurrentColor(float offset) {
    
    return currentColor;
    
}
