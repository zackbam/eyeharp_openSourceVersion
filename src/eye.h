#ifndef _EYE
#define _EYE


#include "EyeHarpVARS.h"
#include "ofMain.h"
#include "Disc.h"
#include "control.h"
#include "DistinctControl.h"
#include "switch.h"
#include "controlArpeggio.h"
#include "XORswitch.h"
#include "slider.h"

class Eye{
    public:
        Disc disc;
        void setup(int * chord,DistControl* Scale,bool* conf,int noteNumbers,bool tomidi, bool semitoneActive);
        void update(ofPoint Gaze, float * velocity,bool *sacadic);
        void draw();
        void resized(int w, int h);
        control glissanto;
        control volume;
        control attack;
        control release;
        control vibratoHz;
        control vibratoW;
        control harmonic[7];
        DistControl octave;
        int oct;
        float Ssize;
        float Msize;
        float XSsize;
		bool* conf;
        controlArp arpInterface;
        xorSwitch multiplex;
        xorSwitch controls;
        float vibratoPhaseAdder;
        Switch  advanced;
        xorSwitch timbrePresets;
        void setTimbrePreset();
        int width;int width2;
        int height;int height2;
		DistControl * scale;
    private:
        
        ofPoint multiPos;
        ofPoint sliderPos;
        ofPoint stepPosUP;ofPoint stepPosDW;
//        bool *advanced;//basic or advanced mode
        ofPoint gaze;
        float * velocity;
        slider notesNumSlider;
        slider vibratoWSlider;
        slider vibratoHzSlider;
        slider releaseSlider;
        slider attackSlider;
        slider glissantoSlider;
        slider volumeSlider;
        void basicMode();
        Switch playArpeggio;
};

#endif
