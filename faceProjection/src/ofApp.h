/*
https://github.com/micuat/sharedFace

Naoto Hieda <micuat@gmail.com> 2014
*/

#pragma once

#include "ofMain.h"

#include "ofxCv.h"
#include "ofxOsc.h"

#define PORT 7110
#define NUM_POINTS 121
#define RES_MULT 2

class ofApp : public ofBaseApp {
public:
	void setup();
	void init();
	void update();
	void draw();
	void keyPressed(int);
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void dragEvent(ofDragInfo);

	vector<string> rootDir;

private:
	int projector_width, projector_height;
	float projector_horizontal_center;
	ofMesh faceMesh;
	ofEasyCam cam;
	cv::Mat proIntrinsic, proExtrinsic;
	ofxCv::Intrinsics proCalibration;
	cv::Size proSize;

	enum CameraMode { EASYCAM_MODE, PRO_MODE, CAM_MODE };
	CameraMode cameraMode;

	ofxOscReceiver receiver;

	bool pathLoaded;
};