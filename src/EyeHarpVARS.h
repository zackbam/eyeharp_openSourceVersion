#ifndef _EYEHARPVARS
#define _EYEHARPVARS

#define SAMPLERATE 44100
#define TUNING 523.251f
#define MIDICH1 1
#define MIDICH2 1
#define mag 1.5
#define magFac -2
//#define FIXVEL 70
#include<vector>
#include <ofTrueTypeFont.h>
typedef struct {
	int scaleNotes[7];
	int size;
	char name[50];
}sscale;
class variables {
public:
	static bool alperMode, record_chords, triggerChords;
	static int firstNote, notesPerScale, harmonize,windowH,windowW;
	static std::vector<sscale> presetScales;
	static int trNotesMidi[3];
	static bool alperConfigureActive;
	static ofTrueTypeFont myfont;
	static ofTrueTypeFont myfontPie;
	static int textColor;
	static int inReleaseFIXVEL;
	static float framerate;
};
#endif