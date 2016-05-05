#pragma once

#include "ofMain.h"
#define NBOARDS 3
#define NPINS 12

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        //serial read from cap sense boards
        void serialFunction();
        ofSerial serial;
        
        bool pos_touched[ NBOARDS * NPINS ];
        bool touched[ NBOARDS * NPINS ];
        bool lastTouched[ NBOARDS * NPINS ];
        
        int touch_time[ NBOARDS * NPINS ];
        int current_time;
        int false_touch_timeout;

};
