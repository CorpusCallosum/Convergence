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
    colorPalette[ 0 ] = ofColor( 237, 237, 237 ); //79, 211, 73 ); // 101, 252, 99 ); //wite
    colorPalette[ 1 ] = ofColor( 186, 255, 74 );   // 200, 23, 178 );   //
    colorPalette[ 2 ] = ofColor( 28, 255, 238 ); //14, 229, 184 ); //21, 70, 228 );   //ice blue green
    colorPalette[ 3 ] = ofColor( 40, 45, 250 ); //64, 52, 95 );    //blue
    colorPalette[ 4 ] = ofColor( 255, 74, 213 ); // 203, 103, 10 );  //magenta
    colorPalette[ 5 ] = ofColor( 52, 11, 64 );   // 200, 23, 178 );
    colorPalette[ 6 ] = ofColor( 101, 252, 99 ); //neon
    colorPalette[ 7 ] = ofColor( 203, 103, 10 );
    colorPalette[ 8 ] = ofColor( 14, 229, 184  );
    colorPalette[ 9 ] = ofColor( 237, 237, 237 ); //79, 211, 73 ); // 101, 252, 99 ); //white
    
    zoneSize[ 0 ] = 0;
    zoneSize[ 1 ] = .216;
    zoneSize[ 2 ] = .228;//.012;
    zoneSize[ 3 ] = .264;//.036;
    zoneSize[ 4 ] = .379;//.115;
    zoneSize[ 5 ] = .461;//.082;
    zoneSize[ 6 ] = .977;//.516;
    zoneSize[ 7 ] = .992;//.015;
    zoneSize[ 8 ] = .996;//.004;
    zoneSize[ 9 ] = 1.0;
    
    
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
