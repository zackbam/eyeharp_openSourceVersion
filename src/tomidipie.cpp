#include "tomidipie.h"

void tomidipie::setup(ofxMidiOut * midiOut){
    EyeMidiOut=midiOut;
	midinote=-1;
//	mouseinNeutral=true;
	notevolume=0;
}

void tomidipie::update(int midinote,int noteVolume,bool release,int mousespeed,bool distVol,bool notesONOFF){
//    if(mouseinNeutral && !MouseinNeutral)
//        cout<<"In!";
	if(!distVol)	noteVolume=100;
	if(release)	EyeMidiOut->sendNoteOff(1, prmidinote, 0);
	if((mousespeed<100 || !distVol) && notesONOFF){
		if(release)	EyeMidiOut->sendNoteOff(1, prmidinote, 0);
		if(noteVolume<0) noteVolume=0;
		if(noteVolume>127) noteVolume=127;
//		if(!MouseinNeutral)
            EyeMidiOut->sendControlChange(1,7,noteVolume);
		if(midinote!=prmidinote /*|| (mouseinNeutral && !MouseinNeutral)*/){
			EyeMidiOut->sendNoteOff(1, prmidinote, 0);
		}

        if(!release)
            EyeMidiOut->sendNoteOn(1, midinote, noteVolume);
        prmidinote=midinote;
//		cout<<MouseinNeutral;
//		mouseinNeutral=MouseinNeutral;
	}
}
