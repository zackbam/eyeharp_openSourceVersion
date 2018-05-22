#include "controlArpeggio.h"

enum{
    VOLUME,BIGSTEP,SMALLSTEP,PITCH,PATTERNSIZE,METER,NOTESINCLUDED
};

void controlArp::setup(DistControl * Scale,int * Transpose, int* Chord, int* TotalSamples, int* SampleRate,int* CurSample){
    scale=Scale;
    chord=Chord;
    sampleRate=SampleRate;
    totalSamples=TotalSamples; //the tempo is for quarter notes and we count in 8ths.
    curSample=CurSample;

    arpVolume=50;
    curArpVol=arpVolume;
//    for(int i=0;i<4;i++){

//        arpeggios[i].setup(scale,Transpose, chord,totalSamples,curSample,sampleRate,.5,-1,-1,25,4,16,i+2);
//    }
//void arpeggio::setup(DistControl * Scale,int * Transpose,int * Chord,int * TotalSamples,int* CurSample, int* SampleRate,
//float Volume,int GlobalStep, int PatternStep,int StartingNote,int PatternSize, int Meter, int NotesIncl){
    arpeggios[0].setup(scale,Transpose, chord,totalSamples,curSample,sampleRate,.5,0,-1,25,3,4,3);
    arpeggios[1].setup(scale,Transpose, chord,totalSamples,curSample,sampleRate,.4,1,-1,34,4,8,4);
    arpeggios[2].setup(scale,Transpose, chord,totalSamples,curSample,sampleRate,.3,1,1,3,5,16,5);
    arpeggios[3].setup(scale,Transpose, chord,totalSamples,curSample,sampleRate,.01,-1,-1,40,10,32,6);

    char** names = new char*[5];
    for(int i = 0; i < 5; ++i){
        names[i] = new char[20];
    }
    strcpy(names[0],"Arp1");
    strcpy(names[1],"Arp2");
    strcpy(names[2],"Arp3");
    strcpy(names[3],"Arp4");
    strcpy(names[4],"MasterVol");
    ofPoint multiPos;
    multiPos.x=-1.1f;
    multiPos.y=-.76f;
    ofPoint stepPosUP=ofPoint(-1.35,0.2);
    ofPoint stepPosDW=ofPoint(-1.35,-.2f);
    ofPoint sliderPos=ofPoint(-1.55,0);
    selectArp.setup(5,names,0,ofPoint(-1.8,0.85),0,0.05,800);

    volume.setup("Vol",arpeggios[selectArp.selected].getVolume()*255,stepPosUP,stepPosDW,0.0f,1.0f,.045f,400,10,0.7f,0.4f,0.1f);
    globalStep.setup("BigStep",stepPosUP,stepPosDW,-4,4,arpeggios[selectArp.selected].getGlobalStep(),1,0.045f,800,0.7f,0.4f,0.1f,true);
    patternStep.setup("SmallStep",stepPosUP,stepPosDW,-4,4,arpeggios[selectArp.selected].getPatternStep(),1,0.045f,800,0.7f,0.4f,0.1f,true);
    startingNote.setup("1stPitch",stepPosUP,stepPosDW,0,40,arpeggios[selectArp.selected].getStartingNote(),1,0.045f,300,0.7f,0.4f,0.1f,true);
    patternSize.setup("patternSize",stepPosUP,stepPosDW,1,32,arpeggios[selectArp.selected].getPatternSize(),1,0.045f,800,0.7f,0.4f,0.1f,true);
    meter.setup("Meter",stepPosUP,stepPosDW,1,64,arpeggios[selectArp.selected].getMeter(),1,0.045f,800,0.7f,0.4f,0.1f,true);
    notesIncl.setup("NotesIncl",stepPosUP,stepPosDW,1,7,arpeggios[selectArp.selected].getNotesIncl(),1,0.045f,800,0.7f,0.4f,0.1f,true);

    char** controlNames= new char*[7];
    for(int i = 0; i < 7; ++i){
        controlNames[i] = new char[20];
    }
    strcpy(controlNames[0],"Volume");
    strcpy(controlNames[1],"BigStep");
    strcpy(controlNames[2],"SmallStep");
    strcpy(controlNames[3],"Pitch");
    strcpy(controlNames[4],"patternSize");
    strcpy(controlNames[5],"Meter");
    strcpy(controlNames[6],"NotesIncluded");
    controls.setup(7,controlNames,0,multiPos,HALF_PI,0.05f,500);

    volumeSlider.setup(sliderPos,volume.color, 0, 255, 0.7,false);
    globalStepSlider.setup(sliderPos,globalStep.value, globalStep.min, globalStep.max, 0.7,false);
    patternStepSlider.setup(sliderPos,patternStep.value, patternStep.min, patternStep.max, 0.7,false);
    startingNoteSlider.setup(sliderPos,startingNote.value, startingNote.min, startingNote.max, 0.7,false);
    patternSizeSlider.setup(sliderPos,patternSize.value, patternSize.min, patternSize.max, 0.7,false);
    meterSlider.setup(sliderPos,meter.value, meter.min, meter.max, 0.7,false);
    notesInclSlider.setup(sliderPos,notesIncl.value, notesIncl.min, notesIncl.max, 0.7,false);
}

void controlArp::update(ofPoint gaze){
    controls.update(gaze);
    selectArp.update(gaze);
    if(selectArp.selected==4){
        controls.selected=VOLUME;
        volume.update(gaze);
        volumeSlider.update(gaze);
            if(volume.changed){
                volumeSlider.setValue(volume.color);
                arpVolume=volume.value;
            }
            if(volumeSlider.changed){
                volume.setValueByColor(volumeSlider.value);
                arpVolume=volume.value;
            }
    }
    else{
        switch(controls.selected){
            case VOLUME:
                volume.update(gaze);
                volumeSlider.update(gaze);
                if(volume.changed){
                    volumeSlider.setValue(volume.color);
                    arpeggios[selectArp.selected].setVolume(volume.value);
                }
                if(volumeSlider.changed){
                    volume.setValueByColor(volumeSlider.value);
                    arpeggios[selectArp.selected].setVolume(volume.value);
                }
                break;
            case BIGSTEP:
                globalStep.update(gaze);
                globalStepSlider.update(gaze);
                if(globalStep.changed){
                    globalStepSlider.setValue(globalStep.value);
                    arpeggios[selectArp.selected].setGlobalStep(globalStep.value);
                    updateChords();
                }
                if(globalStepSlider.changed){
                    globalStep.setColorByValue(globalStepSlider.value);
                    arpeggios[selectArp.selected].setGlobalStep(globalStep.value);
                    updateChords();
                }
                break;
            case SMALLSTEP:
                patternStep.update(gaze);
                patternStepSlider.update(gaze);
                if(patternStep.changed){
                    patternStepSlider.setValue(patternStep.value);
                    arpeggios[selectArp.selected].setPatternStep(patternStep.value);
                    updateChords();
                }
                if(patternStepSlider.changed){
                    patternStep.setColorByValue(patternStepSlider.value);
                    arpeggios[selectArp.selected].setPatternStep(patternStep.value);
                    updateChords();
                }
                break;
            case PITCH:
                startingNote.update(gaze);
                startingNoteSlider.update(gaze);
                if(startingNote.changed){
                    startingNoteSlider.setValue(startingNote.value);
                    arpeggios[selectArp.selected].setStartingNote(startingNote.value);
                    updateChords();
                }
                if(startingNoteSlider.changed){
                    startingNote.setColorByValue(startingNoteSlider.value);
                    arpeggios[selectArp.selected].setStartingNote(startingNote.value);
                    updateChords();
                }
                break;
            case PATTERNSIZE:
                patternSize.update(gaze);
                patternSizeSlider.update(gaze);
                if(patternSize.changed){
                    patternSizeSlider.setValue(patternSize.value);
                    arpeggios[selectArp.selected].setPatternSize(patternSize.value);
                    updateChords();
                }
                if(patternSizeSlider.changed){
                    patternSize.setColorByValue(patternSizeSlider.value);
                    arpeggios[selectArp.selected].setPatternSize(patternSize.value);
                    updateChords();
                }
                break;
            case METER:
                meter.update(gaze);
                meterSlider.update(gaze);
                if(meter.changed){
                    meterSlider.setValue(meter.value);
                    arpeggios[selectArp.selected].setMeter(meter.value);
                    updateChords();
                }
                if(meterSlider.changed){
                    meter.setColorByValue(meterSlider.value);
                    arpeggios[selectArp.selected].setMeter(meter.value);
                    updateChords();
                }
                break;
            case NOTESINCLUDED:
                notesIncl.update(gaze);
                notesInclSlider.update(gaze);
                if(notesIncl.changed){
                    notesInclSlider.setValue(notesIncl.value);
                    arpeggios[selectArp.selected].setNotesIncl(notesIncl.value);
                    updateChords();
                }
                if(notesInclSlider.changed){
                    notesIncl.setColorByValue(notesInclSlider.value);
                    arpeggios[selectArp.selected].setNotesIncl(notesIncl.value);
                    updateChords();
                }
                break;
        }
    }
    if(selectArp.changed ){
        if( selectArp.selected==4){
            volume.setColorByValue(arpVolume);
            volumeSlider.setValue(volume.color);
        }
        else{
            volume.setColorByValue(arpeggios[selectArp.selected].getVolume());
            volumeSlider.setValue(volume.color);
            globalStep.setColorByValue(arpeggios[selectArp.selected].getGlobalStep());
            globalStepSlider.setValue(globalStep.value);
            patternStep.setColorByValue(arpeggios[selectArp.selected].getPatternStep());
            patternStepSlider.setValue(patternStep.value);
            startingNote.setColorByValue(arpeggios[selectArp.selected].getStartingNote());
            startingNoteSlider.setValue(startingNote.value);
            patternSize.setColorByValue(arpeggios[selectArp.selected].getPatternSize());
            patternSizeSlider.setValue(patternSize.value);
            meter.setColorByValue(arpeggios[selectArp.selected].getMeter());
            meterSlider.setValue(meter.value);
            notesIncl.setColorByValue(arpeggios[selectArp.selected].getNotesIncl());
            notesInclSlider.setValue(notesIncl.value);
        }
    }
}

void controlArp::updateChords(){
    for(int i=0;i<4;i++){
        arpeggios[i].changed=true;
        arpeggios[i].update();
    }
}

float controlArp::getSample(){
    curArpVol=0.9995f*curArpVol+0.0005f*arpVolume;
    float sample=0.0f;
    for(int i=0;i<4;i++)
        sample+=arpeggios[i].getSample();
    return sample*curArpVol;
}

void controlArp::updateTempo(){
    for(int i=0;i<4;i++){
        arpeggios[i].updateTempo();
    }
}

void controlArp::draw(){
    selectArp.draw();
    if(selectArp.selected!=4)    controls.draw();
    switch(controls.selected){
        case VOLUME:
            volume.draw();
            volumeSlider.draw();
            break;
        case BIGSTEP:
            globalStep.draw();
            globalStepSlider.draw();
            break;
        case SMALLSTEP:
            patternStep.draw();
            patternStepSlider.draw();
            break;
        case PITCH:
            startingNote.draw();
            startingNoteSlider.draw();
            break;
        case PATTERNSIZE:
            patternSize.draw();
            patternSizeSlider.draw();
            break;
        case METER:
            meter.draw();
            meterSlider.draw();
            break;
        case NOTESINCLUDED:
            notesIncl.draw();
            notesInclSlider.draw();
            break;
    }
}

void controlArp::resized(int w, int h){
    controls.resized(w,h);
    selectArp.resized(w,h);
    volume.resized(w,h);
    globalStep.resized(w,h);
    patternSize.resized(w,h);
    startingNote.resized(w,h);
    patternStep.resized(w,h);
    meter.resized(w,h);
    notesIncl.resized(w,h);
    volumeSlider.resized(w,h);
    globalStepSlider.resized(w,h);
    patternStepSlider.resized(w,h);
    startingNoteSlider.resized(w,h);
    patternSizeSlider.resized(w,h);
    meterSlider.resized(w,h);
    notesInclSlider.resized(w,h);
}
