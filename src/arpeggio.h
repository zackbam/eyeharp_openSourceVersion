#ifndef _ARPEGGIOSIMPLE
#define _ARPEGGIOSIMPLE


#include "ofMain.h"
#include "DistinctControl.h"
#include "EyeHarpVARS.h"

class arpeggio{
    public:
        void setup(DistControl * Scale,int * Transpose,int * Chord,int * TotalSamples,int* CurSample, int* SampleRate,
float Volume,int GlobalStep, int PatternStep,int StartingNote,int PatternSize, int Meter, int NotesIncl);
        void update();
        float getSample();
        bool changed;
        void setMeter(int Meter);
        void setGlobalStep(int Step);
        void setPatternStep(int Step);
        void setStartingNote(int Note);
        void setPatternSize(int Size);
        void setVolume(float Vol);
        void setNotesIncl(int num);
        int getMeter();
        int getGlobalStep();
        int getPatternStep();
        int getStartingNote();
        int getPatternSize();
        float getVolume();
        int getNotesIncl();

        void updateTempo();
    private:
        void computePhases();
        void generateArpeggio();
        DistControl* scale;
        int * chord;

        int patternStep; //0: moving up and then down, 1:moving up, 2: moving down, 3: moving down and up,
                        //: 4:random.
        int * sequence;
        float * targetFrequency;
		float * phaseAdder;
		float * nextPhaseAdder;

		int meter;  //how many beats
        int globalStep;
        int startingNote; //starting note
        float volume;

        int* totalSamples;
        void computeNotesIncl();
        void computeChord();//circle shifts the notesIncluded array chord positions rigth

        int curNote;
        int minGlobalStep;
        int maxGlobalStep;
        int minPatternStep;
        int maxPatternStep;
        int minStartingNote;
        int maxStartingNote;
        float minVolume;
        float maxVolume;
        int minMeter;
        int maxMeter;
        int minPatternSize;
        int maxPatternSize;
        int minNotesIncl;
        int maxNotesIncl;

        int nextArpeggioNote(int,int);
        int *sampleRate;

        int noteSamples;//the number of samples for each note
		float 	phase;
		float  tvol;//the current volume of each frame
		float attackSamples;
		bool ChordNotesIncluded[7];//the shifted array according to the chord
		bool notesIncluded[7];
		int notesInclOrder[7];
		int notesIncl;//number if notes included
		int patternSize;
		int * curSample;
		int prChord;
		int * transpose;
};


#endif
