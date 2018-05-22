#ifndef _ARPEGGIOBUTTON
#define _ARPEGGIOBUTTON


#include "ofMain.h"

class arpeggioButton{
    public:
        void setup();
        void update();
        void draw();
        int meter;
        int tempo;
        int patternStep;
        int globalStep;
        int StartingNote;
        int notesIncluded[7];
        int patternLength;
        ofPoint position;


}
