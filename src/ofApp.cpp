
#include "ofApp.h"
#include "stdio.h"
#include "EyeHarpVARS.h"
//global variables .......
bool variables::alperMode, variables::record_chords, variables::triggerChords, variables::alperConfigureActive;
int variables::firstNote, variables::notesPerScale, variables::harmonize,variables::textColor, variables::windowH, variables::windowW, variables::inReleaseFIXVEL;
int variables::trNotesMidi[3];
float variables::framerate;
ofTrueTypeFont variables::myfont, variables::myfontPie;
vector<sscale> variables::presetScales;

//--------------------------------------------------------------
ofApp::ofApp(){

}

ofApp::~ofApp(){
	fclose(record);
	myTobii.~tobii();
	tribe.~MyGaze();
	HARP.~EyeHarp(); 
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetCircleResolution(40);
	FILE *initParam;
	help = true;
	initParam = fopen("eyeharp.txt", "r");
	char paramName[31];
	float minVol;
	int loadSong = 0, discNotesNumber = 13, stepSequencerNotesNumber = 6, bufferSize = 512, transpose = 0, FIXVEL = 70, loopBeLoopMIDI = 0;
	bool replaySame=true,Breath=false,cc1 = 0, cc2 = 0, cc7 = 1, cc11 = 0,afterTouch=0, inRelease = false, semitoneActive = false, showScale = true, scalePreset = true,  chordsONOFF = false, mouseEyetribeInput = false, clickDwell = false, tomidi = false, fullscreen = false, monophonic = true, showGaze = true;
	int temp;
	variables::alperMode = false;
	for(int i=0;i<3;i++)	variables::trNotesMidi[i] = -1;
	fixationSamples = 4;
	HARP.eye.disc.REPLAYFADEOUT = 0.5;
	HARP.eye.disc.numberNote = true;
	sscale tempScale;
	variables::notesPerScale = 7;
	variables::framerate = 30.f;
	variables::inReleaseFIXVEL = 1000;
	variables::myfont.loadFont("Xoxoxa.ttf", ofGetScreenHeight()*0.02f);
	variables::myfontPie.loadFont("Xoxoxa.ttf", ofGetScreenHeight()*0.017f);
	variables::firstNote = 4;
	variables::record_chords = 0;
	variables::harmonize = 0;
	variables::textColor = 180;
	variables::windowH = 450;
	variables::windowW = 800;

	if (initParam == NULL)
		cout << "No eyeharp.txt file found\n";
	else {
		while (fscanf(initParam, "%s %d", paramName, &temp) != EOF) {
			//printf("%s, %d\n", paramName, temp);
			if (strcmp(paramName, "discNotesNumber") == 0)
				discNotesNumber = temp;
			else if (strcmp(paramName, "stepSequencerNotesNumber") == 0)
				stepSequencerNotesNumber = temp;
			else if (strcmp(paramName, "bufferSize") == 0)
				bufferSize = temp;
			else if (strcmp(paramName, "chords") == 0)
				chordsONOFF = (bool)temp;
			else if (strcmp(paramName, "showScale") == 0)
				showScale = (bool)temp;
			else if (strcmp(paramName, "scalePreset") == 0)
				scalePreset = (bool)temp;
			else if (strcmp(paramName, "mouseEyeTracker") == 0)
				mouseEyetribeInput = (bool)temp;
			else if (strcmp(paramName, "clickDwell") == 0)
				clickDwell = (bool)temp;
			else if (strcmp(paramName, "help") == 0)
				help = (bool)temp;
			else if (strcmp(paramName, "tomidi") == 0)
				tomidi = (bool)temp;
			else if (strcmp(paramName, "fullscreen") == 0) {
				fullscreen = (bool)temp;
				if (fullscreen)
					ofToggleFullscreen();
			}
			else if (strcmp(paramName, "fixationSamples") == 0)
				fixationSamples = temp;
			else if (strcmp(paramName, "monophonicStep") == 0)
				monophonic = temp;
			else if (strcmp(paramName, "showGaze") == 0)
				showGaze = temp;
			else if (strcmp(paramName, "LoopBe_loopMIDI_other") == 0)
				loopBeLoopMIDI = temp;
			else if (strcmp(paramName, "semitoneActive") == 0)
				semitoneActive = temp;
			else if (strcmp(paramName, "transpose") == 0)
				transpose = temp;
			else if (strcmp(paramName, "replaySame") == 0)
				replaySame = temp;
			else if (strcmp(paramName, "inRelease") == 0) {
				if (temp > 0) {
					inRelease = true;
					variables::inReleaseFIXVEL = ofGetScreenWidth()*(float)temp / 100.f;
					//cout << variables::inReleaseFIXVEL << endl;
				}
				else
					inRelease = false;

			}
			else if (strcmp(paramName, "cc1") == 0)
				cc1 = temp;
			else if (strcmp(paramName, "cc2") == 0)
				cc2 = temp;
			else if (strcmp(paramName, "cc7") == 0)
				cc7 = temp;
			else if (strcmp(paramName, "cc11") == 0)
				cc11 = temp;
			else if (strcmp(paramName, "afterTouch") == 0)
				afterTouch = temp;
			else if (strcmp(paramName, "MINVOL") == 0)
				minVol = (float)temp / 100.0;
			else if (strcmp(paramName, "FIXVEL") == 0)
				FIXVEL = ofGetScreenWidth()*(float)temp/100.f;
			else if (strcmp(paramName, "BreathSensor") == 0)
				Breath = temp;
			else if (strcmp(paramName, "loadSong") == 0)
				loadSong = temp;
			else if (strcmp(paramName, "replayFadeOut") == 0)
				HARP.eye.disc.REPLAYFADEOUT = (float)temp / 1000.f;
			else if (strcmp(paramName, "numberNote") == 0)
				HARP.eye.disc.numberNote = temp;
			
			
			else if (strcmp(paramName, "firstNote") == 0)
				variables::firstNote = temp;
			else if (strcmp(paramName, "recordChords") == 0)
				variables::record_chords = temp;
			else if (strcmp(paramName, "triggerChords") == 0)
				variables::triggerChords = temp;
			else if (strcmp(paramName, "alperMode") == 0)
				variables::alperMode = temp;
			else if (strcmp(paramName, "framerate") == 0)
				variables::framerate = temp;
			
			else if (strcmp(paramName, "scale") == 0)
			{
				tempScale.scaleNotes[0] = temp;
				char nl = ' ';
				int i = 1;
				while (nl == ' ') {
					fscanf(initParam, "%d%c", &tempScale.scaleNotes[i++], &nl);
				}
				tempScale.size = i;
				fscanf(initParam,"%s", tempScale.name);
				variables::presetScales.push_back(tempScale);
			}
		}
		fclose(initParam);
	}
	/*for (int i = 0; i < variables::presetScales.size(); i++)
		printf("%s %d %d %d %d %d %d %d\n", variables::presetScales[i].name, variables::presetScales[i].scaleNotes[0], variables::presetScales[i].scaleNotes[1], variables::presetScales[i].scaleNotes[2], variables::presetScales[i].scaleNotes[3], variables::presetScales[i].scaleNotes[4], variables::presetScales[i].scaleNotes[5], variables::presetScales[i].scaleNotes[6]);
	*///printf("cc1 %d\ncc2 %d\ncc7 %d\ncc11 %d\ncafterTouch %d\n", cc1, cc2, cc7, cc11, afterTouch);
	gbuffer = new ofPoint[fixationSamples];
	Switch::click = !clickDwell;
	//printf("discNotesNumber: %d\nstepSequencerNotesNumber: %d\nbufferSize: %d\nchordsONOFF: %d\nshowScale: %d\nmouseEyetribeInput: %d\nclickDwell: %d\nfullscreen: %d", discNotesNumber, stepSequencerNotesNumber, bufferSize, chordsONOFF, showScale, mouseEyetribeInput, clickDwell,fullscreen);
//    glutSetCursor(GLUT_CURSOR_CROSSHAIR); 
    //glutSetCursor(GLUT_CURSOR_NONE);
	ofBackground(50,60,30);
	//---- setup standard application settings
	ofSetVerticalSync(false);
    ofSetFrameRate(variables::framerate);
	//ofHideCursor();
	//ofToggleFullscreen();
	gaze= mouseEyetribeInput;
	HARP.setup(discNotesNumber, stepSequencerNotesNumber, chordsONOFF, showScale,scalePreset, clickDwell,tomidi, loopBeLoopMIDI, semitoneActive, transpose,Breath);
	
	HARP.showCircle = showGaze;
	/*if (showGaze)
		ofHideCursor();*/
	HARP.stepSeq.monophonic.setup("monophonic", monophonic, ofPoint(-1.2, 0.8), .095, 800, .8, .4, 0, false);
	tribe.setup();
	myTobii.setup();
	HARP.eye.disc.inRelease = inRelease;
	printf("Sound Devices:\n");
	//ofSoundStreamListDevices();
	soundstream.printDeviceList();
	soundstream.setup(this, 2, 0, SAMPLERATE, bufferSize, 4);
	
	mouseDwell=0;
	printf("\nPress Esc to exit when the EyeHarp window is active or Ctrl+c in the current terminal\n");
	//showCircle=true;
//	for(float i=-5;i<6;i+=0.1)   cout<<i<<'\t'<<floor(i)<<'\n';
	
	smooth.x=0;
	smooth.y=0;
	bpos=0;
	tracker=MOUSE;
	sacadic=true;
	read=NULL;
	record=NULL;
	firstRead=false;
	/*FILE* conf;
	conf=fopen("configuration.txt", "r");
	float screenx, screeny;
	fscanf(conf,"%f %f", &screenx, &screeny);
	ratiox = ofGetScreenWidth() / screenx;
	ratioy = ofGetScreenHeight() / screeny;
	fclose(conf);*/
	prFixation = false;
	HARP.cc1 = cc1; HARP.cc2 = cc2; HARP.cc7 = cc7; HARP.cc11 = cc11; HARP.afterTouch = afterTouch, HARP.MINVOL = minVol;

	HARP.eye.disc.FIXVEL = FIXVEL;
	HARP.eye.disc.testSong.songFileNumber.value = loadSong;
	if (loadSong != 0) {
		HARP.eye.disc.testSong.loadsong();
	}
	HARP.eye.disc.replaySame.value = replaySame;
	HARP.eye.disc.replaySameActive = replaySame;
}
void ofApp::exit() {
	HARP.exitApp();
}
//--------------------------------------------------------------
void ofApp::update(){
	if(read!=NULL && !feof(read)){
		int temp;
		fscanf(read,"%f,%f,%d,%d\n",&eyeSmoothed.x,&eyeSmoothed.y,&sacadic,&temp);
		if(firstRead){
			firstRead=false;
			HARP.stepSeq.curSample=temp;
		}
	}
	else{
		if (gaze) {
			if (tribe.m_api.is_connected()) {
				tracker = EYETRIBE;
				tribe.m_api.get_frame(gdata);
				//if (gdata.raw.x > ofGetWindowPositionX() && gdata.raw.x<ofGetWindowPositionX() + ofGetWidth() && gdata.raw.y>ofGetWindowPositionY() && gdata.raw.y < ofGetWindowPositionY() + ofGetHeight()) {
				
				if (gdata.state == 7){
					raw.x = gdata.raw.x/**ratiox*/;
					raw.y = gdata.raw.y/**ratioy*/;
				}

				mySmooth();
				int X = smooth.x - ofGetWindowPositionX();
				int Y = smooth.y - ofGetWindowPositionY();
				eyeSmoothed = ofPoint(X,Y);
				eyeSmoothed = ofPoint(smooth.x - ofGetWindowPositionX(), smooth.y - ofGetWindowPositionY());

			}
			else if (myTobii.success) {
				if (myTobii.eventParams.Timestamp != 0) {
					/*cout << myTobii.eventParams.Timestamp << endl;
					*/
					
					tracker = TOBII;
					/*if(HARP.eye.disc.FIXVEL<9999)
						HARP.eye.disc.FIXVEL = 9999999;*/
					//SetCursorPos(myTobii.eventParams.X, myTobii.eventParams.Y);
					if (myTobii.eventParams.X > ofGetWindowPositionX() && myTobii.eventParams.X<ofGetWindowPositionX() + ofGetWidth() && myTobii.eventParams.Y>ofGetWindowPositionY() && myTobii.eventParams.Y < ofGetWindowPositionY() + ofGetHeight()) {
						raw.x = myTobii.eventParams.X;
						raw.y = myTobii.eventParams.Y;
					}
					mySmooth();
					//eyeSmoothed = ofPoint(raw.x - ofGetWindowPositionX(), smooth.y - ofGetWindowPositionY());
					eyeSmoothed = ofPoint(smooth.x - ofGetWindowPositionX(), smooth.y - ofGetWindowPositionY());
					//SetCursorPos(smooth.x, smooth.y);
				}
				else
				{
					sacadic = true;
					eyeSmoothed = ofPoint(mousex, mousey);
				}
			}
			else {
				printf("Did not connect to any Eye-Tracker\n");
				sacadic = true;
				eyeSmoothed = ofPoint(mousex, mousey);
			}
		}
		else{
			sacadic=true;
			eyeSmoothed=ofPoint(mousex,mousey);
		}
		if(record!=NULL)
			fprintf(record,"%f,%f,%d,%d\n",eyeSmoothed.x,eyeSmoothed.y,sacadic,HARP.stepSeq.curSample);
	}
	sacadic = true;
	HARP.update(eyeSmoothed,&sacadic);	
}


void ofApp::mySmooth(){
	//cout<<gdata.state<<'\n';
	if(tracker==TOBII || (tracker==EYETRIBE && gdata.state==7)){
		bool fix = fixation(); 
		if (fix && !prFixation) {
			smooth.x = avgNew.x;
			smooth.y = avgNew.y;
		}
		else{
			/*smooth.x=smooth.x*SM+raw.x*(1-SM);
			smooth.y=smooth.y*SM+raw.y*(1-SM);*/
			smooth = lastFixation;
		}
		prFixation = fix;
	}
}

bool ofApp::fixation() {
	gbuffer[bpos].x = raw.x;
	gbuffer[bpos].y = raw.y;
	bool fix;
	int i;
	avgNew.x = 0; avgNew.y = 0;
	int prpos = bpos - 1;
	if (prpos < 0)
		prpos += fixationSamples ;
	for (i = 0; i <fixationSamples  ; i ++) {
		avgNew.x += gbuffer[i].x;
		avgNew.y += gbuffer[i].y;
	}
	avgNew.x /= (float)fixationSamples ;
	avgNew.y /= (float)fixationSamples ;
	fix = true;
	for (i = 0; i <fixationSamples ; i++) {
		if (ofDist(gbuffer[i].x, gbuffer[i].y, avgNew.x, avgNew.y) > STH * ofGetScreenHeight()) {
			fix=false;
		}
	}
	if (fix) {
		lastFixation = avgNew;
	}
	bpos = (bpos + 1) % fixationSamples ;
	return fix;
}


//--------------------------------------------------------------
void ofApp::draw(){


	//ofSetColor(255, 255, 255);
    HARP.draw();
	/*
	string temp="Mag:"+to_string(Switch::Mag)+"\nMagFac:"+to_string(Switch::MagFac)
		+"\nMagRegion:"+to_string(Switch::MagRegion)+"\nDwellZoom:"+to_string(stepSequencer::timeThre)
		+"\nMagDynamic:"+to_string(HARP.stepSeq.magDynamic);
	ofSetColor(255);
	variables::myfont.drawString(temp,ofGetWidth()*0.9,140);*/
	if(help && HARP.configure.value==false && HARP.layer.value==0){
		string temp="Press 'k' to switch between dwell time\nand mouse click selection method.\nPress 'f' to toggle fullscreen.\nPress 'm' to switch between mouse and gaze input.\nPress 'n' to show/hide the gaze pointer.\nPress 'h' to hide/show keyboard shortcuts.\nPress 'Esc' to exit.";
		ofSetColor(255);
		variables::myfont.drawString(temp,ofGetWidth()*0.67,10);
	}
	//variables::myfont.drawString(ofToString(ofGetFrameRate()), 300, 350);
	/*string temp = to_string(mousex) + ',' + to_string(mousey);
	variables::myfont.drawString(temp,ofPoint(mousex-100,mousey));*/
	/*string temp = to_string(gdata.state);
	variables::myfont.drawString(temp, ofPoint(ofGetWidth()/2,ofGetHeight()/2)); */
}

void ofApp::audioRequested 	(float * output, int bufferSize, int nChannels){
	//cout << HARP.stepSeq.curSample << endl;
        HARP.audioRequested 	(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key){
		case	'f':
		case	'F':
			ofToggleFullscreen();
			HARP.resized(variables::windowW, variables::windowH);
			break;
        case '+':
            HARP.layer.value=!HARP.layer.value;
            break;
		case 'm':
		case 'M':
			gaze=!gaze;
			break;
		case 'n':
		case 'N':
			HARP.showCircle=!HARP.showCircle;
			/*if (HARP.showCircle)
				ofHideCursor();
			else
				ofShowCursor();*/
			break;
		/*case 'x':
		case 'X':
			Switch::focuspoints=!Switch::focuspoints;
			break;*/
		/*case 'G':
		case 'g':
			if(record==NULL){
				string filename=ofGetTimestampString()+".csv";
				record=fopen(filename.c_str(),"w");
			}
			else
				fclose(record);
			break;*/
		case 'H':
		case 'h':
			help=!help;
			break;
			/*if(read==NULL){
				read=fopen("performance.csv","r");
			}
			else
				fclose(read);
			break;*/
		case 'K':
		case 'k':
			Switch::click=!Switch::click;
			break;
		case 'z':
		case 'Z':
			Switch::Mag-=0.1;
			break;
		case 'a':
		case 'A':
			Switch::Mag+=0.1;
			break;
		case 's':
		case 'S':
			Switch::MagFac+=0.1;
			break;
		case 'x':
		case 'X':
			Switch::MagFac-=0.1;
			break;
		case 'c':
		case 'C':
			Switch::MagRegion-=10;
			break;
		case 'd':
		case 'D':
			Switch::MagRegion+=10;
			break;
		case 'v':
		case 'V':
			stepSequencer::timeThre-=10;
			break;
		case 'b':
		case 'B':
			stepSequencer::timeThre+=10;
			break;
		case 'j':
		case 'J':
			HARP.stepSeq.magDynamic=!HARP.stepSeq.magDynamic;
			break;
		//case 'l':
		//case 'L':
		//	//printf("%d, %d, %f, %f\n", ofGetScreenWidth(), ofGetScreenHeight(), eyeSmoothed.x, eyeSmoothed.y);
		//	printf("%d, %d, %f, %f\n", ofGetScreenWidth(), ofGetScreenHeight(), ofGetWindowWidth(), ofGetWindowHeight());
		//	//HARP.eye.disc.replaySameActive=!HARP.eye.disc.replaySameActive;
		//	//Switch::lenseSlave=!Switch::lenseSlave;

	}
	HARP.keyPressed(key);
	
}




//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    mousex=x;
    mousey=y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	mousex = x;
	mousey = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//printf("xRaw: %f   yRaw: %f  Smooth: %f, %f\n, window: %d, %d\n", gdata.raw.x,  gdata.raw.y, eyeSmoothed.x, eyeSmoothed.y , ofGetWindowPositionX(), ofGetWindowPositionY());
	Switch::pressed=true;
	Switch::pressedPos = eyeSmoothed;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	if (w < 1.777777778*h) {
		variables::windowW = w;
		variables::windowH = 0.5625*w;
	}
	else {
		variables::windowW = 1.777777778*h;
		variables::windowH = h;
	}
	HARP.resized(variables::windowW, variables::windowH);
}


