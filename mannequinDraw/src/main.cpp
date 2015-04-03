#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"

//========================================================================
int main( ){
#ifdef NO_WINDOW_APP
	ofAppNoWindow window;
	ofSetupOpenGL(&window, 10, 10, OF_WINDOW);
	ofRunApp(new ofApp());
#else
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
#endif
}
