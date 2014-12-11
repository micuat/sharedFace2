#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	soundStream.listDevices();
	soundStream.setDeviceID(5);
	soundStream.setup(this, 0, 1, 44100, 256, 4);
	
	if (rootDir.size() > 0) {
		init();
		pathLoaded = true;
	}
	else {
		pathLoaded = false;
	}

}

//--------------------------------------------------------------
void ofApp::init(){
	ofSetLogLevel(OF_LOG_WARNING);

	receiver.setup(PORT);

	cameraMode = EASYCAM_MODE;

	// load calibration parameters
	cv::FileStorage fs(ofToDataPath(rootDir[0] + "/config.yml"), cv::FileStorage::READ);
	fs["proWidth"] >> proSize.width;
	fs["proHeight"] >> proSize.height;

	float lensDist;
	cv::FileStorage cfs(ofToDataPath(rootDir[0] + "/calibration.yml"), cv::FileStorage::READ);
	cfs["proIntrinsic"] >> proIntrinsic;
	cfs["proExtrinsic"] >> proExtrinsic;

	cout << proIntrinsic << endl;
	cout << proExtrinsic << endl;

	// set parameters for projection
	proCalibration.setup(proIntrinsic, proSize);
	
	ofEnableDepthTest();

	faceMesh.setMode(OF_PRIMITIVE_TRIANGLES);
	for (int i = 0; i < NUM_POINTS; i++) {
		faceMesh.addVertex(ofVec3f());
	}

	for (int i = 0; i < sizeof(faceTriangles) / sizeof(faceTriangles[0]); i++) {
		faceMesh.addIndex(faceTriangles[i][0]);
		faceMesh.addIndex(faceTriangles[i][1]);
		faceMesh.addIndex(faceTriangles[i][2]);
	}
	faceAnimation.resize(FACE_ANIMATION_SIZE);
}

//--------------------------------------------------------------
void ofApp::update(){
	while (receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		if (m.getAddress() == "/face_mesh"){
			for (int i = 0; i < NUM_POINTS; i++) {
				float x = - m.getArgAsFloat(i * 3 + 2);
				float y = - m.getArgAsFloat(i * 3 + 3);
				float z = m.getArgAsFloat(i * 3 + 4);
				faceMesh.setVertex(i, ofVec3f(x, y, z));
			}
		}
		else if (m.getAddress() == "/face_animation") {
			for (int i = 0; i < FACE_ANIMATION_SIZE; i++) {
				faceAnimation.at(i) = m.getArgAsFloat(i + 2);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (pathLoaded) {

		float curTime = ofGetElapsedTimef();

		ofBackground(0);

		if (cameraMode == EASYCAM_MODE) {
			cam.begin();
			ofScale(1000, -1000, 1000);
			ofTranslate(0, 0, -2);
		}
		else if (cameraMode == PRO_MODE) {
			ofSetupScreenPerspective(proSize.width, proSize.height);
			proCalibration.loadProjectionMatrix(0.0001, 100000000.0);
			cv::Mat m = proExtrinsic;
			cv::Mat extrinsics = (cv::Mat1d(4, 4) <<
				m.at<double>(0, 0), m.at<double>(0, 1), m.at<double>(0, 2), m.at<double>(0, 3),
				m.at<double>(1, 0), m.at<double>(1, 1), m.at<double>(1, 2), m.at<double>(1, 3),
				m.at<double>(2, 0), m.at<double>(2, 1), m.at<double>(2, 2), m.at<double>(2, 3),
				0, 0, 0, 1);
			extrinsics = extrinsics.t();
			glMultMatrixd((GLdouble*)extrinsics.ptr(0, 0));
			ofScale(1000, 1000, 1000);
		}

		ofFloatColor c;
		c.setHsb(ofMap(faceAnimation.at(3), -1, 1, 0, 1), 1.0, 1.0);
		ofSetColor(255, 54);
		ofSetLineWidth(1);
		glPointSize(3);
		faceMesh.drawWireframe();

		/*
		int trIndex = floor(ofMap(ofNoise(ofGetElapsedTimef()), 0.0, 1.0, 0.0, (faceMesh.getNumIndices() / 3)));
		ofMesh m = faceMesh;
		m.clearIndices();
		m.addIndex(faceMesh.getIndex(trIndex * 3));
		m.addIndex(faceMesh.getIndex(trIndex * 3+1));
		m.addIndex(faceMesh.getIndex(trIndex * 3+2));
 		*/

		ofFill();
		ofSetColor(ofColor::blue);
		ofDrawSphere(faceMesh.getVertex(5), smoothedVol * 3 / 100);
		ofSetColor(ofColor::red);
		ofDrawSphere(faceMesh.getVertex(90), smoothedVol * 3 / 100);
		ofDrawSphere(faceMesh.getVertex(91), smoothedVol * 3 / 100);

		if (cameraMode == EASYCAM_MODE) {
			cam.end();
		}

		ofSetWindowTitle(ofToString(smoothedVol));
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (pathLoaded) {

		switch (key) {
		case '1': cameraMode = EASYCAM_MODE; break;
		case '2': cameraMode = PRO_MODE; break;
		case '3': cameraMode = CAM_MODE; break;
		}

	}

	if (key == 'f') {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	if (!pathLoaded) {
		for (int i = 0; i < dragInfo.files.size(); i++) {
			rootDir.push_back(dragInfo.files[i]);
		}
		init();
		pathLoaded = true;
	}
}

//--------------------------------------------------------------
void ofApp::audioIn(float *input, int bufferSize, int nChannels){
	float curVol = 0.0;
	for (int i = 0; i < bufferSize; i++) {
		curVol += input[i] * input[i] * 0.25;
	}
	curVol /= (float)bufferSize;
	curVol = sqrt(curVol);
	smoothedVol = curVol;
}
