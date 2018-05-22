#include "movingSwitch.h"
#include "EyeHarpVARS.h"

bool movingSwitch::isPressed=false;

void movingSwitch::update(ofPoint Gaze){
    changed=false;
    prGaze=gaze;
    gaze=Gaze;
    if(ofDist(gaze.x,gaze.y,pos.x,pos.y)<size || value){
        active=true;
        if(!value && !movingSwitch::isPressed){
            if(FC++ >= dwell) {
                value=true;
                movingSwitch::isPressed=true;
                FC=0;
                changed=true;
            }
        }
        if(value){
            pos=gaze;
            pos_.x=(float)(pos.x-width2)/height2;
            pos_.y=(float)(height2-pos.y)/height2;
            if(ofDist(gaze.x,gaze.y,prGaze.x,prGaze.y)*variables::framerate < 500) {//if fixation detected
                if(FC++ >= dwell){
                    value=false;
                    movingSwitch::isPressed=false;
                    FC=0;
                    changed=true;
                }
            }
            else{
                FC=0;
            }
        }
    }

    else{
        active=false;
        FC=0;
    }
}

void movingSwitch::draw(string xval,string yval){
    ofEnableAlphaBlending();
    if (!value)  ofSetColor(R*255,G*255,B*255);
    else        ofSetColor(R*255,G*255,B*255,200);
    ofCircle(pos.x,pos.y,size);
    if(active)      ofSetColor(0,255,0,200);
    else            ofSetColor(255,255,255,200);
    ofCircle(pos.x,pos.y,pointsize);
    ofSetColor(variables::textColor);
    string x="";
    string y="";
    variables::myfont.drawString(x.append(xval),pos.x ,pos.y-20);
    variables::myfont.drawString(y.append(yval),pos.x ,pos.y+25);
    ofDisableAlphaBlending();
}
