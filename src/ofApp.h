#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "tobii.h"
#include "EyeHarp.h"
//#define DISPL 3
#define STH 0.025
#define SM 0.95
/***********************************************************************

THE MOUSEHARP: BY ZACHARIAS VAMVAKOUSIS
20/3/2011

***************************************************************************/
enum eyetracker { EYETRIBE, TOBII, MOUSE};
class ofApp : public ofBaseApp {

	public:
		//float ratiox, ratioy;
		ofApp();
		~ofApp();
		void setup();
		void update();
		void draw();
        void audioRequested 	(float * output, int bufferSize, int nChannels);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		/*static int notesPerScale,firstNote;
		static bool record_chords,alperMode;*/
		void exit();
        int mousex;
        int mousey;
		bool gaze;
		bool firstRead;
		//----- scenes
		tobii myTobii;
		EyeHarp HARP;
		bool sacadic;
		eyetracker tracker;
		ofPoint eyeSmoothed;
		ofSoundStreamSettings settings;
		vector<ofSoundDevice> soundDevices;
        ofSoundStream soundStream;
		int fixationSamples;
        int mouseDwell;
		void mySmooth();
		bool fixation();
		bool prFixation;
		ofPoint smooth;
		bool help;
		ofPoint *gbuffer;
		int bpos;
		ofPoint raw;
		int BS;
		ofPoint lastFixation;
		FILE * read;
		
		FILE * record;
		ofPoint avgNew;
		
};

#endif
