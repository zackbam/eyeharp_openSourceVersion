#include "recordChords.h"

void recordChords::setup(stepSequencer* StepSeq, int*Chord, bool *ShowChords, bool *Cchange) {
	stepSeq = StepSeq;
	barCount = 0;
	curPos = 0;
	state = 0;
	pos = 0;
	chord = Chord;
	showChords = ShowChords;
	button.setup("PRESS TO RECORD", false, ofPoint(1.55, 0.8), 0.1, 1000, 0, 0, 0, false);
	cchanged = Cchange;
}

void recordChords::update(ofPoint gaze) {
	if (button.changed) {
		state++;
		if (state == 3)	state = 0;
		//cout << state<< endl;
		switch (state) {
		case PAUSE:
			button.setColor(0, 0, 0);
			button.value = true;
			strcpy(button.name, "PRESS TO RECORD");
			*showChords = showChordsBackup;
			break;
		case RECORD:
			button.value = true;
			strcpy(button.name, "RECORDING...\nPRESS TO STOP");
			loop.clear();
			barCount = -1; 
			temp.bar = 0;
			temp.chord = *chord;
			//loop.push_back(temp);
			showChordsBackup = *showChords;
			*showChords = true;
			break;
		case PLAY:
			button.value = true;
			button.setColor(0, 0, 1);
			temp.bar = barCount;
			temp.sampleCount = stepSeq->totalSamples - 2;
			loop.push_back(temp); // add the first one at the end!
			strcpy(button.name, "PLAYING...\nPRESS TO STOP");
			/*if (loop[loop.size() - 1].sampleCount > stepSeq->noteSamples*(stepSeq->numberOfNotes.value - 2)) {
				record temp;
				temp.bar = loop[loop.size() - 1].bar+1;
				temp.chord = *chord;
				temp.sampleCount = 0;
				loop.push_back(temp);
			}*/
			//printVector();
			pos = loop.size()-1;
			barCount = loop[pos].bar; 
			*showChords = false;
			break;
		}
	}
	if (state == PLAY) {
		//cout << ".";
		//printf("%d: %d, %d\n", pos, barCount, loop[pos].bar);
		if (barCount * stepSeq->totalSamples + stepSeq->curSample >= loop[pos].bar * stepSeq->totalSamples + loop[pos].sampleCount) {
			*chord = loop[pos].chord;
			*cchanged = true;
			stepSeq->updatePitch();
			//printf("pos: %d, size:%d\n", pos, loop.size());
			pos++;
			if (pos == loop.size()) {
				pos = 0;
				barCount = 0;
			}
		}
	}
	else if (state == RECORD && barCount >= 0)
		button.setColor(1, 0, 0);
}

void recordChords::getPos() {
	if (stepSeq->curSample == stepSeq->totalSamples-1) {
		barCount++;
		//printf("%d\t%d\n", barCount,*curSample / *noteSamples);
	}
}

void recordChords::addChord(int Chord) {
	if (state == RECORD && barCount >= 0) {
		record temp;
		temp.bar = barCount;
		temp.chord = Chord;
		temp.sampleCount = stepSeq->curSample;
		loop.push_back(temp);
	}
}

void recordChords::printVector() {
	for (auto i = 0; i < loop.size(); ++i)
		printf("%d\t%d\t%d\n", loop[i].bar, loop[i].sampleCount, loop[i].chord);
}

void recordChords::keyPressed(int Key) {
	if(Key=='$')
		printVector();
}

void recordChords::resized(int W, int H) {
	button.resized(W, H);
}


void recordChords::draw() {
	button.draw();
}