#include "ofApp.h"

int main() {
#ifdef SKIP_DRAW
    ofSetupOpenGL(120, 30, OF_WINDOW);
#else
	ofSetupOpenGL(1024, 768, OF_WINDOW);
#endif
	ofRunApp(new ofApp());
}
