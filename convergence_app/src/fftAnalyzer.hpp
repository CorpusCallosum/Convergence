//
//  fft.hpp
//  convergence_app
//
//  Created by Jack Kalish on 6/2/17.
//
//

#ifndef fft_hpp
#define fft_hpp

#include <stdio.h>

#endif /* fft_hpp */

#include "ofMain.h"
#include "ofxEasyFft.h"

class fftAnalyzer{
public:
    void setup();
    void update();
    void draw(int x, int y);
    
    void plot(vector<float>& buffer, float scale);
    
    ofxEasyFft fft;
    
    float averageFrequency;
};
