//
//  currentColor.cpp
//  
//
//  Created by curry on 3/10/16.
//
//

#include "currentColor.h"

//--------------------------------------------------------------
void currentColor::setup( float minutes, int n ) {
    
    imgWidth = 800;
    imgHeight = 50;
    fbo.allocate( imgWidth, imgHeight, GL_RGB );
    colorPaletteImage.allocate( imgWidth, imgHeight, OF_IMAGE_COLOR );
    nBars = n;
    
    //color timers
    startTime = ofGetSystemTime();
    currentTime = startTime;
    lastTime = 0;
    colorTime = 0;
    cycleDuration = 1000 * 60 * minutes;
    colorDuration = cycleDuration / imgWidth;
    colorPosition = 0;
    transSpeed = 0;
    cycles = 1;
    
    colorPalette.resize( 10 );
    gradientZone.resize( 9 );
    zoneSize.resize( 10 );
    
    //AWESOME/BORING
    colorPalette[ 0 ] = ofColor( 0, 171, 24 );//45, 255, 0 );
    colorPalette[ 1 ] = ofColor( 74, 252, 141 );//0, 255, 255 );
    colorPalette[ 2 ] = ofColor( 0, 246, 255 );//16, 0, 255 );
    colorPalette[ 3 ] = ofColor( 72, 186, 228 );// 147, 0, 188 );
    colorPalette[ 4 ] = ofColor( 0, 6, 253 );//250, 3, 255 );
    colorPalette[ 5 ] = ofColor( 113, 0, 218 );//255, 0, 0 );
    colorPalette[ 6 ] = ofColor( 169, 55, 201 );//255, 255, 255 );
    colorPalette[ 7 ] = ofColor( 255, 43, 243 );//255, 255, 255 );
    //colorPalette[ 7 ] = ofColor( 238, 0, 0 );//206, 255, 13 );
    colorPalette[ 8 ] = ofColor(237, 237, 237 );
    colorPalette[ 9 ] = ofColor( 0, 171, 24 );//237, 237, 237 );
    
    
    zoneSize[ 0 ] = 0;
    zoneSize[ 1 ] = .21;
    zoneSize[ 2 ] = zoneSize[ 1 ] + .02;
    zoneSize[ 3 ] = zoneSize[ 2 ] + .04;
    zoneSize[ 4 ] = zoneSize[ 3 ] + .45;
    zoneSize[ 5 ] = zoneSize[ 4 ] + .09;
    zoneSize[ 6 ] = zoneSize[ 5 ] + .02;
    zoneSize[ 7 ] = zoneSize[ 6 ] + .12;
    zoneSize[ 8 ] = zoneSize[ 7 ] + .02;
    zoneSize[ 9 ] = zoneSize[ 8 ] + .03;
    //zoneSize[ 9 ] = 1.0 - zoneSize[ 8 ];
    

    
    
    {
        fbo.begin();
        ofClear(0);
        for ( int i = 0; i < gradientZone.size(); i ++ ) {
            
            // first triangle
            gradientZone[ i ].addVertex(ofVec2f( imgWidth * zoneSize[ i ], 0 ));
            gradientZone[ i ].addColor( colorPalette[ i ] );
            gradientZone[ i ].addVertex(ofVec2f( imgWidth * zoneSize[ i + 1 ], 0 ));
            gradientZone[ i ].addColor( colorPalette[ i + 1 ] );
            gradientZone[ i ].addVertex(ofVec2f( imgWidth * zoneSize[ i + 1 ], 200 ));
            gradientZone[ i ].addColor( colorPalette[ i + 1] );
            
            // second triangle
            gradientZone[ i ].addVertex( ofVec2f( imgWidth * zoneSize[ i + 1 ], 200 ));
            gradientZone[ i ].addColor( colorPalette[ i + 1 ] );
            gradientZone[ i ].addVertex( ofVec2f( imgWidth * zoneSize[ i ], 200 ));
            gradientZone[ i ].addColor( colorPalette[ i ] );
            gradientZone[ i ].addVertex( ofVec2f( imgWidth * zoneSize[ i ], 0 ));
            gradientZone[ i ].addColor( colorPalette[ i ] );
            
            gradientZone[ i ].draw();
        }
        
        
        fbo.end();
        fbo.readToPixels( colorPalettePixels );
        colorPaletteImage.setFromPixels( colorPalettePixels );
    }
    
    
    currentColors.resize( nBars );
    fromColors.resize( nBars );
    toColors.resize( nBars );
    lastColors.resize( nBars );
    nextColors.resize( nBars );
    
    for ( int i = 0; i < currentColors.size(); i ++ ) {
        lastColors[ i ] = ( int ) imgWidth / nBars * i;
        nextColors[ i ] = lastColors[ i ] + 1;
        currentColors[ i ] = colorPalettePixels.getColor( lastColors[ i ], 0 );
        fromColors[ i ] = colorPalettePixels.getColor( lastColors[ i ], 0 );
        toColors[ i ] = colorPalettePixels.getColor( nextColors[ i ], 0 );
        
    }
    
}

//--------------------------------------------------------------
void currentColor::update() {
    
    transSpeed = (float) colorTime / colorDuration;
    currentTime = ofGetSystemTime() - startTime;//how long the sketch has been running in m
    
    for ( int i = 0; i < nBars; i ++ ) {
        currentColors[ i ] = fromColors[ i ].getLerped( toColors[ i ], transSpeed );
    }
    
    if ( colorTime >= colorDuration ) {
        cycles ++;
        lastColors = nextColors;
        
        for ( int i = 0; i < nBars; i ++ ) {
            nextColors[ i ] ++;
            
            if ( nextColors[ i ] > imgWidth - 1 ) {
                nextColors[ i ] = 0;
            }
            
            fromColors[ i ] = colorPalettePixels.getColor( lastColors[ i ], 0 );
            toColors[ i ] = colorPalettePixels.getColor( nextColors[ i ], 0 );
            
        }
        colorPosition ++;
        if ( colorPosition > imgWidth ) {
            colorPosition = 0;
        }
    }
    
    if ( cycles > 1 ) {
        colorTime = currentTime - ((cycles - 1 ) * colorDuration );
    }
    
    else {
        colorTime = currentTime;
    }
    
}
//--------------------------------------------------------------
void currentColor::draw( int x, int y ){
    
    ofSetColor( 255, 255, 255 );
    colorPaletteImage.draw( x, y + 10);
    
    ofSetColor( currentColors[ 0 ] );
    ofDrawRectangle( x / 2 + colorPosition, y, 10, 10 );
    
}

//--------------------------------------------------------------
vector <ofColor> currentColor::getCurrentColors() {
    return currentColors;
}

//--------------------------------------------------------------
ofColor currentColor::getCurrentColor(float i) {
    int colorCount = round(i*100);
    return currentColors[colorCount];
}

//
void currentColor::loadGradientImage(string path){
    gradientImage.allocate(300, 300, OF_IMAGE_COLOR);
    gradientImage.load("gradient.png");
    gradientImagePixels = gradientImage.getPixels();
}

ofColor currentColor::getCurrentColorFromImage(float offset){
    int slowness = 100;//this number affects the rate of change, bigger number makes the colors change more slowly
    int p = (offset * gradientImage.getWidth() + currentTime/slowness);
    p = p % (int)gradientImage.getWidth();
    return gradientImage.getColor(p,0);
}
