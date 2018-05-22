#include "XORswitch.h"

void xorSwitch::setup(int Num, char** names, int defVal, ofPoint Pos, float direction, float Size, int Dwell){
    num=Num;
    button=new Switch[num];
    selected=defVal;
    pos=Pos;
    size=Size;
    ofPoint cur;//current' s button position
    dwell=Dwell;
    cur.x=pos.x;
    cur.y=pos.y;
    changed=false;
    for(int i=0;i<num;i++){
        cur.x+=cos(direction)*4*size;
        cur.y+=sin(direction)*4*size;
        button[i].setup(names[i],i==defVal,cur,size,dwell,0.4,0.25,0,false);
    }

}

void xorSwitch::update(ofPoint gaze){
    changed=false;
    for(int i=0;i<num;i++){
         button[i].update(gaze);
         if(button[i].changed){
            for(int j=0;j<num;j++) button[j].value=false;
            button[i].value=true;
            selected=i;
            changed=true;
         }
    }
}

void xorSwitch::setValue(int Selected){
    for(int j=0;j<num;j++) button[j].value=false;
    button[Selected].value=true;
    selected=Selected;
}

void xorSwitch::draw(){
    ofSetColor(255,255,255);
    for(int i=0;i<num;i++){
        button[i].draw();
    }
}

void xorSwitch::resized(int w,int h){
    for(int i=0;i<num;i++){
        button[i].resized(w,h);
    }
}

