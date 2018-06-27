#ifndef _DISTCONTROLS
#define _DISTCONTROLS


#include "ofMain.h"
#include "switch.h"
#include "EyeHarpVARS.h"

class DistControl{
    public:
        void setup(char Name[20] ,ofPoint posup, ofPoint posdw,
                int Min, int Max, int setval, int Step, float Size, int Dwell, float red, float green, float blue);
        void setup(char Name[20] ,ofPoint posup, ofPoint posdw,
                int Min, int Max, int setval, int Step, float Size, int Dwell, float red, float green, float blue, bool DispValue);
        void update(ofPoint Gaze);
        void draw();
        void resized(int w, int h);
        int value;
        bool changed;
        void setColorByValue(int value);
		void eagleEye(ofPoint);
        int color;
        float min;
        float max;
        bool activeUP; //we are changing the value of this button
        bool activeDW;
		bool EagleEnable; 
		ofPoint posUP_; //relative position of the button for increasing the value
										  //posUP_.x from -1.5 to 1.5 and posUP_.y -1 to 1
		ofPoint posDW_;
		ofPoint posUP;
		ofPoint posDW;
		ofPoint magPosUP;
		ofPoint magPosDW;
    private:
        int height2;
        int width2;
        void scale();
        
		int perUP;
		int per2UP;
		int perDW;
		int per2DW;
		bool fishEyeUP;
		bool fishEyeDW;
        float dwell; //number of frames to change the value
        int step; //the step to change values (in the scale of 0-255)

        int sizeUP;
		int sizeDW;
        int pointsize; //the size of the point in the middle
        float RelSize;//size related to the height; e.g. 0.06 of the height
		int orSize;
        float R,G,B;
        uint64_t FCUP;
		uint64_t FCDW;
        char name[20];
        ofPoint namePos; //position of the name to be printed
        bool displValue;//is true if the value
};

#endif
