#include "DistinctControl.h"

void DistControl::setup(char Name[20] ,ofPoint posup, ofPoint posdw,
                int Min, int Max, int setval, int Step, float Size, int Dwell, float red, float green, float blue){
    strcpy(name,Name);
    height2=ofGetHeight()/2;
    width2=ofGetWidth()/2;
    posUP_=posup;
    posUP.x=width2+posUP_.x*height2;
    posUP.y=height2-posUP_.y*height2;
	magPosUP=posUP;
    posDW_=posdw;
    posDW.x=width2+posDW_.x*height2;
    posDW.y=height2-posDW_.y*height2;
    min=Min;
    max=Max;
    RelSize=Size;
    sizeUP=ofGetHeight()*Size;
    sizeDW=sizeUP;
	orSize=sizeUP;
	perUP=0.8*sizeUP;
	per2UP=0.5656*sizeUP;
	perDW=0.8*sizeUP;
	per2DW=0.5656*sizeUP;
    step=Step;
    value=setval;
	dwell=(int)(variables::framerate*(float)Dwell/1000.f); //
    R=red;B=blue;G=green;
    FCUP=0;
    FCDW=0;
    activeUP=false;
    activeDW=false;
    pointsize=0.1*sizeUP+1;
	namePos.x = (posUP.x + posDW.x) / 2 - variables::myfont.stringWidth(name) / 2.f;
    namePos.y=(posUP.y+posDW.y)/2;
    color=255*(value-min)/(max-min);
    changed=false;
    displValue=false;
	EagleEnable=false;
}

void DistControl::setup(char Name[10] ,ofPoint posup, ofPoint posdw,
                int Min, int Max, int setval, int Step, float Size, int Dwell, float red, float green, float blue,bool DisplValue){
    setup( Name , posup,  posdw, Min,  Max,  setval,  Step,  Size,  Dwell,  red,  green,  blue);
    displValue=DisplValue;
}

void DistControl::update(ofPoint pt){
    changed=false;
	if(EagleEnable) eagleEye(pt);
	if(Switch::click){
		if(ofDist(pt.x,pt.y,magPosUP.x,magPosUP.y)<sizeUP)
		{
			activeUP=true;
			if (Switch::pressed && ofDist(Switch::pressedPos.x, Switch::pressedPos.y, magPosUP.x, magPosUP.y)<sizeUP) {
				Switch::pressed=false;
				value+=step;
				if(value>max) value=max;
				changed=true;
			}
		}
		else
			activeUP=false;
	}
	else{
		if(ofDist(pt.x,pt.y,magPosUP.x,magPosUP.y)<sizeUP){
			activeUP=true;
			if(FCUP++ >= dwell){
				value+=step;
				if(value>max) value=max;
				FCUP=0;
				changed=true;
			}
		}
		else{
			FCUP=0;
			activeUP=false;
		}
	}

	if(Switch::click){
		if(ofDist(pt.x,pt.y,magPosDW.x,magPosDW.y)<sizeDW)
		{
			activeDW=true;
			if (Switch::pressed && ofDist(Switch::pressedPos.x, Switch::pressedPos.y, magPosDW.x, magPosDW.y)<sizeDW) {
				Switch::pressed=false;
				value-=step;
				if(value<min) value=min;
				changed=true;
			}
		}
		else
			activeDW=false;
	}
	else{
		if(ofDist(pt.x,pt.y,magPosDW.x,magPosDW.y)<sizeDW){
			activeDW=true;
			if(FCDW++ >= dwell){
				value-=step;
				if(value<min) value=min;
				FCDW=0;
				changed=true;
			}
		}
		else{
			FCDW=0;
			activeDW=false;
		}
	}
    color=255*(value-min)/(max-min);
}


void DistControl::eagleEye(ofPoint gaze){
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

void  DistControl::draw(){
    ofSetColor(color*R,color*G,color*B);
    ofCircle(magPosUP.x,magPosUP.y,sizeUP);
    ofSetColor((255-color)*R,(255-color)*G,(255-color)*B);
    ofCircle(magPosDW.x,magPosDW.y,sizeDW);
    if(activeUP)    ofSetColor(0,255,0);
    else            ofSetColor(255,255,255);
    ofCircle(magPosUP.x,magPosUP.y,pointsize);
	
	if(fishEyeUP && EagleEnable){
		ofCircle(magPosUP.x+perUP,magPosUP.y,pointsize);
		ofCircle(magPosUP.x-perUP,magPosUP.y,pointsize);
		ofCircle(magPosUP.x,magPosUP.y+perUP,pointsize);
		ofCircle(magPosUP.x,magPosUP.y-perUP,pointsize);
		ofCircle(magPosUP.x+per2UP,magPosUP.y+per2UP,pointsize);
		ofCircle(magPosUP.x-per2UP,magPosUP.y+per2UP,pointsize);
		ofCircle(magPosUP.x+per2UP,magPosUP.y-per2UP,pointsize);
		ofCircle(magPosUP.x-per2UP,magPosUP.y-per2UP,pointsize);
	}
    if(activeDW)    ofSetColor(0,255,0);
    else            ofSetColor(255,255,255);
	if(fishEyeDW && EagleEnable){
		ofCircle(magPosDW.x+perDW,magPosDW.y,pointsize);
		ofCircle(magPosDW.x-perDW,magPosDW.y,pointsize);
		ofCircle(magPosDW.x,magPosDW.y+perDW,pointsize);
		ofCircle(magPosDW.x,magPosDW.y-perDW,pointsize);
		ofCircle(magPosDW.x+per2DW,magPosDW.y+per2DW,pointsize);
		ofCircle(magPosDW.x-per2DW,magPosDW.y+per2DW,pointsize);
		ofCircle(magPosDW.x+per2DW,magPosDW.y-per2DW,pointsize);
		ofCircle(magPosDW.x-per2DW,magPosDW.y-per2DW,pointsize);
	}
    ofCircle(magPosDW.x,magPosDW.y,pointsize);
    ofSetColor(0,0,0);
    ofLine(magPosUP.x,magPosUP.y,magPosDW.x,magPosDW.y);
    ofSetColor(variables::textColor);
    variables::myfont.drawString(name,namePos.x ,namePos.y);
    variables::myfont.drawString(ofToString(value), (posUP.x + posDW.x) / 2- variables::myfont.stringWidth(ofToString(value)) / 2.f,namePos.y+variables::myfont.getLineHeight());
	if(activeUP)
		variables::myfont.drawString(ofToString(value),magPosUP.x ,magPosUP.y+variables::myfont.getLineHeight());
	if(activeDW)
		variables::myfont.drawString(ofToString(value),magPosDW.x ,magPosDW.y+variables::myfont.getLineHeight());
}

void  DistControl::resized(int w, int h){
    sizeUP=RelSize*h;
	sizeDW=RelSize*h;
    width2=w/2;height2=h/2;
    posUP.x=width2+posUP_.x*height2;
    posUP.y=height2-posUP_.y*height2;
    posDW.x=width2+posDW_.x*height2;
    posDW.y=height2-posDW_.y*height2;
	namePos.x = (posUP.x + posDW.x) / 2 - variables::myfont.stringWidth(name)/2.f;
    namePos.y=(posUP.y+posDW.y)/2;
	magPosUP=posUP;
	magPosDW=posDW;
	orSize=sizeUP;
	pointsize=0.1*sizeUP+1;
}

void DistControl::setColorByValue(int Value){
    value=Value;
    color=255*(value-min)/(max-min);
    changed=true;
}
