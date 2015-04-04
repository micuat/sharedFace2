#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxFaceTracker.h"
#include "ofxOsc.h"

#define SKIP_DRAW
#define IMAGE_LOG

#define PORT 57121

class ofApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void drawPointCloud();
	
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofxKinect kinect;

	bool bDrawPointCloud;
	
	// used for viewing the point cloud
	ofEasyCam easyCam;
	
	ofxFaceTracker tracker;
	cv::Mat colorImg, depthImg;
	ofMesh faceMesh;
	ofMatrix4x4 mat;
	ofMesh m0, mC;
	ofxCv::KalmanPosition kalmanPosition;
	ofxCv::KalmanEuler kalmanEuler;
	
	ofxOscSender sender;
	
	float lastSaved;
};
