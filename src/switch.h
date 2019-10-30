#ifndef _SWITCH
#define _SWITCH

#include "ofMain.h"

class Switch{
    public:
        void setup(char Name[20] ,bool Value, ofPoint pos, float Size, int Dwell, float red, float green, float blue, bool EagleOnOff);
        void update(ofPoint Gaze);
		void update(ofPoint Gaze,bool*sacadic);
        void setColor(float R,float G,float B);
        void draw();
        void resized(int w, int h);
        void updatePosSize(float posx,float posy,float Size);
		void eagleEye(ofPoint Gaze);
        bool value;
        bool changed;
        ofPoint pos;
		bool eagleEnable;
        ofPoint pos_;
        int size;
		int orSize;
		int per;
		int per2;
		bool fishEye;
        bool active;
        bool lock;
		ofPoint magPos;
		ofPoint gaze;
        int pointsize; //the size of the point in the middle
		static bool focuspoints;
		static bool click;
		static bool pressed;
		static ofPoint pressedPos;
		bool arrowsAccess;
		static ofPoint MagWindowCenter;
		static float Mag;
		static float MagFac;
		static int MagRegion;
		static bool magActive;
		static bool lenseSlave;
		int magout; //if 0 ok, if <0 too up, if >0 too down
		static int magOffset;
		//static bool windowCenterChanged;
		static int magThr;
		char name[100];
    protected:
        uint64_t FC;
        float dwell;
		float dwellFraction; // from 0 to 1
		int height;
		int width;
        int height2;
        int width2;

        float R,G,B;
    private:
        float relSize;//relative size
};



#endif
