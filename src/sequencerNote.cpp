#include "sequencerNote.h"

//int loopedNote::noteSamples;
int sequencerNote::beat=0;
float sequencerNote::beatDist=0;
int sequencerNote::sequencer_midi = 1;
void sequencerNote::setup(DistControl * Scale,int * Transpose,int * CurSample, int Dwell, int X,
                          int Y,int * Chord, int * TotalSamples, int* NoteSamples, int * WaitSamples,
                          float * tvol, float* nextPhaseAdder, float * Volume, int * AttackSamples,
                          float * AttackFactor, control * ReleaseFactor, int * SampleRate,
                          float * ButtonDiameter, float R, float G, float B,int* Bright,
                          ofxMidiOut * midiout, int * Note, int * Oct,bool EagleOnOFF){
    ix=X;
    iy=Y;
    scale=Scale;
    note=Note;
    oct=Oct;
    midinote=60+*oct*12+*note;
    ofPoint pos;
    scale=Scale;
    chord=Chord;
    transpose=Transpose;
    buttonDiameter=ButtonDiameter;
    pos.x = *buttonDiameter+ix* (*buttonDiameter*2)-1;
    pos.y = *buttonDiameter+iy* (*buttonDiameter*2)-1;
    curSample=CurSample;
    totalSamples=TotalSamples;
    noteSamples=NoteSamples;
    sampleRate=SampleRate;
    waitSamples=WaitSamples;
    loopNote.setup(CurSample,Transpose,midinote,WaitSamples, nextPhaseAdder, Volume,AttackSamples,AttackFactor,ReleaseFactor, SampleRate,Bright);
    red= R +(iy%7==0 || ix%4==0 || iy%7==4 )*R*2;
    green= G +(iy%7==0 || ix%4==0 || iy%7==4 )*G*2;
    blue= B +(iy%7==0 || ix%4==0 || iy%7==4 )*B*2;
    button.setup("",false,pos,*ButtonDiameter/2,Dwell,red,green,blue,EagleOnOFF);
    active=false;
    StepMidiOut=midiout;
    noteoff=true;
	pendingRelease=-1;
	sounds=false;
}


void sequencerNote::update(ofPoint gaze){
    button.update(gaze);
	/*if(button.changed)
		Switch::magActive=false;*/
	button.setColor(red,green,blue);
//    cout<<loopNote.curSample<<'\t';
    if(*curSample>*waitSamples && *curSample<(*waitSamples+*noteSamples)){
        button.setColor(2*red,2*green,2*blue);
    }
}

void sequencerNote::update(ofPoint gaze,bool*sacadic){
    button.update(gaze,sacadic);
	/*if(button.changed)
		Switch::magActive=false;*/
	if(!(*curSample>*waitSamples && *curSample<(*waitSamples+*noteSamples))){
		
		if(sounds==false){
			button.setColor(red,green,blue);
			sounds=true;
			
		}
    }
	else{
		//if(sounds){
		//	StepMidiOut->sendNoteOn(3,60+12*(ix==0),64);
		//	/*if(iy==0){
		//		int tempDist=ofGetFrameNum()-beat;
		//		beat=ofGetFrameNum();
		//		if(beatDist==0)
		//			beatDist=beat;
		//		else
		//			beatDist=beatDist*0.9+0.1*tempDist;
		//	}*/
		//}
		button.setColor(2*red,2*green,2*blue);
		sounds=false;
	}
}

void sequencerNote::releaseOnTimeAfterChordChange(){
	if(!(*curSample>*waitSamples && *curSample<(*waitSamples+*noteSamples))){
		if(sounds==false){
			sounds=true;
		}
    }
	else{
		//if(sounds){
		//	//StepMidiOut->sendNoteOn(3,60+12*(ix==0),64);
		//	/*if(iy==0){
		//		int tempDist=ofGetFrameNum()-beat;
		//		beat=ofGetFrameNum();
		//		if(beatDist==0)
		//			beatDist=beat;
		//		else
		//			beatDist=beatDist*0.9+0.1*tempDist;
		//	}*/
		//}
		sounds=false;
		if(pendingRelease!=-1){
			StepMidiOut->sendNoteOff(sequencer_midi, pendingRelease, 0);
			//cout<<pendingRelease;
			pendingRelease=-1;
		}
	}
}

void sequencerNote::sendMidi(int volume){
    prnote=midinote;
    midinote=60+*oct*12+*note+*transpose;
	
    if(*curSample>*waitSamples && *curSample<(*waitSamples+*noteSamples) ){
        if(noteoff && button.value){
            StepMidiOut->sendNoteOn(sequencer_midi, midinote,volume);
            noteoff=false;
        }
    }
    else{
        if(!noteoff && button.value){
            StepMidiOut->sendNoteOff(sequencer_midi, midinote, 0);
			if (prnote!=midinote){
				StepMidiOut->sendNoteOff(sequencer_midi, prnote, 0);
			}
            noteoff=true;
        }
    }
    if(!button.value && !noteoff){
        StepMidiOut->sendNoteOff(sequencer_midi, midinote, 0);
		if (prnote!=midinote){
			StepMidiOut->sendNoteOff(sequencer_midi, prnote, 0);
		}
        noteoff=true;
//        cout<<midinote<<'\t';
    }
}

void sequencerNote::releaseMidi(){
    midinote=60+*oct*12+*note+*transpose;
    if(active){
       StepMidiOut->sendNoteOff(sequencer_midi, midinote, 0);
	}
}

void sequencerNote::releaseMidi(int midi_note){
	pendingRelease=midi_note;
	
}

void sequencerNote::setNumberOfNotes(){
    ofPoint pos;
    pos.x = *buttonDiameter+ix* (*buttonDiameter*2)-1;
    pos.y = *buttonDiameter+iy* (*buttonDiameter*2)-1;
    button.updatePosSize(pos.x,pos.y,*buttonDiameter/2);
}


float sequencerNote::getSample(){
    return loopNote.getSample();
}

bool sequencerNote::isActive(){
    if(*curSample==*waitSamples){
        active=button.value;        //this way when you deactivate a note, it does not stop that moment. It will
                                    // just note be triggered at the next circle. It will fade out normally.
                                    //If the button.value was used for that immediately, then the reverb of the
                                    //note would stop suddently, causing a 'tick'.
    }
    return active;
}

void sequencerNote::draw(){
    button.draw();
	string temp=to_string(ix+1)+'/'+to_string(iy%7+1);
	if(button.active){
		ofSetColor(variables::textColor);
		variables::myfont.drawString(temp.c_str(),button.magPos.x,button.magPos.y);
	}
}

void sequencerNote::resized(int w,int h){
    button.resized(w,h);
}
