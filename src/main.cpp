#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
/***********************************************************************

THE EyeHarp: BY ZACHARIAS VAMVAKOUSIS
20/3/2011

***************************************************************************/

//========================================================================
int main( ){
	
//    ofAppGlutWindow window;
//	RECT desktop;
//	// Get a handle to the desktop window
//	const HWND hDesktop = GetDesktopWindow();
//	// Get the size of screen to the variable desktop
//	GetWindowRect(hDesktop, &desktop);
////	ofSetupOpenGL(&window, 1024,575, OF_WINDOW);			// <-------- setup the GL context
//	ofSetupOpenGL(&window,desktop.right,desktop.bottom*0.92 , OF_WINDOW);			// <-------- setup the GL context
//	printf("Screen resolution: %d x %d\n", ofGetScreenWidth(), ofGetScreenHeight());
//	// this kicks off the running of my app
//	// can be OF_WINDOW or OF_FULLSCREEN
//	// pass in width and height too:
//	ofRunApp( new ofApp());



	ofSetupOpenGL(1366, 768, OF_WINDOW);            // <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	//globalVars::AsioOn = false;

	ofRunApp(new ofApp());
}
