#include "slider.h"
#include "EyeHarpVARS.h"
void slider::setup(ofPoint LinePos,int Value, int Min, int Max, float LineSize,bool Right){
    width2=ofGetWidth()/2;
    height2=ofGetHeight()/2;
    right=Right;
    linePos_=LinePos;
    linePos.x=width2+linePos_.x*height2;
    linePos.y=height2-linePos_.y*height2;
    if(right)   linePos2.x=linePos.x+0.06f*ofGetHeight();
    else        linePos2.x=linePos.x-0.06f*ofGetHeight();
    linePos2.y=linePos.y;
    value=Value;
    min=Min;
    max=Max;
    lineSize_=LineSize;
    lineSize=lineSize_*ofGetHeight();
    selectPos.x=linePos2.x;
    selectPos.y=linePos2.y+lineSize/2-lineSize*(value-min)/(max-min);
    spotSize=0.01f*ofGetHeight();
    timer=0;
    changed=false;
}

void slider::update(ofPoint gaze){
    changed=false;
    if(ofDist(gaze.x,gaze.y,linePos2.x,gaze.y)<0.02*ofGetHeight() && abs((int)(linePos2.y-gaze.y))<=lineSize/2 ){
        if(timer++>20){
            changed=true;
            value=(int)(((linePos2.y+lineSize/2)-gaze.y)/lineSize*(max-min)+min);
            selectPos.y=linePos2.y+lineSize/2-lineSize*(value-min)/(max-min);
            timer=0;
        }
    }
    else    timer=0;
}

void slider::draw(){
    ofSetColor(250,100,20);
    ofLine(linePos.x , (linePos.y-lineSize/2) , linePos.x , (linePos.y+lineSize/2) );
    ofLine(linePos2.x , (linePos2.y-lineSize/2) , linePos2.x , (linePos2.y+lineSize/2) );
    int bitmapOffset=0.009f*ofGetHeight();
    int bitmapX=linePos.x-0.04f*ofGetHeight()*(right);
    int y=linePos2.y+lineSize/2;
    int pry=y+21;
    for(int i=0;i<=max-min;i++){
        y=linePos2.y+lineSize/2-lineSize*i/(max-min);
        if(pry-y>20){
            variables::myfont.drawString(ofToString(i+min),bitmapX,bitmapOffset+y);
            ofCircle(linePos2.x,y,spotSize);
            pry=y;
        }
    }
    ofCircle(selectPos.x,selectPos.y,0.02*ofGetHeight());
    ofSetColor(255,255,255);
    variables::myfont.drawString(ofToString(value),selectPos.x-7,selectPos.y+4);
}

void slider::resized(int w, int h){
    width2=w/2;
    height2=h/2;
    linePos.x=width2+linePos_.x*height2;
    linePos.y=height2-linePos_.y*height2;
    linePos2.y=linePos.y;
    if(right)   linePos2.x=linePos.x+0.06f*h;
    else        linePos2.x=linePos.x-0.06f*h;
    lineSize=lineSize_*h;
    selectPos.x=linePos2.x;
    selectPos.y=linePos2.y+lineSize/2-lineSize*(value-min)/(max-min);
}

void slider::setValue(int Value){
    value=Value;
    selectPos.y=linePos2.y+lineSize/2-lineSize*(value-min)/(max-min);
}
