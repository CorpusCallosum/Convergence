//
//  utils.cpp
//  convergence_app
//
//  Created by Floating Point on 5/8/16.
//
//

#include "utils.hpp"

float utils::getColorSimilarity(ofColor colorA, ofColor colorB){
    //do color comparison here...
    float beta = colorB.getHueAngle();
    float alpha = colorA.getHueAngle();
    int phi = (int)abs(beta - alpha) % 360;       // This is either the distance or 360 - distance
    float distance = phi > 180 ? 360 - phi : phi;
    //cout<< "color distanc:"<<distance<<endl;
    float colorSimilarity = distance/180;
    //cout<< "color similarity:"<<colorSimilarity<<endl;
    return colorSimilarity;
}