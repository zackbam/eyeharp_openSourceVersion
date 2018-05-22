#ifndef _MOVSWITCH
#define _MOVSWITCH

#include "ofMain.h"
#include "switch.h"

class movingSwitch : public Switch {
    public:
        void update(ofPoint Gaze);
        static bool isPressed;//indicates if one of the buttons is active. We want just one button to be active at a time.
        void draw(string xval,string yval);
    private:
        ofPoint gaze;
        ofPoint prGaze;
};

#endif
