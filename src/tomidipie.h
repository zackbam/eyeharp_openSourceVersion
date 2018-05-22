#ifndef _TOMIDIPIE
#define _TOMIDIPIE

#include "ofMain.h"
#include "ofxMidi.h"

class tomidipie{
public:
	void setup(ofxMidiOut * midiOut);
	void update(int Midinote,int NoteVolume,bool release,int mousespeed,bool distVol,bool noteONOFF);
	ofxMidiOut * EyeMidiOut;
private:
	int midinote;
//	bool mouseinNeutral;
	int notevolume;
	int prmidinote;
};

#endif
