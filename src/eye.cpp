#include "eye.h"

enum{
    NOTESNUM,VIBWIDTH,VIBFREQ,RELEASE,ATTACK,GLISSANTO,VOLUME
};



void Eye::setup(int * chord, DistControl * Scale,bool* Conf,int noteNumber,bool tomidi,bool semitoneActive){
	conf=Conf;
    scale=Scale;
    width=ofGetWidth();width2=width/2.0;
    height=ofGetHeight();height2=height/2.0;
    XSsize=0.045f;
    Ssize=0.05;
    Msize=0.07;
    multiPos.x=-1.1f;
    multiPos.y=-.76f;
    sliderPos=ofPoint(-1.55,0);
    stepPosUP=ofPoint(-1.28,0.5);
    stepPosDW=ofPoint(-1.62,0.5);
    disc.setup(noteNumber,0.7,0.3,0.1,chord,false,conf,semitoneActive);
	char* scaleNames[7];
	scaleNames[0] = "I";
	scaleNames[1] = "II";
	scaleNames[2] = "III";
	scaleNames[3] = "IV";
	scaleNames[4] = "V";
	scaleNames[5] = "VI";
	scaleNames[6] = "VII";

   // harmonic[0].setup("Timbre",-3,ofPoint(-1.3f,-0.58f),ofPoint(-1.62f,-0.58f),-60,-1,Ssize,100,10,0.99,0.1,0.0,true);
//    harmonic[1].setup("",50,ofPoint(0.95,0.92),ofPoint(0.95,0.77),0.0,0.9,Ssize,30,5,0.4,0.4,0.8);
	for (int i = 0; i < 7; i++) {
		harmonic[i].setup("", -60, ofPoint(-1.28f, -0.86 + i*0.287), ofPoint(-1.64f, -0.86 + i*0.287), -60, -1, Ssize, 100, 10, 0.99, 0.1, 0.0, true);
	}
	if(variables::alperMode)
		octave.setup("8va", ofPoint(1.62, 0.82), ofPoint(1.3, 0.82), -1, 3, 1, 1, 0.07, 1000, 0.2, 0.2, 0, true);
	else
		octave.setup("8va", ofPoint(-1.6, 0.13), ofPoint(-1.6, -0.25), -1, 3, 1, 1, 0.09, 1000, 0.2, 0.2, 0, true);

    advanced.setup("Advanced",false,ofPoint(-0.8,-0.85),0.05f,1000,0.7,0.4,0,false);
    playArpeggio.setup("ArpeggioON",false,ofPoint(-1.05,-0.6),0.05f,1000,0.7,.4,0,false);
    oct=octave.value;
    char** names = new char*[4];
    for(int i=0;i<4;i++)
        names[i]=new char[20];
    strcpy(names[0],"Controls");
    strcpy(names[1],"Arpegio");
    strcpy(names[2],"Spectrum");
    strcpy(names[3],"Scale");
    multiplex.setup(4,names,0,ofPoint(-1.8,-0.85),0,0.05f,800);
    char* controlNames[7];
    controlNames[0]="NotesNum";
    controlNames[1]="VibWidth";
    controlNames[2]="VibFreq";
    controlNames[3]="Release";
    controlNames[4]="Attack";
    controlNames[5]="Glissanto";
    controlNames[6]="Volume";
    controls.setup(7,controlNames,6,multiPos,HALF_PI,0.05f,500);

    glissanto.setup("Gliss",247,stepPosUP,stepPosDW,0.98,0.9999,XSsize,300,5,0.6f,0.2f,0.0f);
    attack.setup("Atck",200,stepPosUP,stepPosDW,0.0001,0.005,XSsize,300,5,0.6f,0.2f,0.0f);
    release.setup("Rls",235,stepPosUP,stepPosDW,0.9995,0.9999,XSsize,300,5,0.6f,0.2f,0.0f);
    vibratoHz.setup("Vib cHz",700,stepPosUP,stepPosDW,50,1000,XSsize,300,5,0.6f,0.2f,0.0f,true);
    vibratoW.setup("VibW",150,stepPosUP,stepPosDW,0.0,3.5,XSsize,300,5,0.6f,0.2f,0.0f);
    //disc.NotesNumber.setup("NotesNum",stepPosUP,stepPosDW,7,36,noteNumber,1,0.085,800,0.6f,0.2f,0.0f);
    volume.setup("Vol",200,stepPosUP,stepPosDW,0.0,1.0,XSsize,500,20,0.6f,0.2f,0.0f);

    char* instrumentNames[4];
    instrumentNames[0]="Theremin";
    instrumentNames[1]="Sinusoidal";
    instrumentNames[2]="Vibraphone";
    instrumentNames[3]="ToMidi";
	if(tomidi)
		timbrePresets.setup(4,instrumentNames,3,ofPoint(-1.93,0.85),0,0.072f,1000);
	else
		timbrePresets.setup(4, instrumentNames, 2, ofPoint(-1.93, 0.85), 0, 0.072f, 1000);
	disc.percussive.value=true;
    

    notesNumSlider.setup(sliderPos,15, 7, 36, 0.7,false);
    vibratoWSlider.setup(sliderPos,128, 0, 255, 0.7,false);
    vibratoHzSlider.setup(sliderPos,vibratoHz.value, vibratoHz.min, vibratoHz.max,0.7,false);
    releaseSlider.setup(sliderPos,240, 0, 255, 0.7,false);
    attackSlider.setup(sliderPos,128, 0, 255, 0.7,false);
    glissantoSlider.setup(sliderPos,220, 0, 255, 0.7,false);
    volumeSlider.setup(sliderPos,200,0,255,0.7,false);
    vibratoPhaseAdder=(vibratoHz.value/100.0f / SAMPLERATE) * TWO_PI;

    setTimbrePreset();
	
	
}

void Eye::update(ofPoint Gaze, float* Velocity,bool *sacadic){
    velocity=Velocity;
    gaze=Gaze;
    disc.update(gaze, Velocity,sacadic);
 //   advanced.update(gaze);
//    arpInterface.updateChords();
   // octave.update(gaze);
	if(*conf){
		disc.NotesNumber.update(gaze);
		if (disc.NotesNumber.changed) {
			disc.resized(width, height);

		}
		octave.update(gaze);
		basicMode();
		
	}
	else if (variables::alperMode && variables::alperConfigureActive) {
		octave.update(gaze);
		disc.NotesNumber.update(gaze);
	}
    if(disc.changed){ oct=octave.value;
		//cout<<disc.note;
	}

}

void Eye::basicMode(){
    timbrePresets.update(gaze);
   // 
  //  playArpeggio.update(gaze);
    if(timbrePresets.changed){
        setTimbrePreset();
    }
    /*
    if(playArpeggio.value){
        for(int i=0;i<4;i++){
            arpInterface.arpVolume=0.5;
        }
    }
    else{
        for(int i=0;i<4;i++){
            arpInterface.arpVolume=0;
        }
    }*/
}

void Eye::setTimbrePreset(){
    switch(timbrePresets.selected){
            case 0:
                harmonic[0].setColorByValue(-1.5);
                harmonic[1].setColorByValue(-12.7);
                harmonic[2].setColorByValue(-15);
                harmonic[3].setColorByValue(-16.7);
                harmonic[4].setColorByValue(-18.5);
                harmonic[5].setColorByValue(-23);
                harmonic[6].setColorByValue(-25);
                glissanto.setValueByColor(253);
                glissantoSlider.setValue(glissanto.color);
                vibratoHz.setColorByValue(500);
                vibratoHzSlider.setValue(vibratoHz.value);
                vibratoPhaseAdder=(vibratoHz.value/100.0f / SAMPLERATE) * TWO_PI;
                vibratoW.setValueByColor(200);
                vibratoWSlider.setValue(vibratoW.color);
                attack.setValueByColor(1);
                attackSlider.setValue(attack.color);
                release.setValueByColor(230);
                releaseSlider.setValue(release.color);
                disc.percussive.value=false;
                break;
            case 1:
                harmonic[0].setColorByValue(-2);
                harmonic[1].setColorByValue(-60);
                harmonic[2].setColorByValue(-60);
                harmonic[3].setColorByValue(-60);
                harmonic[4].setColorByValue(-60);
                harmonic[5].setColorByValue(-60);
                harmonic[6].setColorByValue(-60);
                glissanto.setValueByColor(247);
                glissantoSlider.setValue(glissanto.color);
                vibratoHz.setColorByValue(700);
                vibratoHzSlider.setValue(vibratoHz.value);
                vibratoPhaseAdder=(vibratoHz.value/100.0f / SAMPLERATE) * TWO_PI;
                vibratoW.setValueByColor(150);
                vibratoWSlider.setValue(vibratoW.color);
                attack.setValueByColor(200);
                attackSlider.setValue(attack.color);
                release.setValueByColor(235);
                releaseSlider.setValue(release.color);
                disc.percussive.value=false;
                break;
            case 2:
                harmonic[0].setColorByValue(-1);
                harmonic[1].setColorByValue(-28);
                harmonic[2].setColorByValue(-19);
                harmonic[3].setColorByValue(-14);
                harmonic[4].setColorByValue(-30);
                harmonic[5].setColorByValue(-28);
                harmonic[6].setColorByValue(-35);
//                for(int i=0;i<7;i++)
//                    cout<<pow(10.0f,harmonic[i].value/10.0f)<<'\n';
                glissanto.setValueByColor(0);
                glissantoSlider.setValue(glissanto.color);
                vibratoHz.setColorByValue(0);
                vibratoHzSlider.setValue(vibratoHz.value);
                vibratoPhaseAdder=(vibratoHz.value/100.0f / SAMPLERATE) * TWO_PI;
                vibratoW.setValueByColor(0);
                vibratoWSlider.setValue(vibratoW.color);
                attack.setValueByColor(255);
                attackSlider.setValue(attack.color);
                release.setValueByColor(250);
                releaseSlider.setValue(release.color);
                disc.percussive.value=true;
                break;
            case 3:
				disc.percussive.value=false;
//                harmonic[0].setColorByValue(-6);
//                harmonic[1].setColorByValue(-23);
//                harmonic[2].setColorByValue(-10.5);
//                harmonic[3].setColorByValue(-23);
//                harmonic[4].setColorByValue(-12);
//                harmonic[5].setColorByValue(-23);
//                harmonic[6].setColorByValue(-8.6);
//                glissanto.setValueByColor(0);
//                glissantoSlider.setValue(glissanto.color);
//                vibratoHz.setColorByValue(500);
//                vibratoHzSlider.setValue(vibratoHz.value);
//                vibratoPhaseAdder=(vibratoHz.value/100.0f / SAMPLERATE) * TWO_PI;
//                vibratoW.setValueByColor(120);
//                vibratoWSlider.setValue(vibratoW.color);
//                attack.setValueByColor(230);
//                attackSlider.setValue(attack.color);
//                release.setValueByColor(150);
//                releaseSlider.setValue(release.color);
//                disc.percussive.value=false;
//                break;
        }
}

void Eye::draw(){
    disc.draw();
	if (*conf) {
		disc.NotesNumber.draw();
		timbrePresets.draw();
		octave.draw();

	}
	else if (variables::alperMode && variables::alperConfigureActive) {
		octave.draw();
		disc.NotesNumber.draw();
	}
	else
		disc.testSong.draw();
    //

    //advanced.draw();
    /*if(advanced.value){
    multiplex.draw();
    if(multiplex.selected==0){
        controls.draw();
        switch(controls.selected){
            case NOTESNUM:
                notesNumSlider.draw();
                disc.NotesNumber.draw();
                break;
            case VIBWIDTH:
                vibratoW.draw();
                vibratoWSlider.draw();
                break;
            case VIBFREQ:
                vibratoHz.draw();
                vibratoHzSlider.draw();
                break;
            case RELEASE:
                release.draw();
                releaseSlider.draw();
                break;
            case ATTACK:
                attack.draw();
                attackSlider.draw();
                break;
            case GLISSANTO:
                glissanto.draw();
                glissantoSlider.draw();
                break;
            case VOLUME:
                volume.draw();
                volumeSlider.draw();
                break;
        }
//        glissanto.draw();
//        attack.draw();
//        release.draw();
//        vibratoHz.draw();
//        vibratoW.draw();
    }
    if(multiplex.selected==1)    arpInterface.draw();
    if(multiplex.selected==2){
        for(int i=0;i<7;i++)
            harmonic[i].draw();
    }
//    if(multiplex.selected==4){
//        timbrePresets.draw();
//        musicalModes.draw();
//    }
    }
	else if(configure.value){
        timbrePresets.draw();
        //musicalModes.draw();
       // playArpeggio.draw();
    }*/
	//advanced.draw();
}

void Eye::resized(int w, int h){
    advanced.resized(w,h);
    multiplex.resized(w,h);
    playArpeggio.resized(w,h);
    controls.resized(w,h);
    timbrePresets.resized(w,h);
    notesNumSlider.resized(w,h);
    vibratoWSlider.resized(w,h);
    vibratoHzSlider.resized(w,h);
    releaseSlider.resized(w,h);
    attackSlider.resized(w,h);
    glissantoSlider.resized(w,h);
    volumeSlider.resized(w,h);
	disc.NotesNumber.resized(w, h);
    width=w;width2=w/2;
    height=h;height2=h/2;
    disc.resized(w,h);
    glissanto.resized(w,h);
    volume.resized(w,h);
    attack.resized(w,h);
    release.resized(w,h);
    vibratoHz.resized(w,h);
    vibratoW.resized(w,h);
	for (int i = 0; i < 7; i++) {
		harmonic[i].resized(w, h);
	}
    octave.resized(w,h);
    arpInterface.resized(w,h);
}
