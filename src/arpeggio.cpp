#include "arpeggio.h"

void arpeggio::setup(DistControl * Scale,int * Transpose,int * Chord,int * TotalSamples,int* CurSample, int* SampleRate,
float Volume,int GlobalStep, int PatternStep,int StartingNote,int PatternSize, int Meter, int NotesIncl){

    scale=Scale;
    chord=Chord;
    transpose=Transpose;
    totalSamples=TotalSamples;
    curSample=CurSample;
    sampleRate=SampleRate;
    changed=false;
    minGlobalStep=-4;
    maxGlobalStep=4;
    minPatternStep=-4;
    maxPatternStep=4;
    minStartingNote=0;
    maxStartingNote=40;
    minVolume=0.0f;
    maxVolume=.9f;
    minMeter=1;
    maxMeter=64;
    minPatternSize=1;
    maxPatternSize=32;
    maxNotesIncl=7;
    minNotesIncl=1;

    sequence=new int[maxMeter];
    targetFrequency=new float[maxMeter];
    phaseAdder=new float[maxMeter];
    nextPhaseAdder=new float[maxMeter];

    volume=Volume;
    globalStep=GlobalStep;
    patternStep=PatternStep;
    startingNote=StartingNote;
    patternSize=PatternSize;
    meter=Meter;
    notesIncl=NotesIncl;

    phase=0;
    noteSamples=*totalSamples/meter;
    changed=false;
    tvol=.05f;
    attackSamples=0.05**sampleRate;

    int NotesInclOrder[]={1,5,3,6,2,7,4};
    for(int i=0;i<7;i++)
        notesInclOrder[i]=NotesInclOrder[i];
    computeNotesIncl();
    curNote=0;
    prChord=*chord;//previous chord
    computeChord();
    generateArpeggio();
    computePhases();
}


void arpeggio::update(){
    if(prChord!=*chord){
        prChord=*chord;
        computeChord();
        changed=true;
    }
    if(changed){
        generateArpeggio();
        computePhases();
        changed=false;
    }
}

void arpeggio::setMeter(int Meter){
    meter=Meter;
    noteSamples=*totalSamples/meter;
    if(Meter>maxMeter)  meter=maxMeter;
    if(Meter<minMeter)  meter=minMeter;
    changed=true;
}

void arpeggio::setGlobalStep(int Step){
    globalStep=Step;
    if(Step>maxGlobalStep) globalStep=maxGlobalStep;
    if(Step<minGlobalStep) globalStep=minGlobalStep;
    changed=true;
}

void arpeggio::setPatternStep(int PatStep){
    patternStep=PatStep;
    if(PatStep>maxPatternStep) patternStep=maxPatternStep;
    if(PatStep<minPatternStep) patternStep=minPatternStep;
    changed=true;
}

void arpeggio::setStartingNote(int Note){
    startingNote=Note;
    if(Note>maxStartingNote) startingNote=maxStartingNote;
    if(Note<minStartingNote) startingNote=minStartingNote;
    changed=true;
}

void arpeggio::setPatternSize(int Size){
    patternSize=Size;
    if(Size>maxPatternSize) patternStep=maxPatternSize;
    if(Size<minPatternSize) patternStep=minPatternSize;
    changed=true;
}

void arpeggio::setVolume(float Vol){
    volume=Vol;
    if(Vol>maxVolume)   volume=maxVolume;
    if(Vol<minVolume)   volume=minVolume;
    changed=true;
}

void arpeggio::setNotesIncl(int num){
    notesIncl=num;
    if(num>maxNotesIncl)   notesIncl=maxNotesIncl;
    if(num<minNotesIncl)   notesIncl=minNotesIncl;
    computeNotesIncl();
    computeChord();
}

int arpeggio::getMeter(){
    return meter;
}
int arpeggio::getGlobalStep(){
    return globalStep;
}
int arpeggio::getPatternStep(){
    return patternStep;
}
int arpeggio::getStartingNote(){
    return startingNote;
}
int arpeggio::getPatternSize(){
    return patternSize;
}
float arpeggio::getVolume(){
    return volume;
}
int arpeggio::getNotesIncl(){
    return notesIncl;
}

void arpeggio::computeNotesIncl(){
    for(int i=0;i<=6;i++){
        if(notesInclOrder[i]<=notesIncl)   notesIncluded[i]=true;
        else                                    notesIncluded[i]=false;
    }
    changed=true;
}

void arpeggio::computeChord(){//always starting the arpeggio of the chord from the
    //closest note moving up from the starting note. That way the common notes
    //are kept the same between the chords. The chords are been played in played
    //in the appropriate position (1 3 5 or 1 6 4 or 1 6 3) automatically.
    int temp[7];
    for(int i=0;i<7;i++)    temp[(i+*chord)%7]=notesIncluded[i];
    for(int i=0;i<7;i++){
        ChordNotesIncluded[i]=temp[i];
    }
}

void arpeggio::computePhases(){
    int octave,note;
    for(int i=0;i<meter;i++){
            octave=floor(sequence[i]/7);
            note=scale[(sequence[i]+70)%7].value;
            targetFrequency[i] = (float)pow(2,(float)octave)*TUNING/8*(float)pow(2,(note+*transpose)/12.0f);
            nextPhaseAdder[i] = ((float)targetFrequency[i] / (float) *sampleRate) * 6.2831f;
    }
}

void arpeggio::updateTempo(){
    noteSamples=*totalSamples/meter;
}

void arpeggio::generateArpeggio(){
    changed=true;
    int nn=0;//notes put in the arpeggio so far
    int j=startingNote;
    int curStart=j;//beggining of current pattern
    if(ChordNotesIncluded[j%7]){
        sequence[nn++]=j;
    }
    j=nextArpeggioNote(j,patternStep);

    while(nn<meter){
        if(nn%patternSize==0){
            j=nextArpeggioNote(curStart,globalStep);
            curStart=j;
        }
        sequence[nn++]=j;
        j=nextArpeggioNote(j,patternStep);
    }
}

int arpeggio::nextArpeggioNote(int j, int step){
    int kn=0;
    if(step>0){
        while(kn<step){
            j++;
            while(!ChordNotesIncluded[(j+700)%7]){
                j++;
            }
            kn++;
        }
    }
    kn=0;
    if(step<0){
        while(kn>step){
            j--;
            while(!ChordNotesIncluded[(j+700)%7]){
                j--;
            }
            kn--;
        }
    }
    return j;
}



float arpeggio::getSample(){
    float sample;
    if(*curSample==0)   curNote=0;
    while (phase > 6.2831f){
        phase -= 6.2831f;
    }
    if(((*curSample)%noteSamples)<attackSamples) tvol=volume*0.001f+tvol*0.999f;
    else tvol=tvol*0.999f;
    phase += phaseAdder[curNote];
    sample=sin(phase) * tvol;
    if((*curSample)%noteSamples==0 &&  *curSample!=0){
        curNote=(curNote+1)%meter;
        for(int i=0;i<meter;i++)    phaseAdder[i]=nextPhaseAdder[i]; //never change the pitch untill a note finishes
    }
    return sample;
}





