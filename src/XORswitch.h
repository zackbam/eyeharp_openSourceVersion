#ifndef _XORSWITCH
#define _XORSWITCH

#include "switch.h"

#include "ofMain.h"

class xorSwitch{
    public:
        void setup(int Num,char **Names, int defVal,ofPoint Pos,float Direction,float Size, int Dwell);//num:number of buttons. defVal: Default button selected.
                                                        // Direction: the angle in which note are placed
        void update(ofPoint Gaze);
        void draw();
        void resized(int w, int h);
        int selected;//the selected button.
        bool changed;
        void setValue(int);

    private:
        int num; //the number of buttons
        Switch * button;//the array of buttons
        ofPoint pos;//the angle in which the buttons are placed
        float size;//relative size e.g. 0.05 of window height
        int dwell;
};


#endif
