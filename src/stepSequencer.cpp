#include "stepSequencer.h"
//#include "ofApp.h"
int stepSequencer::timeThre=40;
int stepSequencer::curSample;
int stepSequencer::noteSamples;
DistControl stepSequencer::numberOfNotes;

void stepSequencer::setup(DistControl * Scale,int * Transpose, int NumberOfNotes, control * Tempo, int Dwell, float R, float G, float B, int * SampleRate,int * Chord, ofxMidiOut * midinote,bool*Conf){
    scale=Scale;
    sampleRate=SampleRate;
    maxNotes=32;
    tempo=Tempo;
	conf=Conf;
    waitSamples=new int[maxNotes];
    tvol=new float[maxNotes];
    nextPhaseAdder=new float[maxNotes];
    rownote=new int[maxNotes];
    rowoct=new int[maxNotes];
    int numOfNotes;
    if(NumberOfNotes<=maxNotes)
        numOfNotes=NumberOfNotes;
    else     numOfNotes=maxNotes;

    char** names = new char*[4];
    char ** brightNames= new char*[4];
    for(int i = 0; i < 4; ++i){
        names[i] = new char[20];
        brightNames[i]=new char[20];
    }
    strcpy(names[0],"Meter");
    strcpy(names[1],"Release");
    strcpy(names[2],"Octave");
    strcpy(names[3],"Volume");
    ofPoint sliderPos=ofPoint(-1.55,0);
    ofPoint stepPosUP=ofPoint(-1.35,0.3);
    ofPoint stepPosDW=ofPoint(-1.35,-.3f);
    strcpy(brightNames[0],"Sinus");
    strcpy(brightNames[1],"Dim");
    strcpy(brightNames[2],"Vibra");
    strcpy(brightNames[3],"Bright");
    brightness.setup(4,brightNames,3,ofPoint(-1.83,-0.85),0,0.04,1000);
    controlMultiplex.setup(4,names,0,ofPoint(-1.12,-0.5),HALF_PI,0.045,800);
    numberOfNotes.setup("Meter" , stepPosUP,stepPosDW, 1, maxNotes, numOfNotes,1, .12f,800, 0.7f,0.4f,0.1f,true);
    releaseFactor.setup("Release",128, stepPosUP,stepPosDW,0.9995f,0.99999f, .045f,  100, 5, 0.7f,0.4f,0.1f);
    octave.setup("Octave",stepPosUP,stepPosDW, -3,4,-1,1,.045f,800,0.7f,0.4f,0.1f,true);
    targetVolume.setup("Volume",100, stepPosUP,stepPosDW,0,.7f,0.045f,500,20,0.7f,0.4f,0.1f);
    numberOfNotesSlider.setup(sliderPos,numberOfNotes.value,numberOfNotes.min,numberOfNotes.max,0.7,false);
    releaseFactorSlider.setup(sliderPos,releaseFactor.color,0,255,0.7,false);
    octaveSlider.setup(sliderPos,octave.value,octave.min,octave.max,0.7,false);
    targetVolumeSlider.setup(sliderPos,targetVolume.color,0,255,0.7,false);
    buttonDiameter=1.0f/(float)numberOfNotes.value;
    transpose=Transpose;
    dwell=Dwell;
    RED=R;
    GREEN=G;
    BLUE=B;
    curSample=0;
	timer=0;
    prChord=*Chord;

    noteSamples=(int)(SAMPLERATE/(tempo->value/60.0f));
    noteFrames=tempo->value/60.0f*variables::framerate;
    totalSamples=(int)noteSamples*numberOfNotes.value;
    attackSamples=*sampleRate*0.05;
    attackFactor=10.0f/attackSamples;
    chord=Chord;
    seqNote = new sequencerNote*[maxNotes];
    volume=targetVolume.value;
	
	Eagle.setup("Magnify",false,ofPoint(1.25,0.85),.08,800,.8,.4,0,false);
    for(int i=0;i<maxNotes;i++){
        seqNote[i]=new sequencerNote[maxNotes];
        tvol[i]=0;
        waitSamples[i]=noteSamples*i;
        rownote[i]=scale[i%variables::notesPerScale].value;
        rowoct[i]=(int)i/ variables::notesPerScale +octave.value;
        float targetFrequency= TUNING/2*pow(2,rowoct[i])*(float)pow(2,(float)rownote[i]/12.0f);
        nextPhaseAdder[i] = (targetFrequency / (float) *sampleRate) * 6.2831f;
        for(int j=0;j<maxNotes;j++){
			seqNote[i][j].setup(Scale,Transpose,&curSample,dwell,i,j,chord,&totalSamples,&noteSamples,&waitSamples[i],&tvol[i],&nextPhaseAdder[j],&volume,&attackSamples,&attackFactor,&releaseFactor,sampleRate, &buttonDiameter, RED, GREEN, BLUE,&(brightness.selected), midinote, &(rownote[j]), &(rowoct[j]),Eagle.value);
        }
   }
   localOctave=0;
   deleteAll.setup("ClearAll",false,ofPoint(-1.58,0.8),0.095,1000,0.1,0.1,0.1,false);
   //monophonic.setup("monophonic",false,ofPoint(-1.2,0.8),.095,800,.8,.4,0,false);
   undo.setup("Delete", false, ofPoint(-1.2, 0.8), .095, 800, 0, 0, 0, false);
	   //   myfile.open ("./data/logSeq.txt",ios::trunc);
   beat=1;
   numAccessX=0;
   numAccessY=0;
  // seqNote[numAccessX][numAccessY].button.arrowsAccess=true;
   numPressed=false;
   numAccess=false;
   magDynamic=false;
   lastEagle=ofPoint(width/2,height/2);
   outSideLense=true;
   lastFix=ofPoint(0,0);
}

//stepSequencer::~stepSequencer(){
//    myfile.close();
//}

void stepSequencer::update(ofPoint gaze,bool*sacadic,float*velocity){
	//cout<<Switch::magActive;
   // controlMultiplex.update(gaze);
	
	//Switch::windowCenterChanged=false;
	Gaze=gaze;
	
	if(*conf){
		//monophonic.update(gaze);
		Eagle.update(gaze,sacadic);
	}
	if(*sacadic)
		lastFix=gaze;
	if(Eagle.changed){
		for(int i=0;i<maxNotes;i++)
			for(int j=0;j<maxNotes;j++){
				seqNote[i][j].button.eagleEnable=Eagle.value;
				seqNote[i][j].button.resized(width,height);
			}
	}
    deleteAll.update(gaze);
	undo.update(gaze);
	if (undo.changed && !arpeggioStack.empty()) {
		gridElement temp;
		temp = arpeggioStack.top();
		arpeggioStack.pop();
		seqNote[temp.y][temp.x].button.value = false;
	}
	//brightness.update(gaze);
	if(controlMultiplex.selected==0){
		if(*conf){
			numberOfNotes.update(gaze);
			// numberOfNotesSlider.update(gaze);
			if(numberOfNotes.changed)     {
				numberOfNotesSlider.setValue(numberOfNotes.value);
				buttonDiameter=1.0f/(float)numberOfNotes.value;
				for(int i=0;i<maxNotes;i++)
					for(int j=0;j<maxNotes;j++){
						seqNote[i][j].button.pointsize=0.05*seqNote[i][j].button.size+1;
						seqNote[i][j].resized(width,height);
					}

			}

			if(numberOfNotesSlider.changed){
				numberOfNotes.setColorByValue(numberOfNotesSlider.value);
			}
		}
    }
    if(controlMultiplex.selected==1){
        releaseFactor.update(gaze);
        releaseFactorSlider.update(gaze);
        if(releaseFactor.changed)            releaseFactorSlider.setValue(releaseFactor.color);
        if(releaseFactorSlider.changed)      releaseFactor.setValueByColor(releaseFactorSlider.value);
    }
    if(controlMultiplex.selected==2){
        octave.update(gaze);
        octaveSlider.update(gaze);
        if(octave.changed)      octaveSlider.setValue(octave.value);
        if(octaveSlider.changed){
            octave.setColorByValue(octaveSlider.value);
        }
    }
    if(controlMultiplex.selected==3){
        targetVolume.update(gaze);
        targetVolumeSlider.update(gaze);
        if(targetVolume.changed)            targetVolumeSlider.setValue(targetVolume.color);
        if(targetVolumeSlider.changed)      targetVolume.setValueByColor(targetVolumeSlider.value);
    }



    if(deleteAll.changed)   clearAll();
    if(octave.changed) updatePitch();
    if(numberOfNotes.changed){
        buttonDiameter=1.0f/(float)numberOfNotes.value;
        totalSamples=noteSamples*numberOfNotes.value;
        for(int i=0;i<maxNotes;i++){
            for(int j=0;j<maxNotes;j++){
                seqNote[i][j].setNumberOfNotes();
            }
        }
    }
    if(tempo->changed){
        updateTempo();
    }
	bool awayEnough=true;
	//if(Switch::lenseSlave){
	//	Switch::MagWindowCenter=gaze;
	//	Switch::magActive=true;
	//}
	//else
		awayEnough=sacadicDistance(gaze,velocity,*sacadic);
    for(int i=0;i<numberOfNotes.value;i++){
        for(int j=0;j<numberOfNotes.value;j++){
            bool temp;
			seqNote[i][j].update(gaze,&awayEnough);
			if (seqNote[i][j].button.changed && seqNote[i][j].button.value) {
				gridElement temp;
				temp.x = j;
				temp.y = i;
				arpeggioStack.push(temp);
			}
			if(i==numAccessX && j==numAccessY &&numPressed){
				seqNote[numAccessX][numAccessY].button.value=!seqNote[numAccessX][numAccessY].button.value;
				seqNote[numAccessX][numAccessY].button.changed=true;
				numPressed=false;
			}
            if(seqNote[i][j].button.changed && monophonic.value){
                temp=seqNote[i][j].button.value;
                for(int k=0;k<numberOfNotes.value;k++){
                    if(seqNote[i][k].button.value==true && k!=j){
                        seqNote[i][k].button.value=false;
//                        myfile<<ofGetFrameNum()<<" , "<<i<<" , "<<k<<" , "<<seqNote[i][k].button.value<<"\n";
                    }
                }
            }
        }
    }

}
//
//void stepSequencer::computeMagThr(){
//	Switch::magThr=0;
//	int temp=0;
//	if(Switch::MagWindowCenter.y>height/2){
//		for(int i=0;i<numberOfNotes.value;i++){
//			if(seqNote[i][0].button.magout>temp)
//				temp=seqNote[i][0].button.magout;
//		}
//		Switch::magThr=temp;
//		//Switch::magThr=-100;
//	}
//	else{
//		for(int i=0;i<numberOfNotes.value;i++){
//			if(seqNote[i][numberOfNotes.value-1].button.magout<temp)
//				temp=seqNote[i][numberOfNotes.value-1].button.magout;
//		}
//		Switch::magThr=temp;
//		//Switch::magThr=100;
//	}
//	cout<<Switch::magThr<<'\n';
//}

bool stepSequencer::sacadicDistance(ofPoint gaze,float* velocity,bool sacadic){
	bool awayEnough=false;
	outSideLense=outsideMagRegion(gaze,lastEagle);
	if(*velocity<200 && (!Switch::magActive || outSideLense || Switch::lenseSlave)){//if an new fixation outside
		Switch::magActive=false;
		timer++;
		if(timer>=timeThre || Switch::lenseSlave){
			awayEnough=true;
			lastEagle=lastFix;
			/*if(gaze.y-Switch::MagRegion*Switch::Mag<0){
				lastEagle.y-=gaze.y-Switch::MagRegion*Switch::Mag;
				cout<<lastEagle.y<<' ';
			}*/
			Switch::MagWindowCenter=lastEagle;
			Switch::magOffset=0;
			Switch::magActive=true;
			timer=0;
			//computeMagThr();
		}
	}
	else
		timer=0;
	
	return awayEnough;
}

bool stepSequencer::outsideMagRegion(ofPoint gaze,ofPoint lastEagle){
	/*if(!magDynamic){
	int region=;
	return(abs(gaze.x-lastEagle.x) > region || abs(gaze.y-lastEagle.y)>region);
	}*/
	float thrSize;
	if(magDynamic)
		thrSize=seqNote[0][0].button.orSize*(Switch::Mag*0.9);
	else
		thrSize=seqNote[0][0].button.orSize;
	for(int i=0;i<numberOfNotes.value;i++){
		for(int j=0;j<numberOfNotes.value;j++){
			if(seqNote[i][j].button.size>thrSize){
				if(gaze.x<width/2-height/2 || gaze.x>width/2+height/2){
					if(abs(lastEagle.y-gaze.y)<seqNote[i][j].button.size*1.42)
						return false;
				}
				else if(gaze.y<0 || gaze.y>height){
					if(abs(lastEagle.x-gaze.x)<seqNote[i][j].button.size*1.42)
						return false;
				}
				else
					if(ofDist(seqNote[i][j].button.magPos.x,seqNote[i][j].button.magPos.y,gaze.x,gaze.y)<seqNote[i][j].button.size*1.42)
						return false;
			}
		}
	}
	return true;

}

void stepSequencer::sendMidi(){
    for(int i=0;i<maxNotes;i++)
        for(int j=0;j<maxNotes;j++)
            seqNote[i][j].sendMidi(volume*128);
}

void stepSequencer::releaseAllMidi(){
    for(int i=0;i<numberOfNotes.value;i++)
        for(int j=0;j<numberOfNotes.value;j++){
            seqNote[i][j].releaseMidi();
			Sleep(1);
		}
}

void stepSequencer::clearAll(){
     for(int i=0;i<maxNotes;i++)
            for(int j=0;j<maxNotes;j++)
                seqNote[i][j].button.value=false;
}

void stepSequencer::updatePitch(){
//    if(*chord-prChord>4){     //dynamic chord progression
//        if(localOctave>-1)   localOctave--;
//        cout<<localOctave<<'\n';
//    }
//    if(*chord-prChord<-4){
//        if(localOctave<1)   localOctave++;
//        cout<<localOctave<<'\n';
//    }
	for(int i=0;i<numberOfNotes.value;i++)
            for(int j=0;j<numberOfNotes.value;j++){
				if(seqNote[i][j].sounds){
					seqNote[i][j].releaseMidi(seqNote[i][j].midinote);
					//seqNote[i][j].StepMidiOut->sendNoteOff(sequencer_midi, seqNote[i][j].midinote, 0);
				}
			}
    if(*chord>5)    localOctave=-1;
    else    localOctave=0;
    prChord=*chord;
    for(int i=0;i<maxNotes;i++){
        rownote[i]=scale[(i+*chord)% variables::notesPerScale].value;
        rowoct[i]=(int)(i+*chord)/ variables::notesPerScale +octave.value+localOctave;
        float targetFrequency= TUNING/2*pow(2,rowoct[i])*(float)pow(2,(rownote[i]+*transpose)/12.0f);
        nextPhaseAdder[i] = (targetFrequency / (float) *sampleRate) * 6.2831f;
    }
}

void stepSequencer::updateTempo(){
    noteSamples=(int)((float)(*sampleRate)/(tempo->value/60.0f));
    noteFrames=tempo->value/60.0f*variables::framerate;
    totalSamples=noteSamples*numberOfNotes.value;
    for(int i=0;i<maxNotes;i++)   waitSamples[i]=noteSamples*i;
}

float stepSequencer::getSample(){
    float sample=0.0f;
    float tempSample;
    for(int i=0;i<numberOfNotes.value;i++){
        volume=0.99*volume+0.01*targetVolume.value;
        if(curSample>=waitSamples[i] && curSample<=attackSamples+waitSamples[i]){
            tvol[i]=volume*attackFactor+tvol[i]*(1-attackFactor);
            beat=ofGetFrameNum();
        }
        else    tvol[i]=tvol[i]*(releaseFactor.value);
        if(tvol[i]>0.001){//here put 0.001 for example if for computational reason we do not want to compute very low amplitude sounds
            tempSample=0.0f;
            for(int j=0;j<maxNotes;j++){
                if(seqNote[i][j].isActive()){
                    tempSample+=seqNote[i][j].getSample();
                }
            }
            tempSample*=tvol[i];
            sample+=tempSample;
        }
    }
    curSample=(curSample+1)%totalSamples;
    return sample;
}

void stepSequencer::draw(){
    if(controlMultiplex.selected==0){
		if(*conf)
			numberOfNotes.draw();
        //numberOfNotesSlider.draw();
    }
    if(controlMultiplex.selected==1){
        releaseFactor.draw();
        releaseFactorSlider.draw();
    }
    if(controlMultiplex.selected==2){
        octave.draw();
        octaveSlider.draw();
    }
    if(controlMultiplex.selected==3){
        targetVolume.draw();
        targetVolumeSlider.draw();
    }
    //controlMultiplex.draw();
    deleteAll.draw();
	undo.draw();
	if(*conf){
		//monophonic.draw();
		Eagle.draw();
	}
	//brightness.draw();
	if(Eagle.value && abs(Gaze.x-width/2)<height*0.6 && !Switch::lenseSlave){
		ofSetColor((float)timer/timeThre*255);
		if(outSideLense)
			ofRect(lastFix.x-Switch::MagRegion,lastFix.y-Switch::MagRegion,2*Switch::MagRegion,2*Switch::MagRegion);
		else
			ofRect(lastEagle.x-Switch::MagRegion,lastEagle.y-Switch::MagRegion,2*Switch::MagRegion,2*Switch::MagRegion);
		//ofCircle(Gaze,(float)timer/(flo*Switch::MagRegion);
	}
    for(int i=0;i<numberOfNotes.value;i++){
        for(int j=0;j<numberOfNotes.value;j++){
            seqNote[i][j].draw();
        }
    }
	for(int i=0;i<numberOfNotes.value;i++){
        for(int j=0;j<numberOfNotes.value;j++){
			if(seqNote[i][j].button.size>seqNote[i][j].button.orSize)
				seqNote[i][j].draw();
        }
    }
	/*char temp[20];
	sprintf(temp,"%d, %d", numAccessX, numAccessY);
	variables::myfont.drawString(temp,ofGetWidth()/2,ofGetHeight()/2);*/
}

void stepSequencer::resized(int w,int h){
	width=w;
	height=h;
    controlMultiplex.resized(w,h);
    brightness.resized(w,h);
    deleteAll.resized(w,h);
   // monophonic.resized(w,h);
	undo.resized(w,h);
    releaseFactor.resized(w,h);
    numberOfNotes.resized(w,h);
    octave.resized(w,h);
    targetVolume.resized(w,h);
    numberOfNotesSlider.resized(w,h);
    releaseFactorSlider.resized(w,h);
    octaveSlider.resized(w,h);
	Eagle.resized(w,h);
    targetVolumeSlider.resized(w,h);
    for(int i=0;i<maxNotes;i++){
        for(int j=0;j<maxNotes;j++){
            seqNote[i][j].resized(w,h);
        }
    }
	buttonDiameter=1.0f/(float)numberOfNotes.value;
}


void stepSequencer::keyPressed(int key){
	if(numAccess){
		int prX=numAccessX;
		int prY=numAccessY;
		switch(key){
			case '4':numAccessX--;
				break;
			case '6':numAccessX++;
				break;
			case '8':numAccessY++;
				break;
			case '2':numAccessY--;
				break;
			case '1':numAccessX--;numAccessY--;
				break;
			case '3':numAccessX++;numAccessY--;
				break;
			case '7':numAccessX--;numAccessY++;
				break;
			case '9':numAccessX++;numAccessY++;
				break;
			case '5':numPressed=true;
				break;
			/*case '?':
				gridElement temp;
				temp= arpeggioStack.top();
				arpeggioStack.pop();
				seqNote[temp.y][temp.x].button.value = false;
				cout << temp.x;
				cout << "wtf";*/
		}
		
		if(numAccessX<0) numAccessX=numberOfNotes.value-1;
		if(numAccessY<0) numAccessY=numberOfNotes.value-1;
		numAccessX=numAccessX%numberOfNotes.value;	
		numAccessY=numAccessY%numberOfNotes.value;

		seqNote[prX][prY].button.arrowsAccess=false;
		seqNote[numAccessX][numAccessY].button.arrowsAccess=true;
	}
	if(key=='5'){
		numAccess=true;
		seqNote[numAccessX][numAccessY].button.arrowsAccess=true;
	}
}