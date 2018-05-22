#ifndef _EYESLIDER
#define _EYESLIDER


#include "ofMain.h"

class slider{
    public:
        void setup(ofPoint LinePos,int Value, int Min, int Max, float LineSize, bool Right);
        void update(ofPoint gaze);
        void draw();
        void resized(int,int);
        bool changed;
        void setValue(int Value);
        int value;
    private:
        ofPoint linePos;
        ofPoint linePos_;
        ofPoint selectPos;
        ofPoint selectPos_;
        ofPoint linePos2;
        int lineSize;
        float lineSize_;
        int min;
        int max;
        int width2;
        int height2;
        int spotSize;
        int timer;
        bool right;
};

#endif
