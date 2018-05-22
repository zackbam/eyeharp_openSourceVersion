#ifndef _SONG
#define _SONG

#include "ofMain.h"
#include "DistinctControl.h"
//#include "ofApp.h"
typedef struct {
	int note;
	int octave;
	bool flat;
	float duration;
}NOTE;

class song {
public:
	void setup(int* note, bool* changed, bool* flat);
	void update(ofPoint Gaze);
	void draw();
	void resized(int w,int h);
	void sampleCounter();
	FILE* input;
	void loadsong();
	int transpose;
	char songName[50];
	int gridSize, tempo;
	DistControl* scale;
	DistControl songFileNumber;
	vector<NOTE> melody;
	bool* changed;
	int* note;
	bool* flat;
	int pos;
	
	bool centerRepeat;
	int ilumina;
	int iluminaNext, nextPos;
	int prDuration;
	unsigned int curDuration;
	float noteProgress;
	ofPoint gaze;
	int height;
};

#endif