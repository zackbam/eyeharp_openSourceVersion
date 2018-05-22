#ifndef _CONTROLARP
#define _CONTROLARP


#include "ofMain.h"
#include "DistinctControl.h"
#include "arpeggio.h"
#include "XORswitch.h"
#include "control.h"
#include "slider.h"

class controlArp{
    public:
        void setup(DistControl * Scale,int * Transpose, int* Chord, int* TotalSamples, int* SampleRate,int* CurSample);
        void update(ofPoint Gaze);
        float getSample();
        void updateTempo();
        void updateChords();
        void draw();
        void resized(int,int);
        arpeggio arpeggios[4];
        float arpVolume;
    private:
        int* curSample;
        int * totalSamples;
        DistControl* scale;
        int* chord;
        xorSwitch selectArp;
        int* sampleRate;
        control volume;
        DistControl globalStep;
        DistControl patternStep;
        DistControl startingNote;
        DistControl patternSize;
        DistControl meter;
        DistControl notesIncl;

        float curArpVol;

        slider volumeSlider;
        slider globalStepSlider;
        slider patternStepSlider;
        slider startingNoteSlider;
        slider patternSizeSlider;
        slider meterSlider;
        slider notesInclSlider;

        xorSwitch controls;
};

#endif
