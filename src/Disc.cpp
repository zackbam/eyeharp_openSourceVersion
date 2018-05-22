#include "Disc.h"
#include <string>
#include <stepSequencer.h>
#include <EyeHarpVARS.h>
//#include <ofApp.h>
#define NR 0.29
#define RELEASE_DIST 1
#define RELEASE_DIST_END 1.2/*
#define SEMI_SIZE 170*/
Switch Disc::chordONOFF;

void Disc::setup(int NumOfNotes,float red,float green,float blue,int * Chord, bool Advanced,bool* Conf, bool SemitoneActive){
	if(variables::alperMode)
		NotesNumber.setup("NotesNum", ofPoint(-1.28, 0.55), ofPoint(-1.62, 0.55),7,36,NumOfNotes,1, 0.07, 800, 0.6f, 0.2f, 0.0f);
	else
		NotesNumber.setup("NotesNum", ofPoint(-1.28, 0.5) , ofPoint(-1.62, 0.5), 7, 36, NumOfNotes, 1, 0.085, 800, 0.6f, 0.2f, 0.0f);
    chord=Chord;
	conf=Conf;
	chordnum=CHORDSNUM;
    advanced=Advanced;
    distVolume.setup("Dist\nVolume",true, ofPoint(-1.0f,-0.8f),0.1,1000,0.6,0.2,0.1,false);
    fixation.setup("Fixation",true,ofPoint(-0.8f,0.85f),0.05,1000,0.6,0.2,0.1,false);
    note=-1;
	semi = false;
    melody=note;
    prNote=note;
    tangle=TWO_PI/NotesNumber.value;
    colorstep=255/NotesNumber.value;
    R=red;G=green; B=blue;
    if(distVolume.value)
        neutralRegion=ofGetHeight()*NR;
    else
        neutralRegion=ofGetHeight()*NR;
    //releaseRegion=ofGetHeight()*0.11;
	inreleaseRegion = 0.85*neutralRegion;
    dist=0;
    angle=0.0;
    outSpotStep=(ofGetHeight()/2-neutralRegion)/5;
    inSpotDist=0.6*neutralRegion;
	if(variables::alperMode)	
		chordONOFF.setup("Chords", *chord, ofPoint(1.62, -0.15), 0.07, 1000, 0.6, 0.2, 0.1, false);
	else
		chordONOFF.setup("Chords", *chord, ofPoint(-1, -0.8), 0.1, 1000, 0.6, 0.2, 0.1, false);
    percussive.setup("Percu\nssive",false, ofPoint(-1.2,0.85),0.05,1000,0.6,0.2,0.1,false);
	replaySame.setup("",false,ofPoint(0,0),0.095,10,0,0,0,false);
    changed=false;
	pointSize=height*0.005;
//    char filename[50];

//    time_t rawtime;struct tm * timeinfo;time ( &rawtime );timeinfo = localtime ( &rawtime );
//    sprintf(filename,"./data/logDisc%d_%d_%d.txt",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
//    myfile.open(filename,ios::trunc);
//    if (myfile.is_open())
//    {
//        cout<<"File opened\n";
//
//    }
//    else cout << "Unable to open file";
    prbeat=1;
	replaySameActive=true;
	//sharp45.setup("4# 5#", false, ofPoint(-1.0f, -0.8f), 0.1, 100, 0.6f, 0.2f, 0.0f, false);
	releaseDist = height2*RELEASE_DIST;
	releaseDistEnd = releaseDist*RELEASE_DIST_END;
	pressed = false;
	notesONOFF.setup("ON/OFF", true, ofPoint(-1.5, -0.75), 0.1f, 800, 0.6f, 0.2f, 0.0f, false);
	semiActive = -1;
	SemiSize = height2*0.35;
	semitoneActive = SemitoneActive;
	inRelease = false;
	testSong.setup(&note, &changed, &semi);
	fixationOutside = true;
}


Disc::~Disc(){
//    time_t rawtime;
//    time ( &rawtime );
//    myfile<<"\nClosed on "<<asctime(localtime ( &rawtime ));
//    myfile.close();
//    while (myfile.is_open()){
//        cout<<"file NOT closed\n";
//        myfile.close();
//    }
//    cout<<"file closed\n";
}

void Disc::update(ofPoint gaze, float* velocity,bool *sacadic){
	//sharp45.update(gaze);
    if(pressed){
        pressed=false;
    }
    else{
		//if(*conf){
		//	//distVolume.update(gaze);
		//	if(distVolume.changed){
		//		if(distVolume.value)
		//			neutralRegion=ofGetHeight()*NR;
		//		else
		//			neutralRegion=ofGetHeight()*NR;
		//		/*outSpotStep=(ofGetHeight()/2-neutralRegion)/5;
		//		inSpotDist=0.3*neutralRegion;*/
		//	}
		//}
		changed=false;
		tangle=TWO_PI/NotesNumber.value;
		colorstep=255/NotesNumber.value;
		angle=atan2(-gaze.y+height2,gaze.x-width2)+TWO_PI/2;
		//int prDist=dist;
		dist=ofDist(gaze.x,gaze.y,width2,height2);
		if(variables::alperMode && !variables::alperConfigureActive)
			notesONOFF.update(gaze);
		if(*conf/*|| variables::alperMode*/)
			chordONOFF.update(gaze);
		if(*velocity < FIXVEL && dist < neutralRegion){
			fixationInNeutral=true;
		}
		//semiActive = -1;
		if (*velocity < FIXVEL && semitoneActive && variables::notesPerScale == 7) {
			semiActive = -1;
			for (int j = 0; j < NotesNumber.value - 7*chordONOFF.value; j++) {
				ofPoint curPosSemitone = ofPoint(width2 - cos((j)*tangle)*radius, height2 + sin((j)*tangle)*radius);

				int ii = (j + variables::firstNote) % variables::notesPerScale;
				//if ((ii == 2 || ii == 3 || ii == 5 || ii == 6 || ii == 0) && j != 0) {
				int tempDif = (*scale[ii] - *scale[(ii + 6) % variables::notesPerScale]);
				if ((tempDif> 1 || (tempDif < 0 && tempDif>-11)) && j != 0) {
					if (ofDist(gaze.x, gaze.y, curPosSemitone.x, curPosSemitone.y) < tangle*SemiSize /*&& *velocity < FIXVEL*/) {
						semiActive = j;
						//cout << semiActive << endl;
					}
				}
			}
		}
		int tempNote = (int)floor(angle / TWO_PI*NotesNumber.value) % NotesNumber.value; //integer: note;
		if (dist > neutralRegion && *velocity<FIXVEL) {
			prSemi = semi;
			if (semiActive == -1 && dist<releaseDist) {
				semi = false;
			}
			else {
				tempNote = semiActive % NotesNumber.value;
				semi = true;
				inside = false;
			}
			if ((semi && prNote != tempNote) || prSemi != semi) {
				//cout << "  [" << semi << "," << prSemi << "]   ";
				//if (temp) {
				prNote = note;
				note = tempNote;
				//}
				changed = true;
				//inside = false;
			}
		}
		//cout << semi;
		//if(advanced){
		//	//chordONOFF.update(gaze);
		//	percussive.update(gaze);
		//	fixation.update(gaze);

		//	
		//}


		if(percussive.value && !pressed){
			
			if(dist>neutralRegion && (dist<height2 || semi)&& notesONOFF.value ){
				if(tempNote!=prNote || semi != prSemi || fixationInNeutral ){
					if(*velocity<FIXVEL || (!fixation.value && (!chordONOFF.value || tempNote<NotesNumber.value-CHORDSNUM))){
						prNote=note;
						note=tempNote;
						if(note!=prNote || fixationInNeutral){
							if(!chordONOFF.value || tempNote<NotesNumber.value-CHORDSNUM)
							{
								changed=true;
							}
							if((!chordONOFF.value || note<NotesNumber.value-CHORDSNUM ))
								inside=false;
						}
					}
				}
				else note=-1;
			}
			else note=-1;
		}
		else{
			pressed=false;
			if(dist>neutralRegion && (dist<height2 /*|| semiActive != -1*/) && notesONOFF.value && (*velocity<FIXVEL /*|| !fixation.value*/ )){
				if(chordONOFF.value && tempNote>=NotesNumber.value-CHORDSNUM){
					prNote=note;
					if(*velocity<FIXVEL){
						note=tempNote;
					}
					//dist=prDist;
				}
				//always apply a fixation detection for changing the chords
				else{
					prNote=note;
					note=tempNote;
					if(note!=prNote /*&& semi==prSemi*/|| fixationInNeutral){
						changed=true;
						//cout << "changed\t";
					}
					if((!chordONOFF.value || note<NotesNumber.value-CHORDSNUM ))
						inside=false;
				}
			}

		}
		if (((dist > releaseDist && dist < releaseDistEnd && !semi)||(inRelease && dist <neutralRegion && dist > inreleaseRegion))&& *velocity < variables::inReleaseFIXVEL) {
			//cout << semi;
			prNote = note;
			note = -1;//this means release note / rest
			    changed=true;
			//replaySame.setup("", false, ofPoint(-10, -10), 0.001, 10, 0, 0, 0, false);
		}
		

		//if((dist<releaseRegion ) && *velocity<FIXVEL)     notesONOFF.value=true;
		
		if((/*replaySame.size<0.1 && */!chordONOFF.value || note<NotesNumber.value-CHORDSNUM ) && note!=-1 && dist>neutralRegion  &&  *velocity<FIXVEL){
			melody=note;
			/*cout << ".";*/
			//replaySame.setup("",false,ofPoint(-cos((note+0.5)*tangle)*scaling, -sin((melody+0.5)*tangle)*scaling),0.095,10,0,0,0,false);
			
			if (semitoneActive) {
				replaySame.setup("", false, ofPoint(0, 0), 0.095f*radius / (float)height2, 10, 0, 0, 0, false);
			}
			else {
				replaySame.setup("", false, ofPoint(0, 0), 0.095, 10, 0, 0, 0, false);
			}
			
			replaySame.resized(width,height);
			//release.setup("Rls",false,ofPoint(cos((note+0.5)*tangle)*scaling, sin((melody+0.5)*tangle)*scaling),tangle/TWO_PI,10,0,0,0,false);
			
		}
		if(replaySameActive)
			replaySame.update(gaze);
		if(replaySame.active && *velocity<FIXVEL && !inside){
			note=melody;
			//cout <<"["<< melody<<","<<prSemi<<"]"<<endl;
			//semi = prSemi;
			inside=true;
			changed=true;
		}
		if (!replaySame.active)
			inside = false;
		
		/*if(tempNote==melody && dist<neutralRegion*RELEASE_DIST_END && !inside && *velocity < 100){
			note=prNote;
			melody=note;
			inside=true;
			changed=true;
		}*/
		if(changed){
			fixationInNeutral=false;
		}
	}
	static int replaySameTimer = 0;
	if (ofDist(gaze.x, gaze.y, width2, height2) < neutralRegion/* && *velocity < FIXVEL*/ && !replaySame.active) {
		if (replaySameTimer++ > variables::framerate * REPLAYFADEOUT) {
			replaySame.setup("", false, ofPoint(-10, -10), 0.001, 10, 0, 0, 0, false);
			replaySameTimer = 0;
		}
	}
	else
		replaySameTimer = 0;
	//cout << /*"TempoNote: " << tempNote << */" note: " << note << endl;
	if(!*conf)
		testSong.update(gaze);
	
}

float Disc::getMagVal(float i){
/*	float mag=1;
	float magFac=-0.01;
	float out;
	out=(i*tangle-angle)*mag;
	float r=(out*out);
	//if(r<10){
		out=out*(1+magFac*r)+angle;
		return out;
	//}*/
	return i*tangle;
}

void Disc::draw(){
	ofSetColor(60,70,40);
	ofCircle(width2, height2, releaseDistEnd);
	ofCircle(width2, height2, neutralRegion);
	int brightActive=(float)(dist-neutralRegion)/(float)(height2-neutralRegion)*80;
    for(int i=0;i<NotesNumber.value;i++){
		
		if(chordONOFF.value && (i+CHORDSNUM+1-NotesNumber.value<=CHORDSNUM) && (i+CHORDSNUM-NotesNumber.value>=0)){
            ofSetColor(i*colorstep*R+((note == i && !semi) && notesONOFF.value)*brightActive, i*colorstep *1.5*G+((note == i && !semi) && notesONOFF.value)*brightActive,i*colorstep*B+((note==i && !semi) && notesONOFF.value)*brightActive);
		}
		else {
			ofSetColor(i*colorstep*R + ((note == i && !semi) && notesONOFF.value)*brightActive, i*colorstep *G + ((note == i && !semi) && notesONOFF.value)*brightActive, i*colorstep*B + ((note == i && !semi) && notesONOFF.value)*brightActive);
		}
		if (testSong.ilumina == i && !testSong.melody[testSong.pos].flat) {
			ofSetColor(255, 20, 20);
		}
		
		ofBeginShape();
        ofVertex(width2, height2);
		for (float c = 0; c <= 1.0f; c += (float)NotesNumber.value/50.f) {
			ofVertex(width2 - cos(getMagVal(i + c))*radius, height2 + sin(getMagVal(i + c))*radius);
			
		}
		ofVertex(width2 - cos(getMagVal(i + 1))*radius, height2 + sin(getMagVal(i + 1))*radius);
		ofEndShape(TRUE);
		if (note == i && testSong.ilumina != -1 && !chordONOFF.value && !semi && !fixationInNeutral) {
			ofEnableAlphaBlending();
			float quedaSamples = (float)stepSequencer::noteSamples * (float)testSong.prDuration - testSong.curDuration;
			if (quedaSamples < (float)SAMPLERATE * 0.6) {
				ofSetColor(255, 170);
			}
			else {
				ofSetColor(testSong.noteProgress * 170, 170);
			}
			ofBeginShape();
			ofVertex(width2, height2);
			for (float c = 0; c <= 1.0f; c += (float)NotesNumber.value / 50.f) {
				ofVertex(width2 - cos(getMagVal(i + c))*radius, height2 + sin(getMagVal(i + c))*radius);

			}
			ofVertex(width2 - cos(getMagVal(i + 1))*radius, height2 + sin(getMagVal(i + 1))*radius);
			ofEndShape(TRUE);
		}
		if ((i - 3) % variables::notesPerScale == 0)
			ofSetColor(255, 255, 0);
		else if((i ) % variables::notesPerScale == 0)
			ofSetColor(255, 0, 0);
		else
			ofSetColor(255, 255, 255);
        if(distVolume.value){
            for(int j=1;j<5;j++){
                ofCircle(width2-cos((i+0.5)*tangle)*(neutralRegion + j*outSpotStep), height2+sin((i+0.5)*tangle)*(neutralRegion + j*outSpotStep),pointSize);
            }
        }
        else{    
			ofCircle(width2-cos((i+0.5)*tangle)*(neutralRegion + 2.5*outSpotStep), height2+sin((i+0.5)*tangle)*(neutralRegion + 2.5*outSpotStep),pointSize);
		}
		ofCircle(width2-cos((i+0.5)*tangle)*height2*RELEASE_DIST*1.13, height2+sin((i+0.5)*tangle)*height2* RELEASE_DIST*1.13,pointSize);
//        ofSetColor(200,200,255);
//        ofCircle(width2-cos((i+0.5)*tangle)*inSpotDist, height2+sin((i+0.5)*tangle)*inSpotDist,pointSize);
		
	}
	if (inRelease) {
		ofSetColor(60, 70, 40);
		ofCircle(width2, height2, neutralRegion);
		//ofEnableAlphaBlending();
		ofSetColor(20, 20, 20);
		ofCircle(width2, height2, inreleaseRegion);
		int releaseSpot = ((neutralRegion + inreleaseRegion) / 2);
		ofSetColor(255, 255, 255);
		for (int i = 0; i < NotesNumber.value; i++)
			ofCircle(width2 - cos((i + 0.5)*tangle)*releaseSpot, height2 + sin((i + 0.5)*tangle)*releaseSpot, pointSize);
	}
	else
	{
		ofSetColor(20, 20, 20);
		ofCircle(width2, height2, neutralRegion);
	}
	
	
	ofSetColor(255,255,255);
	ofCircle(width2,height2,pointSize);
	if (/*percussive.value && */replaySameActive) {
		replaySame.draw();
	}
	if (!chordONOFF.value /*&& fixationInNeutral*/ && testSong.ilumina!=-1) {
		if (testSong.centerRepeat) {
			ofSetColor(255, 20, 20);
			ofCircle(replaySame.pos, replaySame.size);
		}
		ofEnableAlphaBlending();
		float quedaSamples = (float)stepSequencer::noteSamples * (float)testSong.prDuration - testSong.curDuration;
		if (quedaSamples < (float)SAMPLERATE * 0.6f) {
			ofSetColor(255, 170);
		}
		else {
			ofSetColor(testSong.noteProgress * 170, 170);
		}
		if(inRelease)
			ofCircle(ofPoint(width2,height2), inreleaseRegion);
		else
			ofCircle(ofPoint(width2, height2), neutralRegion);
		ofDisableAlphaBlending();
		ofSetColor(255, 255, 255);
		ofCircle(replaySame.pos, pointSize);
	}
	
			//ofDisableAlphaBlending();
	
    for(int i=0;i<NotesNumber.value;i++){
		ofPoint curPos = ofPoint(width2 - cos((i + 0.5)*tangle)*inSpotDist, height2 + sin((i + 0.5)*tangle)*inSpotDist);
		ofPoint curPosSemitone = ofPoint(width2 - cos((i)*tangle)*radius, height2 + sin((i)*tangle)*radius);
		ofPoint SemSpotPos[4];
		if (semitoneActive && variables::notesPerScale==7) {
			for (int j = 0; j < 4; j++)
				SemSpotPos[j] = ofPoint(width2 - cos((i)*tangle)*(radius - tangle*SemiSize*0.6 + tangle * SemiSize*0.4*j), height2 + sin((i)*tangle)*(radius - tangle * SemiSize*0.6 + tangle * SemiSize*0.4*j));

			int ii = (i + variables::firstNote) % variables::notesPerScale;
			//if ((ii == 2 || ii == 3 || ii == 5 || ii == 6 || ii == 0) && i != 0 && i < NotesNumber.value - 7 * chordONOFF.value) {
			int tempDif = (*scale[ii] - *scale[(ii + 6) % variables::notesPerScale]);
			if ((tempDif> 1 || (tempDif < 0 && tempDif>-11)) && i != 0 && i < NotesNumber.value - variables::notesPerScale * chordONOFF.value) {

				ofSetColor(40 + (i == semiActive) * 50);
				
				if (testSong.ilumina == i && testSong.melody[testSong.pos].flat) {
					ofSetColor(255, 20, 20);
				}
				ofCircle(curPosSemitone.x, curPosSemitone.y, tangle*SemiSize);
				if (!chordONOFF.value && i==semiActive) {
					ofEnableAlphaBlending();
					float quedaSamples = (float)stepSequencer::noteSamples * (float)testSong.prDuration - testSong.curDuration;
					if (quedaSamples < (float)SAMPLERATE * 0.6) {
						ofSetColor(255, 170);
					}
					else {
						ofSetColor(testSong.noteProgress * 170, 170);
					}

					ofCircle(curPosSemitone.x, curPosSemitone.y, tangle*SemiSize);
					ofDisableAlphaBlending();
				}
				ofSetColor(255);
				for (int j = 0; j < 4; j++)
					ofCircle(SemSpotPos[j].x, SemSpotPos[j].y, tangle * SemiSize*0.05);
			}
		}
        if(chordONOFF.value){
            if(i<NotesNumber.value-CHORDSNUM){
                
                /*int c=255*(i!=melody);
                ofSetColor(c,255,c);
                variables::myfont.drawString(ofToString((i+4)%7+1),curPos.x,curPos.y);*/
				if (numberNote == 0) {
					string temp = ofToString((i + variables::firstNote) % variables::notesPerScale + 1);
					variables::myfontPie.drawString(temp, curPos.x - variables::myfontPie.stringWidth(temp) / 2.f, curPos.y);
				}
				else {
					switch ((i + variables::firstNote) % variables::notesPerScale + 1) {
					case 1:
						//ofSetColor(255);
						variables::myfontPie.drawString("DO", curPos.x-variables::myfontPie.stringWidth("DO")/2.f, curPos.y);
						//variables::myfontPie.drawString("DO", curPos.x-variables::myfontPie.stringWidth("DO")/2.f, curPos.y);
						break;
					case 2:
						variables::myfontPie.drawString("RE", curPos.x-variables::myfontPie.stringWidth("RE")/2.f, curPos.y);
						break;
					case 3:
						variables::myfontPie.drawString("MI", curPos.x-variables::myfontPie.stringWidth("MI")/2.f, curPos.y);
						break;
					case 4:
						variables::myfontPie.drawString("FA", curPos.x-variables::myfontPie.stringWidth("FA")/2.f, curPos.y);
						break;
					case 5:
						variables::myfontPie.drawString("SOL", curPos.x-variables::myfontPie.stringWidth("SOL")/2.f, curPos.y);
						break;
					case 6:
						variables::myfontPie.drawString("LA", curPos.x-variables::myfontPie.stringWidth("LA")/2.f, curPos.y);
						break;
					case 7:
						variables::myfontPie.drawString("SI", curPos.x-variables::myfontPie.stringWidth("SI")/2.f, curPos.y);
						break;
					}
				}
            }
            int pos=i+CHORDSNUM+1-NotesNumber.value;
			string temp;
            switch(pos){
                case 1:
                    ofSetColor(155*(*chord==5)+100,155*(*chord==5)+100,155*(*chord==5)+100);
					if (numberNote == 0)
						temp = "VI";
					else
						temp = "LA";
                    break;
                case 2:ofSetColor(100+155*(*chord==1),100+155*(*chord==1),100+155*(*chord==1));
					if (numberNote == 0)
						temp = "II";
					else
						temp = "RE";
                    break;
                case 3:ofSetColor(100+155*(*chord==4),100+155*(*chord==4),100+155*(*chord==4));
					if (numberNote == 0)
						temp = "V";
					else
						temp = "SOL";
                    break;
                case 4:ofSetColor(100+155*(*chord==0),100+155*(*chord==0),100+155*(*chord==0));
					if (numberNote == 0)
						temp = "I";
					else
						temp = "DO";
                    break;
                case 5:ofSetColor(100+155*(*chord==3),100+155*(*chord==3),100+155*(*chord==3));
					if (numberNote == 0)
						temp = "IV";
					else
						temp = "FA";
                    break;
                case 6:ofSetColor(100+155*(*chord==6),100+155*(*chord==6),100+155*(*chord==6));
					if (numberNote == 0)
						temp = "VII";
					else
						temp = "SI";
                    break;
				case 7:ofSetColor(100+155*(*chord==2),100+155*(*chord==2),100+155*(*chord==2));
					if (numberNote == 0)
						temp = "III";
					else
						temp = "MI";
                    break;
            }
			variables::myfontPie.drawString(temp, width2 - cos((i + 0.5)*tangle)*inSpotDist-variables::myfontPie.stringWidth(temp)/2.f, height2 + sin((i + 0.5)*tangle)*inSpotDist);
        }
        else{
            int c=255*(i!=melody);
            ofSetColor(c,255,c);
			if (!numberNote) {
				string temp = ofToString((i + variables::firstNote) % variables::notesPerScale + 1);
				variables::myfontPie.drawString(temp, curPos.x - variables::myfontPie.stringWidth(temp) / 2.f, curPos.y);

			}
			else {
				switch ((i + variables::firstNote) % variables::notesPerScale + 1) {
				case 1:
					variables::myfontPie.drawString("DO", curPos.x-variables::myfontPie.stringWidth("DO")/2.f, curPos.y);
					break;
				case 2:
					variables::myfontPie.drawString("RE", curPos.x-variables::myfontPie.stringWidth("RE")/2.f, curPos.y);
					break;
				case 3:
					variables::myfontPie.drawString("MI", curPos.x-variables::myfontPie.stringWidth("MI")/2.f, curPos.y);
					break;
				case 4:
					variables::myfontPie.drawString("FA", curPos.x-variables::myfontPie.stringWidth("FA")/2.f, curPos.y);
					break;
				case 5:
					variables::myfontPie.drawString("SOL", curPos.x-variables::myfontPie.stringWidth("SOL")/2.f, curPos.y);
					break;
				case 6:
					variables::myfontPie.drawString("LA", curPos.x-variables::myfontPie.stringWidth("LA")/2.f, curPos.y);
					break;
				case 7:
					variables::myfontPie.drawString("SI", curPos.x-variables::myfontPie.stringWidth("SI")/2.f, curPos.y);
					break;
				}
			}
		}
    }
   
    //ofSetColor(0,50*(prbeat!=*beat),0);prbeat=*beat;
	
	if (variables::alperMode && !variables::alperConfigureActive)
		notesONOFF.draw();
	// chordONOFF.draw();
	 if(*conf/*||variables::alperMode*/)
		chordONOFF.draw();
		 //distVolume.draw();
	 //if(!inside)
    if(advanced){
       // chordONOFF.draw();
        fixation.draw();
        distVolume.draw();
        percussive.draw();
    }
	
	 //sharp45.draw();
//    ofSetColor(0,0,0);
//    int melPosX=width2-cos((melody+0.5)*tangle)*inSpotDist;
//    int melPosY=height2+sin((melody+0.5)*tangle)*inSpotDist;
//    ofCircle(melPosX,melPosY,0.03f*height);
//    ofSetColor(200,200,200);
//    variables::myfont.drawString(ofToString(melody%7+1),melPosX,melPosY);
}

void Disc::resized(int w, int h){

	
    width=w;width2=w/2;
    height=h; height2=height/2.f;
	if (semitoneActive) {
		radius = height2 - height / (float)NotesNumber.value;
		//radius = height2;
		replaySame.setup("", false, ofPoint(0, 0), 0.095f*radius/(float)height2, 10, 0, 0, 0, false);
	}
	else {
		radius = height2;
		replaySame.setup("", false, ofPoint(0, 0), 0.095, 10, 0, 0, 0, false);
	}
	float diameter = radius * 2;
	SemiSize = radius*0.35f;
    if(distVolume.value)
        neutralRegion=diameter*NR;
    else
        neutralRegion=diameter*NR;
    //releaseRegion=h*0.11;
    outSpotStep=(radius -neutralRegion)/5;
    inSpotDist=0.6*neutralRegion;
	pointSize = diameter*0.005;
	releaseDist = radius*RELEASE_DIST;
	releaseDistEnd = releaseDist*RELEASE_DIST_END;
	inreleaseRegion = neutralRegion*0.85;
    notesONOFF.resized(w,h);
    chordONOFF.resized(w,h);
    NotesNumber.resized(w,h);
    fixation.resized(w,h);
    percussive.resized(w,h);
    distVolume.resized(w,h);
	sharp45.resized(w, h);
	testSong.resized(w, h);
	replaySame.resized(w, h);
}


void Disc::keyPressed(int key){
    prNote=-1;
    pressed=true;
	switch (key){
		case	'q':
            note=0;changed=true;
			break;
        case	'w':
            note=1;changed=true;
			break;
        case	'e':
            note=2;changed=true;
			break;
        case	'r':
            note=3;changed=true;
			break;
        case	't':
            note=4;changed=true;
			break;
        case	'y':
            note=5;changed=true;
			break;
        case	'u':
            note=6;changed=true;
			break;
        case	'i':
            note=7;changed=true;
			break;
        case	'o':
            note=8;changed=true;
			break;
		case	'p':
            note=9;changed=true;
			break;
	}
}

