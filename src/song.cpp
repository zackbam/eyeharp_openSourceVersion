#include "song.h"
#include "stepSequencer.h"
#include "EyeHarp.h"
#include <Disc.h>
void song::setup(int* Note, bool* Changed, bool* Flat) {
	
	note = Note;
	changed = Changed;
	flat = Flat;
	if(songFileNumber.value!=0)
		loadsong();
	pos = 0;
	
	ilumina = -1;
	/*nextPos = 1;
	iluminaNext = -1;*/
	/*for (int i = 0; i < 7; i++) {
		printf("%d\n", (scale+i)->value);
	}*/
	songFileNumber.setup("loadSong", ofPoint(-1.3f, 0.8f), ofPoint(-1.6f, 0.8f), 0, 100, 0, 1, 0.07, 1000, 0.8, 0.3, 0, 3);
	curDuration = 0;
	noteProgress = 0.f;
	centerRepeat = false;
	noteProgress = 1;
}

void song::loadsong() {
	melody.clear();
	if (songFileNumber.value == 0) {
		ilumina = -1;
		strcpy(songName, "empty");
	}
	else {
		string ssong = to_string(songFileNumber.value) + ".sng";
		input = fopen(ssong.c_str(), "r");
		cout << "Loading file: " << ssong.c_str() << endl;
		if (!input) {
			std::perror("File not found!\n");
			ilumina = -1;
			strcpy(songName, "empty");
		}
		else {
			fscanf(input,"%s %d", songName, &transpose);
			for (int i = 0; i < 7; i++) {
				fscanf(input, "%d", &(scale[i].value));
			}
			fscanf(input, "%d %d", &gridSize, &tempo);
			stepSequencer::numberOfNotes.value = gridSize;
			stepSequencer::numberOfNotes.changed = true;
			EyeHarp::tempo.value = tempo;
			EyeHarp::tempo.changed = true;
			while (!feof(input)) {
				NOTE temp; char flat = 'e';
				fscanf(input, "%d %d %c %f", &temp.octave, &temp.note, &flat, &temp.duration);
				if (flat != 'e') {
					temp.flat = flat == 'b' ? true : false;
					melody.push_back(temp);
				}
			}
			/*for (int i = 0; i < melody.size(); i++) {
				printf("%d %d %c\n", melody[i].octave, melody[i].note, melody[i].flat ? 'b' : '_');
			}*/
			Disc::chordONOFF.value = false;
			ilumina = melody[pos].octave * 7 + melody[pos].note - 5;
		}
	}
}

void song::update(ofPoint Gaze) {
	gaze = Gaze;
	if (!variables::alperMode)
		songFileNumber.update(gaze);
	if (songFileNumber.value != 0) {
		if (songFileNumber.changed) {
			loadsong();
			pos = 0;
		}
		if (melody.size() > 0 && *changed) {
			//iluminaNext = melody[nextPos].octave * 7 + melody[nextPos].note - 5;
			//printf("[%d]", ilumina);
			int octave, rnote;
			rnote = *note + 4;
			octave = rnote / 7;
			rnote = rnote % 7 + 1;
			
			if (octave == melody[pos].octave && rnote == melody[pos].note && *flat == melody[pos].flat) {
				bool prFlat = melody[pos].flat;
				prDuration = melody[pos].duration;
				pos = (pos + 1) % melody.size();
				//nextPos = (nextPos + 1) % melody.size();
				//printf("ilumina: %d, note: %d, flat: %d, octave: %d, rnote: %d\n", ilumina, *note, *flat, octave, rnote);
				int pr = ilumina;
				ilumina = melody[pos].octave * 7 + melody[pos].note - 5;
				curDuration = 0;
				//iluminaNext = melody[nextPos].octave * 7 + melody[nextPos].note - 5;
				if (pr == ilumina && prFlat == melody[pos].flat)
					centerRepeat = true;
				else
					centerRepeat = false;
			}
			
		}
	}
	else {
		ilumina = -1;
		strcpy(songName, "empty");
	}
}

void song::draw() {
	if (!variables::alperMode) {
		ofSetColor(variables::textColor);
		songFileNumber.draw();
		variables::myfont.drawString(songName, songFileNumber.posDW.x + 10, songFileNumber.posDW.y + 50);
	}
	/*if (ilumina != -1 && EyeHarp::showCircle) {
		ofEnableAlphaBlending();
		float quedaSamples = (float)stepSequencer::noteSamples * (float)prDuration - curDuration;
		if (quedaSamples < (float)SAMPLERATE * 0.6) {
			ofSetColor(255, 230);
		}
		else {
			ofSetColor(noteProgress * 200, 170);
		}
		ofCircle(gaze, height*0.076);
		ofDisableAlphaBlending();
	}*/
}

void song::resized(int w, int h) {
	songFileNumber.resized(w,h);
	height = h;
}

void song::sampleCounter() {
	if (ilumina!=-1) {
		if(curDuration<(float)stepSequencer::noteSamples * (float)prDuration)
			curDuration++;
		/*if(curDuration%100==0)
			cout<< curDuration<<":" << stepSequencer::noteSamples <<endl;*/
		noteProgress = (float)curDuration / (float)stepSequencer::noteSamples / (float)prDuration;
		/*if (noteProgress > 1)
			noteProgress = 1;*/
		/*if (noteProgress>=1) {
			bool prFlat = melody[pos].flat;
			pos = (pos + 1) % melody.size();
			nextPos = (nextPos + 1) % melody.size();
			int pr = ilumina;
			ilumina = melody[pos].octave * 7 + melody[pos].note - 5;
			iluminaNext = melody[nextPos].octave * 7 + melody[nextPos].note - 5;
			curDuration = 0;
			if (pr == ilumina && prFlat == melody[pos].flat)
				centerRepeat = true;
			else
				centerRepeat = false;
		}*/
	}
}
