#ifndef _SEQNOTE
#define _SEQNOTE


#include "ofMain.h"
#include "Switch.h"
#include "loopedNote.h"
#include "DistinctControl.h"
#include "Control.h"
#include "ofxMidi.h"

class sequencerNote{
    public:
        void setup(DistControl * Scale,int * Transpose, int * CurSample, int Dwell, int X, int Y,
                   int * Chord, int * TotalSamples,int * NoteSamples,int * waitSamples, float * tvol,
                   float * nextphaseAdder, float * Volume, int * attackSamples,
                   float * AttackFactor,  control * ReleaseFactor, int * SampleRate,
                   float * ButtonDiameter, float R, float G, float B,int* Bright,
                   ofxMidiOut * midiout, int* note, int* oct,bool EagleOnOff);
        void update(ofPoint gaze);
		void update(ofPoint gaze,bool*sacadic);
		void releaseOnTimeAfterChordChange();
        void sendMidi(int volume);
        void releaseMidi();
		void releaseMidi(int midinote);
        void draw();
        void resized(int w, int h);
        float getSample();
        bool isActive();
        void setNumberOfNotes();
        Switch button;
        bool active;
        int midinote;
        int prnote;
        ofxMidiOut * StepMidiOut;
        float * buttonDiameter;
        int ix;//the positions of the buttons
        int iy;
		int pendingRelease;
		bool sounds;
		static int beat;
		static float beatDist;
		static int sequencer_midi;
    private:
        bool noteoff;
        int * note;
        int * oct;
        int * curSample;
        int * totalSamples;
        int * noteSamples;
        int * chord;
        int * transpose;
        int * sampleRate;
        DistControl * scale;
        loopedNote loopNote;
        int * waitSamples;
        float red;
        float green;
        float blue;
};



#endif
