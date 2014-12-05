#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
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

	faceMesh.setMode(OF_PRIMITIVE_POINTS);
	for (int i = 0; i < NUM_POINTS; i++) {
		faceMesh.addVertex(ofVec3f());
	}
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

		ofSetColor(255);
		glPointSize(3);
		faceMesh.draw();

		if (cameraMode == EASYCAM_MODE) {
			cam.end();
		}
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
