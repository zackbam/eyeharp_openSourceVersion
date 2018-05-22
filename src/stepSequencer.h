#ifndef _STEPSEQ
#define _STEPSEQ


#include "ofMain.h"
#include "sequencerNote.h"
#include "loopedNote.h"
#include "control.h"
#include "DistinctControl.h"
#include "switch.h"
#include "XORswitch.h"
#include "slider.h"
#include "EyeHarpVARS.h"
#include <stack>

#include <fstream>

class stepSequencer{
    public:
        void setup(DistControl * Scale,int * Transpose, int NumberOfNotes,control *Tempo, int Dwell, float R, float G, float B, int * SampleRate, int * Chord,  ofxMidiOut * midinote,bool* conf);
        void update(ofPoint ,bool*sacadic,float*velocity);
        void draw();
        float getSample();

        void resized(int w,int h);
        void updatePitch();
        void updateTempo();
		void keyPressed(int);
		bool outsideMagRegion(ofPoint gaze,ofPoint lastEagle);
        static int curSample;
        int totalSamples;
        static DistControl numberOfNotes;
        void clearAll();
        static int noteSamples;
        int noteFrames;
		bool* conf;
        int beat;
        void sendMidi();
        void releaseAllMidi();
        control targetVolume;
        slider targetVolumeSlider;
		int numAccessX; //to access with the num keyboard;
		int numAccessY;
		bool numAccess;
		bool sacadicDistance(ofPoint gaze,float*velocity,bool sacadic);
		bool magDynamic;
		float sacdist;
		static int timeThre;
		ofPoint Gaze;
		ofPoint lastEagle;
		int timer;
		Switch Eagle;
		bool outSideLense;
		ofPoint lastFix;
        sequencerNote ** seqNote;
		struct gridElement { int x=0, y=0; };
		std::stack<gridElement> arpeggioStack;
		Switch monophonic;
		//void eagleEye(ofPoint gaze);
//        ~stepSequencer();
    private:
		/*void computeMagThr();*/
		int width;
		int height;
		bool numPressed; //if a note is selected with the num keys
        xorSwitch controlMultiplex;
        xorSwitch brightness;
        int prChord;
        int localOctave;
        Switch deleteAll;
        int *waitSamples;
        float *tvol;//temporary volume for each column.
        float * nextPhaseAdder;
        int * rownote;
        int * rowoct;
        control releaseFactor;
        control * tempo;
        DistControl octave;
		Switch undo; //deleteLast on the Stack
        int maxNotes;
        int * chord;
        float volume;

        float attackFactor;
        int attackSamples;
        int * sampleRate;
        DistControl * scale;
        float buttonDiameter;
        int dwell;
        float RED;//0 to 1
        float GREEN;
        float BLUE;
        int * transpose;
        slider numberOfNotesSlider;
        slider releaseFactorSlider;
        slider octaveSlider;

//        ofstream myfile;
};


#endif
