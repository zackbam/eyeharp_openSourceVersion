#pragma once
//#include <iostream>
#include <vector>
#include "ofMain.h"
#include "stepSequencer.h"
#include "switch.h"
enum { PAUSE, RECORD, PLAY };

struct record {
	int chord;
	int sampleCount;
	int bar;
};

class recordChords {
public:
	std::vector<record> loop;
	void setup(stepSequencer *, int* Chord, bool* showChords,bool*chordChanged);
	void update(ofPoint);
	void draw();
	void resized(int, int);
	void getPos();
	void addChord(int Chord);
	void keyPressed(int Key);
	void printVector();
	int state; //0: pause, 1: record, 2: play
	int curPos;
	int barCount;
	int pos; //playback position
	stepSequencer* stepSeq;
	int *chord;
	bool* showChords;
	bool* cchanged;
	bool showChordsBackup;
	ofPoint gaze;
	Switch button;
	record temp;
};