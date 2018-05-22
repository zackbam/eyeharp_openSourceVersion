#ifndef _DISC
#define _DISC


#include "ofMain.h"
#include "switch.h"
#include "DistinctControl.h"
#include "EyeHarpVARS.h"
#include "song.h"
#include <windows.h>
#include <fstream>
#include "song.h"
#define CHORDSNUM 7

class Disc{
    public:
        void setup(int NumOfNotes,float r,float g,float b,int * Chord, bool Advanced, bool* Conf,bool semitoneActive);
        void update(ofPoint Gaze, float * Velocity,bool *sacadic);
        void draw();
        void resized(int w, int h);
        void keyPressed(int Key);
        int note;//the note that the disc is sending.
		bool semi;
		bool prSemi;
		float REPLAYFADEOUT;
        float R,G,B;//the color of the disc. values from 0 to 1;
        int neutralRegion;//area where nothing happens inside the circle
		int inreleaseRegion;
        //int releaseRegion;//the release button in the middle
        Switch  notesONOFF;
        static Switch chordONOFF;
        DistControl NotesNumber;
		Switch replaySame;
        Switch percussive;
        Switch distVolume;
        int prDist;
        int dist;//distance from the centre
        bool changed;
		bool* conf;
        bool advanced;
		bool inRelease;
        Switch fixation;
        int * beat;
        int prbeat;
        int * noteFrames;
		int chordnum;
		bool replaySameActive;
		bool semitoneActive;
		float radius;
		Switch sharp45;
		int releaseDist;
		int releaseDistEnd;
		int semiActive;
		int* scale[7];
		int prNote;
		int FIXVEL;
		song testSong;
		float angle;//angle in the circle
		bool numberNote;
		bool fixationInNeutral;
		bool fixationOutside;
        ~Disc();
    private:
		int pointSize;
		bool inside;//did we last play they inside note?
        float tangle;//the angle between successive regions-notes(one region corresponds to one note)
        int colorstep;//how different are the colors of successive regions.
        int width;//window width
        int width2;
        int height;
        int height2;
        int outSpotStep;//distance of the outer Spots
        int inSpotDist;//distance of the inner Spots
        int * chord;
        int melody;
        int totalSamples;
//        ofstream myfile;
        bool pressed;
		ofPoint lgaze;
		float getMagVal(float);
		int SemiSize;
};

#endif
