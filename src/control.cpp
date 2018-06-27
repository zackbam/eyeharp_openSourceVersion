#include "control.h"

void control::setup(char Name[20], int Color ,ofPoint posup, ofPoint posdw,
                float Min, float Max, float Size, int Dwell,int Cstep, float red, float green, float blue){
    strcpy(name,Name);
    height2=ofGetHeight()/2;
    width2=ofGetWidth()/2;
    posUP_=posup;
	magPosUP=posUP;
    posUP.x=width2+posUP_.x*height2;
    posUP.y=height2-posUP_.y*height2;
    posDW_=posdw;
	magPosDW=posDW;
    posDW.x=width2+posDW_.x*height2;
    posDW.y=height2-posDW_.y*height2;
    min=Min;
    max=Max;
    RelSize=Size;
	sizeDW=ofGetHeight()*Size;
    sizeUP=ofGetHeight()*Size;
	orSize=sizeUP;
	perUP=0.8*sizeUP;
	per2UP=5656*sizeUP;
	perDW=0.8*sizeUP;
	per2DW=0.5656*sizeUP;
    color=Color;
    cstep=Cstep;
    step=(float)(max-min)/255.0 * cstep;
    value=(float)color/255.0*(float)(max-min)+min;
	//dwell=(int)(variables::framerate*(float)Dwell/1000.0); //
	dwell = Dwell;
    R=red;B=blue;G=green;
    FCUP=0;
    FCDW=0;
    activeUP=false;
    activeDW=false;
    pointsize=0.1*sizeUP+1;
	namePos.x = (posUP.x + posDW.x) / 2 - variables::myfont.stringWidth(name) / 2.f;
    namePos.y=(posUP.y+posDW.y)/2;
    changed=false;
    displayValue=false;
}

void control::setup(char Name[10], float Value ,ofPoint posup, ofPoint posdw,
                float Min, float Max, float Size, int Dwell,int Cstep, float red, float green, float blue,bool DisplayValue){
    int Color=(Value-Min)/(Max-Min)*255;
    setup(Name,  Color , posup,  posdw,Min,  Max,  Size,  Dwell, Cstep,  red,  green,  blue);
    displayValue=DisplayValue;
    value=Value;
}
void control::update(ofPoint pt){
	//eagleEye(pt);
    changed=false;

	if(Switch::click){
		if(ofDist(pt.x,pt.y,magPosUP.x,magPosUP.y)<sizeUP)
		{
			activeUP=true;
			if (Switch::pressed && ofDist(Switch::pressedPos.x, Switch::pressedPos.y, magPosUP.x, magPosUP.y)<sizeUP) {
				Switch::pressed=false;
				color+=cstep;
				value+=step;
				if(value>max) {
					value=max;
					color=255;
				}
				changed=true;
			}
		}
		else
			activeUP=false;
	}
	else{
		if(ofDist(pt.x,pt.y,magPosUP.x,magPosUP.y)<sizeUP){
			if (activeUP == false) //if in the previous framewe were out
				FCUP = ofGetElapsedTimeMillis();//save the starting time
			activeUP=true;
		
			if(ofGetElapsedTimeMillis() - FCUP >= dwell){//if we passed the time
				color+=cstep;
				value+=step;
				if(color>255-cstep){
					value=max;
					color=255;
				}
				FCUP = ofGetElapsedTimeMillis();//save the starting time
				changed=true;
			}
		}
		else{
			FCUP = ofGetElapsedTimeMillis();//save the starting time
			activeUP=false;
		}
	}

	if(Switch::click){
		if(ofDist(pt.x,pt.y,magPosDW.x,magPosDW.y)<sizeDW)
		{
			activeDW=true;
			if (Switch::pressed && ofDist(Switch::pressedPos.x, Switch::pressedPos.y, magPosDW.x, magPosDW.y)<sizeDW) {
				Switch::pressed=false;
				color-=cstep;
				value-=step;
				if(value<min){
					value=min;
					color=0;
				}
				changed=true;
			}
		}
		else
			activeDW=false;
	}
	else{
		
		if(ofDist(pt.x,pt.y,magPosDW.x,magPosDW.y)<sizeDW){
			if (activeDW == false) //if in the previous framewe were out
				FCDW = ofGetElapsedTimeMillis();//save the starting time
			activeDW=true;
		
			if(ofGetElapsedTimeMillis() - FCDW >= dwell){//if we passed the time
				color-=cstep;
				value-=step;
				if(value<min){
					value=min;
					color=0;
				}
				FCDW = ofGetElapsedTimeMillis();//save the starting time
				changed=true;
			}
		}
		else{
			FCDW = ofGetElapsedTimeMillis();//save the starting time
			activeDW=false;
		}
	}
}


void control::eagleEye(ofPoint gaze){
	//float size=buttonDiameter*ofGetHeight()/2;
	
				float xx=(posUP.x-gaze.x)/ofGetHeight()*mag;
				float yy=(posUP.y-gaze.y)/ofGetHeight()*mag;
				float r=(xx*xx+yy*yy);
				float xd=xx*(1+magFac*r);
				float yd=yy*(1+magFac*r);
				sizeUP = orSize*(1+magFac*r)*mag;
				if(sizeUP>orSize){
					magPosUP.x = xd*ofGetHeight()+gaze.x;
					magPosUP.y = yd*ofGetHeight()+gaze.y;
				
					perUP = sizeUP*0.8;
					per2UP = sizeUP*0.5656;
					fishEyeUP = true;
				}
			
			else{
				sizeUP=orSize;
				perUP = sizeUP*0.8;
				per2UP = sizeUP*0.5656;
				magPosUP.x = posUP.x;
				magPosUP.y = posUP.y;
				fishEyeUP = false;
			}
				xx=(posDW.x-gaze.x)/ofGetHeight()*mag;
				yy=(posDW.y-gaze.y)/ofGetHeight()*mag;
				r=(xx*xx+yy*yy);
				xd=xx*(1+magFac*r);
				yd=yy*(1+magFac*r);
				sizeDW = orSize*(1+magFac*r)*mag;
				if(sizeDW>orSize){
					magPosDW.x = xd*ofGetHeight()+gaze.x;
					magPosDW.y = yd*ofGetHeight()+gaze.y;
				
					perDW = sizeDW*0.8;
					per2DW = sizeDW*0.5656;
					fishEyeDW = true;
				}
			
			else{
				sizeDW=orSize;
				perDW = sizeDW*0.8;
				per2DW = sizeDW*0.5656;
				magPosDW.x = posDW.x;
				magPosDW.y = posDW.y;
				fishEyeDW = false;
			}
}


void control::draw(){
    ofSetColor(color*R,color*G,color*B);
    ofCircle(magPosUP.x,magPosUP.y,sizeUP);
    ofSetColor((255-color)*R,(255-color)*G,(255-color)*B);
    ofCircle(magPosDW.x,magPosDW.y,sizeDW);
    if(activeUP)    ofSetColor(0,255,0);
    else            ofSetColor(255,255,255);
    ofCircle(magPosUP.x,magPosUP.y,pointsize);
	/*if(fishEyeUP){
		ofCircle(magPosUP.x+perUP,magPosUP.y,pointsize);
		ofCircle(magPosUP.x-perUP,magPosUP.y,pointsize);
		ofCircle(magPosUP.x,magPosUP.y+perUP,pointsize);
		ofCircle(magPosUP.x,magPosUP.y-perUP,pointsize);
		ofCircle(magPosUP.x+per2UP,magPosUP.y+per2UP,pointsize);
		ofCircle(magPosUP.x-per2UP,magPosUP.y+per2UP,pointsize);
		ofCircle(magPosUP.x+per2UP,magPosUP.y-per2UP,pointsize);
		ofCircle(magPosUP.x-per2UP,magPosUP.y-per2UP,pointsize);
	}*/
    if(activeDW)    ofSetColor(0,255,0);
    else            ofSetColor(255,255,255);
    ofCircle(magPosDW.x,magPosDW.y,pointsize);
	/*if(fishEyeDW){
		ofCircle(magPosDW.x+perDW,magPosDW.y,pointsize);
		ofCircle(magPosDW.x-perDW,magPosDW.y,pointsize);
		ofCircle(magPosDW.x,magPosDW.y+perDW,pointsize);
		ofCircle(magPosDW.x,magPosDW.y-perDW,pointsize);
		ofCircle(magPosDW.x+per2DW,magPosDW.y+per2DW,pointsize);
		ofCircle(magPosDW.x-per2DW,magPosDW.y+per2DW,pointsize);
		ofCircle(magPosDW.x+per2DW,magPosDW.y-per2DW,pointsize);
		ofCircle(magPosDW.x-per2DW,magPosDW.y-per2DW,pointsize);
	}*/
    ofSetColor(0,0,0);
    ofLine(magPosUP.x,magPosUP.y,magPosDW.x,magPosDW.y);
    ofSetColor(variables::textColor);
    variables::myfont.drawString(name,namePos.x ,namePos.y);
    if(displayValue)    variables::myfont.drawString(ofToString(value,0),namePos.x ,namePos.y+variables::myfont.getLineHeight());
    else                variables::myfont.drawString(ofToString(color),namePos.x ,namePos.y+variables::myfont.getLineHeight());
}

void control::resized(int w, int h){
	sizeUP=RelSize*h;
	sizeDW=RelSize*h;
    orSize=RelSize*h;
    width2=w/2;height2=h/2;
    posUP.x=width2+posUP_.x*height2;
    posUP.y=height2-posUP_.y*height2;
    posDW.x=width2+posDW_.x*height2;
    posDW.y=height2-posDW_.y*height2;
	namePos.x = (posUP.x + posDW.x) / 2 - variables::myfont.stringWidth(name) / 2.f;
    namePos.y=(posUP.y+posDW.y)/2;
	magPosUP=posUP;
	magPosDW=posDW;
	pointsize=0.1*sizeUP+1;
	
}

void control::setColorByValue(float Value){
    value=Value;
    color=255.0f*(float)(value-min)/(float)(max-min);
    changed=true;
}

void control::setValueByColor(int Color){
    color=Color;
    value=(float)color/255.0*(float)(max-min)+min;
    changed=true;
}
