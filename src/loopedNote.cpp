#include "loopedNote.h"

// void setup(int* curSample, int MidiNote, int * TotalSamples, int WaitSamples, float * Volume, int * AttackSamples, float * AttackFactor, float * ReleaseFactor, int * SampleRate);

void loopedNote::setup(int* CurSample, int* Transpose, int MidiNote, int * WaitSamples,float* NextPhaseAdder, float * Volume, int * AttackSamples, float * AttackFactor, control * ReleaseFactor, int * SampleRate,int* Bright){
    curSample=CurSample;
    waitSamples=WaitSamples;
    volume=Volume;
    attackSamples=AttackSamples;
    attackFactor=AttackFactor;
    releaseFactor=ReleaseFactor;
    sampleRate=SampleRate;
    transpose=Transpose;
    int Note=MidiNote-60;//midi note 57 is A3 at 220Hz. If sth is lower than that thw pow function will be < 1
    float targetFrequency= TUNING/2*(float)pow(2,(Note+*transpose)/12.0f);
    phaseAdder= (targetFrequency / (float) *sampleRate) * 6.2831f;
    phase=0;
//    attackSamples=attackTime*sampleRate/1000.0f;
//    attackFactor=10.0f/attackSamples;//FIX THIS
//    int releaseSamples=releaseTime*sampleRate/5000.0f;
//    releaseFactor=(1-1.0f/releaseSamples);
//    curSample=0;
    nextPhaseAdder=NextPhaseAdder;
    nextBright=Bright;
    bright=*nextBright;
}


void loopedNote::update(){
}

float loopedNote::getSample(){
    float sample;
    while (phase > 6.2831f){
        phase -= 6.2831f;
    }

    if(*curSample==*waitSamples){
        phaseAdder=*nextPhaseAdder;
        bright=*nextBright;
    }
    phase += phaseAdder;
    switch(bright){
        case 0:
            sample=sin(phase);
            break;
        case 1:
            sample=sin(phase)+0.1*sin(2*phase)+0.1*sin(5*phase);
            break;
        case 2:
            sample=0.79f*sin(phase)+
                0.0158f*sin(2*phase)+
                0.0125f*sin(3*phase)+
                0.0398f*sin(phase)+
                0.001f*sin(2*phase)+
                0.0015f*sin(3*phase)+
                0.0003f*sin(3*phase);
            break;
        case 3:
            sample=sin(phase)+0.2*sin(5*phase)+0.01*sin(6*phase)+0.01*sin(7*phase)+0.2*sin(8*phase);
            break;
    }
//    cout<<phaseAdder<<'\t'<<tvol<<'\t'<<sample<<'\t'<<curSample<<'\t'<<waitSamples<<'\t'<<attackSamples<<'\n';

//    curSample=(curSample+1)%loopedNote::noteSamples;
    return sample;
}
