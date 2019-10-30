#include "Switch.h"
#include "stdio.h"
#include "EyeHarpVARS.h"

bool Switch::focuspoints=false;
bool Switch::click=false;
bool Switch::pressed=false;
ofPoint Switch::MagWindowCenter;
ofPoint Switch::pressedPos;
float Switch::Mag=2;
float Switch::MagFac=0;
int Switch::MagRegion;
bool Switch::magActive=false;
bool Switch::lenseSlave=false;
//bool Switch::windowCenterChanged=false;
int Switch::magThr=0;
int Switch::magOffset=0;

void Switch::setup(char Name[20] ,bool Value, ofPoint Pos,float Size, int Dwell, float red, float green, float blue, bool EagleOnOff){
	strcpy(name,Name);
	height2=ofGetHeight()/2;
	width2=ofGetWidth()/2;
	relSize=Size;
	size=ofGetHeight()*Size;
	orSize=size;
	per=0.9*size;
	per2=0.6364*size;
	//dwell=(int)((float)Dwell/1000.0*variables::framerate); //  
	dwell = Dwell;
	R=red;B=blue;G=green;
	pos_=Pos;
	pos.x=width2+pos_.x*height2;
	pos.y=height2-pos_.y*height2;
	magPos=pos;
	value=Value;
	active=false;
	changed=false;
	lock=false;
	FC=0;
	pointsize=0.05*size+1;
	fishEye=0;
	eagleEnable=EagleOnOff;
	arrowsAccess=false;
	magout=0;
	Switch::MagRegion = height*0.14;
}

void Switch::update(ofPoint gazee){
	gaze=gazee;
	if(eagleEnable)
		eagleEye(gaze);
	if(click){
		changed=false;
		if(ofDist(gaze.x,gaze.y,magPos.x,magPos.y)<size)
		{
			active=true;
			if(Switch::pressed && ofDist(Switch::pressedPos.x, Switch::pressedPos.y, magPos.x, magPos.y)<size){
				Switch::pressed=false;
				value=!value;
				changed=true;
			}
		}
		else
			active=false;
	}
	else{
		if(changed)    lock=true;
		if(!active)    lock=false;
		changed=false;
		if(ofDist(gaze.x,gaze.y,magPos.x,magPos.y)<size){
			if (active == false) //if in the previous framewe were out
				FC = ofGetElapsedTimeMillis();//save the starting time
			active=true;
			dwellFraction = (ofGetElapsedTimeMillis() - FC) / dwell;
			if(dwellFraction > 1.0 && !lock && !click) {
				value=!value;
				FC = ofGetElapsedTimeMillis();
				changed=true;
			}
		}
		else{
			active=false;
			FC = ofGetElapsedTimeMillis();
		}
	}
}



void Switch::update(ofPoint gazee,bool*sacadic){
	gaze=gazee;
	if(eagleEnable){
		//if( *sacadic || windowCenterChanged)
			eagleEye(MagWindowCenter);
		if(!magActive){
			size=orSize;
			per = size*0.9;
			per2 = size*6364;
			magPos.x = pos.x;
			magPos.y = pos.y;
			fishEye = false;
		}
		if(size>orSize){
			if(magPos.y<size){
				magOffset++;
				//	cout<<magOffset<<'\t';
				//MagWindowCenter.y--;
				//windowCenterChanged=true;
			}
			else if(magPos.y>height-size){
				magOffset--;
				//MagWindowCenter.y++;
				//windowCenterChanged=true;
			}
		}
	}
	if(click){
		changed=false;
		if(ofDist(gaze.x,gaze.y,magPos.x,magPos.y)<size && (size>orSize || !eagleEnable))
		{
			active=true;
			if (Switch::pressed && ofDist(Switch::pressedPos.x, Switch::pressedPos.y, magPos.x, magPos.y)<size) {
				Switch::pressed=false;
				value=!value;
				changed=true;
			}
		}
		else
			active=false;
	}
	else{
		if (changed)    lock = true;
		if (!active)    lock = false;
		changed = false;
		if (ofDist(gaze.x, gaze.y, magPos.x, magPos.y)<size) {
			if (active == false) //if in the previous framewe were out
				FC = ofGetElapsedTimeMillis();//save the starting time
			active = true;
			if (ofGetElapsedTimeMillis() - FC >= dwell && !lock && !click) {
				value = !value;
				FC = ofGetElapsedTimeMillis();
				changed = true;
			}
		}
		else {
			active = false;
			FC = ofGetElapsedTimeMillis();
		}
	}
}

void Switch::setColor(float red, float green, float blue){
	R=red;G=green;B=blue;
}

void Switch::updatePosSize(float pos_x,float pos_y,float Size){
	pos_.x=pos_x;
	pos_.y=pos_y;
	pos.x=width2+pos_.x*height2;
	pos.y=height2-pos_.y*height2;
	magPos=pos;
	relSize=Size;
	size=height*relSize;
	orSize=size;
}


void Switch::eagleEye(ofPoint gaze){
	//if(gaze.y>height*0.85)//in order to be able to choose the first and final line
	//	gaze.y=height*0.98;
	//if(gaze.y<height*0.15)
	//	gaze.y=height*0.02;
	//float size=buttonDiameter*ofGetHeight()/2;
	float xx=(pos.x-gaze.x)/ofGetHeight()*Mag;
	float yy=(pos.y-gaze.y)/ofGetHeight()*Mag;
	float r=(xx*xx+yy*yy);
	float xd=xx*(1+MagFac*r);
	float yd=yy*(1+MagFac*r);
	size = orSize*(1+MagFac*r)*Mag;
	//float dist=ofDist(pos.x,pos.y,gaze.x,gaze.y);
	if(size>orSize && abs(pos.x-MagWindowCenter.x)<MagRegion && abs(pos.y-MagWindowCenter.y)<MagRegion && magActive){
	//if(abs(pos.x-MagWindowCenter.x)<100 && abs(pos.y-MagWindowCenter.y)<100){
		magPos.x = xd*ofGetHeight()+gaze.x;
		magPos.y = yd*ofGetHeight()+gaze.y+magOffset;
		magout=0;
		if(magPos.y<0)
			magout=magPos.y;
		else if(magPos.y>height)
			magout=magPos.y-height;
		per = size*0.9;
		per2 = size*0.6364;
		fishEye = true;
		
	}
	//MagWindowCenter.x-pos.x+size<100 && pos.x-size-MagWindowCenter.x<100 && pos.y-size-MagWindowCenter.y<100 && MagWindowCenter.y-pos.y+size<100
	//else if((pos.x>MagWindowCenter.x && pos.x-size-MagWindowCenter.x<100) || (pos.y>MagWindowCenter.y && pos.y-size-MagWindowCenter.y<100) || (pos.x<MagWindowCenter.x && MagWindowCenter.x-pos.x+size<100) || (pos.y<MagWindowCenter.y && MagWindowCenter.y-pos.y+size<100)){
	/*else if(abs(pos.x-MagWindowCenter.x)<100+size && abs(pos.y-MagWindowCenter.y)<100+size){
			Draw=false;
	}*/
	else{
		size=orSize;
		per = size*0.9;
		per2 = size*0.6364;
		magPos.x = pos.x;
		magPos.y = pos.y;
		fishEye = false;
	}
}



void Switch::draw(){
	bool magnified=false;
	if(size>orSize) magnified=true;
	if (!value)  ofSetColor(R*30+10*magnified,G*30+100*arrowsAccess+10*magnified,B*30+10*magnified);
	else        ofSetColor(R*255+10*magnified,G*255+100*arrowsAccess+10*magnified,B*255+10*magnified);
	ofCircle(magPos.x,magPos.y,size);
	//ofDrawSphere(magPos.x,magPos.y,magPos.z,size);
	if(active)      ofSetColor(0,255,0);
	else            ofSetColor(255,255,255);

	ofCircle(magPos.x,magPos.y,pointsize);
	if(focuspoints){
		if(/*(fishEye && eagleEnable) ||*/ ofDist(gaze.x,gaze.y,magPos.x,magPos.y)< size ){
			ofCircle(magPos.x+per,magPos.y,pointsize);
			ofCircle(magPos.x-per,magPos.y,pointsize);
			ofCircle(magPos.x,magPos.y+per,pointsize);
			ofCircle(magPos.x,magPos.y-per,pointsize);
			ofCircle(magPos.x+per2,magPos.y+per2,pointsize);
			ofCircle(magPos.x-per2,magPos.y+per2,pointsize);
			ofCircle(magPos.x+per2,magPos.y-per2,pointsize);
			ofCircle(magPos.x-per2,magPos.y-per2,pointsize);
		}
	}
	//ofDrawSphere(magPos.x,magPos.y,magPos.z,pointsize);
	ofSetColor(variables::textColor);
	variables::myfont.drawString(name,magPos.x- variables::myfont.stringWidth(name) / 2.f,magPos.y);
	

}

void Switch::resized(int w, int h){
	size=relSize*h;
	pointsize=0.05*size+1;
	orSize=size;
	per=0.9*size;
	per2=0.6364*size;
	width2=w/2;height2=h/2;
	width=w;
	height=h;
	pos.x=width2+pos_.x*height2;
	pos.y=height2-pos_.y*height2;
	magPos=pos;
}


