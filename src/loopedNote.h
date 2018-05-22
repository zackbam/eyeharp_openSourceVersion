#ifndef _LOOPNOTE
#define _LOOPNOTE


#include "ofMain.h"
#include "control.h"
#include "EyeHarpVARS.h"

class loopedNote{
    public:
        void setup(int* curSample,int * Transpose, int MidiNote, int * WaitSamples, float* NextPhaseAdder, float * Volume, int * AttackSamples, float * AttackFactor, control * ReleaseFactor, int * SampleRate,int * Bright);
        void update();
        float getSample();
    private:
        float * volume;
        int * curSample;
        float * attackFactor;
        int * attackSamples;
        control * releaseFactor;
        int * sampleRate;
        int * waitSamples;//time in samples to wait before start.
        float phase;
        float phaseAdder;
        float * nextPhaseAdder;//is used to change the midivalue only during the attack
        int * transpose;
        int * nextBright;
        int bright;
};

#endif
