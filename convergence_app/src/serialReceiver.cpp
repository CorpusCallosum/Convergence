//
//  serialReceiver.cpp
//  
//
//  Created by curry on 5/4/16.
//
//

#include "serialReceiver.h"

//--------------------------------------------------------------
void serialReceiver::setup( int t , int numRods, int rodSpacing) {
    
    _rodSpacing = rodSpacing;
    _numRods = numRods;
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
     
    serial.setup("/dev/cu.usbmodem1589091", 9600); // serial port of arduino
    serial.flush( true, true );

    boxX = ofGetWindowWidth() / NPINS;
    boxY = ofGetWindowHeight() / 3;
    
    numReadingsStored = 50;
    
    for( int i = 0; i < numRods; i ++ ) {
        touched[ i ] = false;
        pos_touched[ i ] = false;
        lastTouched[ i ] = false;
        color[ i ].set( 255, 0, 0 );
        touch_time[ i ] = ofGetElapsedTimeMillis();
        readings[i] = 0;
        baselines[i] = 0;
        
        //init all readings at 0
        readingsVectors.push_back(*new vector <int>);
        for(int c=0; c<numReadingsStored; c++)
        {
            //set
            readingsVectors.at(i).push_back(0);
           // readingsArrays[i][c]=0;
        }
    }
    
    current_time = ofGetElapsedTimeMillis();
    false_touch_timeout = t; //in milliseconds
    
    start = false;
    
    place = 0;
   
}

//--------------------------------------------------------------
void serialReceiver::update(){
    
    serialFunction();

    //clear out the inputs after 2 seconds in case 
    if ( current_time  >= 2000 && current_time < 2200 ) {
        for ( int i = 0; i < _numRods; i ++ ) {
            pos_touched[ i ] = false;
            touched[ i ] = false;
        }
    }
    
    for ( int i = 0; i < _numRods; i ++ ) {
        
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
            
            if ( touched[ i ] ) {
                color[ i ].set( 0, 255, 0 );
                
            }
            else {
                color[ i ].set( 255, 0, 0 );
            }
        }
        
    }
    
    for ( int i = 0; i < _numRods; i ++ ) {
        lastTouched[ i ] = touched[ i ];
    }
    
    
    current_time = ofGetElapsedTimeMillis();
    
    
}

//--------------------------------------------------------------
void serialReceiver::draw(int x, int y){
    //ofBackground( 0 );
    
    
    
    for ( int i = 0; i < _numRods; i ++ ) {
        ofSetColor( color[ i ] );
        float xPos = i * _rodSpacing + x;
        ofDrawRectangle( xPos, y, 5, 10 );
        ofDrawBitmapString(averages[i], xPos, y-5);
        ofDrawBitmapString(readings[i], xPos, y-20);
        
        ofDrawBitmapString(averages[i], xPos, y-5);
        ofDrawBitmapString(readings[i], xPos, y-20);
    }
    
}

//--------------------------------------------------------------
void serialReceiver::serialFunction() {
    while ( serial.available() > 0 ){
        int byte;
        
        byte = serial.readByte();
        //cout << "byte: "<<byte << endl;
        
        if(place == 1){
            //byte is index
            currentReadingIndex = byte;
            place = 2;
        }
        else if(place == 2){
            //byte is reading
            int reading = byte;
          //  cout<<"reading at "<< currentReadingIndex <<": "<<reading<<endl;
            readings[currentReadingIndex] = reading;
            place = 0;
            
            
            //remove first element
            readingsVectors.at(currentReadingIndex).erase(readingsVectors.at(currentReadingIndex).begin());
            //readingsArrays[currentReadingIndex].push(reading);
            readingsVectors.at(currentReadingIndex).push_back(reading);
            
            
            //calculate total average
            for(int vCnt = 0; vCnt< readingsVectors.size(); vCnt++){
                float sum = 0;
                for(int rCnt = 0; rCnt< numReadingsStored; rCnt++)
                {
                    sum += readingsVectors.at(vCnt).at(rCnt);
                }
                float avg = sum/numReadingsStored;
                avg = ceilf(avg * 100) / 100;
                averages[vCnt] = avg;
            }
            
            //OLDER METHOD OF SETTING BASELINES AT START 5 SECONDS...
            if(ofGetElapsedTimeMillis() < 5000){
                //averages[currentReadingIndex] = reading;
                for(int c=0; c<numReadingsStored; c++)
                {
                    //set
                    readingsVectors.at(currentReadingIndex).at(c) = reading;
                    // readingsArrays[i][c]=0;
                }
               // cout<<"baseline: "<<baselines[currentReadingIndex]<<endl;
            }
            /*else{
                float dif = baselines[currentReadingIndex] - reading;
                diffs[currentReadingIndex] = dif;
               // cout<<"dif: "<<dif<<endl;
            }*/
            
            //NEWER METHOD calculates baseline by averaging over long-term
            float dif = averages[currentReadingIndex] - reading;
            diffs[currentReadingIndex] = dif;
            
        }
        
        if(byte == 255){
            //got a delimiter
            place = 1;
        }
        
        
        /*for ( int i = 0; i < _numRods; i ++ ) {
            if ( myByte == i * 2 ){
                cout<<"rod "<< i << " ON"<< endl;
                pos_touched[ i ] = true;
                touch_time[ i ] = ofGetElapsedTimeMillis();
                //cout << i << "on" <<endl;
                
            }
            if ( myByte == i * 2 + 1 ){
                cout<<"rod "<< i << " OFF"<< endl;
                pos_touched[ i ] = false;
                //cout << i << "off" <<endl;
            }
        }*/
        
    }
}

//--------------------------------------------------------------
void serialReceiver::keyPressed(int key){
    switch( key ) {
        case '1':
            pos_touched[ 0 ] = true;
            touch_time[ 0 ] = ofGetElapsedTimeMillis();
            break;
        case '2':
            pos_touched[ 1 ] = true;
            touch_time[ 1 ] = ofGetElapsedTimeMillis();
            break;
        case '3':
            pos_touched[ 2 ] = true;
            touch_time[ 2 ] = ofGetElapsedTimeMillis();
            break;
        case '4':
            pos_touched[ 3 ] = true;
            touch_time[ 3 ] = ofGetElapsedTimeMillis();
            break;
        case '5':
            pos_touched[ 4 ] = true;
            touch_time[ 4 ] = ofGetElapsedTimeMillis();
            break;
        case '6':
            pos_touched[ 5 ] = true;
            touch_time[ 5 ] = ofGetElapsedTimeMillis();
            break;
        case '7':
            pos_touched[ 6 ] = true;
            touch_time[ 6 ] = ofGetElapsedTimeMillis();
            break;
        case '8':
            pos_touched[ 7 ] = true;
            touch_time[ 7 ] = ofGetElapsedTimeMillis();
            break;
        case '9':
            pos_touched[ 8 ] = true;
            touch_time[ 8 ] = ofGetElapsedTimeMillis();
            break;
        case '0':
            pos_touched[ 9 ] = true;
            touch_time[ 9 ] = ofGetElapsedTimeMillis();
            break;
            
        case 'q':
            pos_touched[ 10 ] = true;
            touch_time[ 10 ] = ofGetElapsedTimeMillis();
            break;
        case 'w':
            pos_touched[ 11 ] = true;
            touch_time[ 11 ] = ofGetElapsedTimeMillis();
            break;
        case 'e':
            pos_touched[ 12 ] = true;
            touch_time[ 12 ] = ofGetElapsedTimeMillis();
            break;
        case 'r':
            pos_touched[ 13 ] = true;
            touch_time[ 13 ] = ofGetElapsedTimeMillis();
            break;
        case 't':
            pos_touched[ 14 ] = true;
            touch_time[ 14 ] = ofGetElapsedTimeMillis();
            break;
        case 'y':
            pos_touched[ 15 ] = true;
            touch_time[ 15 ] = ofGetElapsedTimeMillis();
            break;
        case 'u':
            pos_touched[ 16 ] = true;
            touch_time[ 16 ] = ofGetElapsedTimeMillis();
            break;
        case 'i':
            pos_touched[ 17 ] = true;
            touch_time[ 17 ] = ofGetElapsedTimeMillis();
            break;
        case 'o':
            pos_touched[ 18 ] = true;
            touch_time[ 18 ] = ofGetElapsedTimeMillis();
            break;
        case 'p':
            pos_touched[ 19 ] = true;
            touch_time[ 19 ] = ofGetElapsedTimeMillis();
            break;
            
        case 'a':
            pos_touched[ 20 ] = true;
            touch_time[ 20 ] = ofGetElapsedTimeMillis();
            break;
        case 's':
            pos_touched[ 21 ] = true;
            touch_time[ 21 ] = ofGetElapsedTimeMillis();
            break;
        case 'd':
            pos_touched[ 22 ] = true;
            touch_time[ 22 ] = ofGetElapsedTimeMillis();
            break;
        case 'f':
            pos_touched[ 23 ] = true;
            touch_time[ 23 ] = ofGetElapsedTimeMillis();
            break;
        case 'g':
            pos_touched[ 24 ] = true;
            touch_time[ 24 ] = ofGetElapsedTimeMillis();
            break;
        case 'h':
            pos_touched[ 25 ] = true;
            touch_time[ 25 ] = ofGetElapsedTimeMillis();
            break;
        case 'j':
            pos_touched[ 26 ] = true;
            touch_time[ 26 ] = ofGetElapsedTimeMillis();
            break;
        case 'k':
            pos_touched[ 27 ] = true;
            touch_time[ 27 ] = ofGetElapsedTimeMillis();
            break;
        case 'l':
            pos_touched[ 28 ] = true;
            touch_time[ 28 ] = ofGetElapsedTimeMillis();
            break;
        case ';':
            pos_touched[ 29 ] = true;
            touch_time[ 29 ] = ofGetElapsedTimeMillis();
            break;
            
        case 'z':
            pos_touched[ 30 ] = true;
            touch_time[ 30 ] = ofGetElapsedTimeMillis();
            break;
        case 'x':
            pos_touched[ 31 ] = true;
            touch_time[ 31 ] = ofGetElapsedTimeMillis();
            break;
        case 'c':
            pos_touched[ 32 ] = true;
            touch_time[ 32 ] = ofGetElapsedTimeMillis();
            break;
        case 'v':
            pos_touched[ 33 ] = true;
            touch_time[ 33 ] = ofGetElapsedTimeMillis();
            break;
        case 'b':
            pos_touched[ 34 ] = true;
            touch_time[ 34 ] = ofGetElapsedTimeMillis();
            break;
        case 'n':
            pos_touched[ 35 ] = true;
            touch_time[ 35 ] = ofGetElapsedTimeMillis();
            break;
            
    }
    
}

//--------------------------------------------------------------
void serialReceiver::keyReleased(int key){
    switch( key ) {
        case '1':
            pos_touched[ 0 ] = false;
            break;
        case '2':
            pos_touched[ 1 ] = false;
            break;
        case '3':
            pos_touched[ 2 ] = false;
            break;
        case '4':
            pos_touched[ 3 ] = false;
            break;
        case '5':
            pos_touched[ 4 ] = false;
            break;
        case '6':
            pos_touched[ 5 ] = false;
            break;
        case '7':
            pos_touched[ 6 ] = false;
            break;
        case '8':
            pos_touched[ 7 ] = false;
            break;
        case '9':
            pos_touched[ 8 ] = false;
            break;
        case '0':
            pos_touched[ 9 ] = false;
            break;
            
        case 'q':
            pos_touched[ 10 ] = false;
            break;
        case 'w':
            pos_touched[ 11 ] = false;
            break;
        case 'e':
            pos_touched[ 12 ] = false;
            break;
        case 'r':
            pos_touched[ 13 ] = false;
            break;
        case 't':
            pos_touched[ 14 ] = false;
            break;
        case 'y':
            pos_touched[ 15 ] = false;
            break;
        case 'u':
            pos_touched[ 16 ] = false;
            break;
        case 'i':
            pos_touched[ 17 ] = false;
            break;
        case 'o':
            pos_touched[ 18 ] = false;
            break;
        case 'p':
            pos_touched[ 19 ] = false;
            break;
            
        case 'a':
            pos_touched[ 20 ] = false;
            break;
        case 's':
            pos_touched[ 21 ] = false;
            break;
        case 'd':
            pos_touched[ 22 ] = false;
            break;
        case 'f':
            pos_touched[ 23 ] = false;
            break;
        case 'g':
            pos_touched[ 24 ] = false;
            break;
        case 'h':
            pos_touched[ 25 ] = false;
            break;
        case 'j':
            pos_touched[ 26 ] = false;
            break;
        case 'k':
            pos_touched[ 27 ] = false;
            break;
        case 'l':
            pos_touched[ 28 ] = false;
            break;
        case ';':
            pos_touched[ 29 ] = false;
            break;
            
        case 'z':
            pos_touched[ 30 ] = false;
            break;
        case 'x':
            pos_touched[ 31 ] = false;
            break;
        case 'c':
            pos_touched[ 32 ] = false;
            break;
        case 'v':
            pos_touched[ 33 ] = false;
            break;
        case 'b':
            pos_touched[ 34 ] = false;
            break;
        case 'n':
            pos_touched[ 35 ] = false;
            break;
            
        case '/':
            serial.close();
            break;
            
    }
    
    
}

//--------------------------------------------------------------
void serialReceiver::exit(){
    serial.flush( true, true );
    serial.close();
}

