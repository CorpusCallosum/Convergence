
//
//  currentColor.h
//  
//
//  Created by curry on 5/4/16.
//
//

#ifndef serialReceiver_h
#define serialReceiver_h


#include "ofMain.h"
#define NBOARDS 3
#define NPINS 12

class serialReceiver : public ofBaseApp{
	
public:
    
    //methods
    void setup( int t , int numRods, int rodSpacing);
    void update();
    void draw(int x, int y);
    void serialFunction();
    void keyPressed  (int key);
    void keyReleased(int key);
    void exit();
    
    //variables
    ofSerial serial;
    
    bool pos_touched[ NBOARDS * NPINS ];
    bool touched[ NBOARDS * NPINS ];
    bool lastTouched[ NBOARDS * NPINS ];
    int readings[ NBOARDS * NPINS ];
    int smoothedReadings[ NBOARDS * NPINS ];
    int readingsArrays[ NBOARDS * NPINS ][1000];
    vector< vector<int> > readingsVectors;
    int numReadingsStored, numReadingsSmoothed;
    int baselines[ NBOARDS * NPINS ];
    float averages[ NBOARDS * NPINS ];
    float diffs[ NBOARDS * NPINS ];
    bool start;
    
    //int nPins[ NBOARDS ];
    ofColor color[ NBOARDS * NPINS ];
    
    int boxX, boxY, currentReadingIndex;
    
    int touch_time[ NBOARDS * NPINS ];
    int current_time;
    int false_touch_timeout;
    
    int _rodSpacing, _numRods, place;

};

#endif