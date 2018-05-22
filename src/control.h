#ifndef _CONTROLS
#define _CONTROLS


#include "ofMain.h"

#include "EyeHarpVARS.h"
#include "switch.h"

class control{
    public:
        void setup(char Name[20] ,int Color ,ofPoint posup, ofPoint posdw,
                float Min, float Max, float Size, int Dwell, int Cstep, float red, float green, float blue);
        void setup(char Name[20] ,float Value ,ofPoint posup, ofPoint posdw,
                float Min, float Max, float Size, int Dwell, int Cstep, float red, float green, float blue,bool DisplayValue);
        void update(ofPoint Gaze);
        void draw();
        void resized(int w, int h);
        int color; //0 to 255
        float value; // scaled appropriately
        ofPoint posUP_; //relative position of the button for increasing the value
                        //posUP_.x from -1.5 to 1.5 and posUP_.y -1 to 1
        ofPoint posDW_;
        bool changed;
        void setColorByValue(float Value);
        void setValueByColor(int Color);
		void eagleEye(ofPoint);
        float min;
        float max;
		ofPoint magPosUP;
		ofPoint magPosDW;
		int perUP;
		int per2UP;
		int perDW;
		int per2DW;
		bool fishEyeUP;
		bool fishEyeDW;
    private:
        int height2;
        int width2;
        ofPoint posUP;
        ofPoint posDW;
        float dwell; //number of frames to change the value
        int cstep; //the step to change values (in the scale of 0-255)
		int orSize;
        int sizeUP;
		int sizeDW;
        int pointsize; //the size of the point in the middle
        float RelSize;//size related to the height; e.g. 0.06 of the height
        float R,G,B;
        int FCUP;
        int FCDW;
        float step;//step to change the value (rescaled to min-max)
        bool activeUP; //we are changing the value of this button
        bool activeDW;
        char name[20];
        ofPoint namePos; //position of the name to be printed

        bool displayValue;//is true if we want to display the real value, not color (0,255)
};

#endif
