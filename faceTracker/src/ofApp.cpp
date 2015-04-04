#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetLogLevel(OF_LOG_WARNING);
	
	kinect.setRegistration(true);
    
	kinect.init();
	kinect.open();
	
	ofSetFrameRate(60);

	// start from the front
	bDrawPointCloud = false;
	
	tracker.setup();
	kalmanPosition.init(30000.0, 30000.0);
	kalmanEuler.init(30000.0, 30000.0);
	
	sender.setup("localhost", PORT);
	
	lastSaved = 0;
}

//--------------------------------------------------------------
void ofApp::update() {
	kinect.update();
	
	if(kinect.isFrameNew()) {
		using namespace cv;
		using namespace ofxCv;
		
#ifdef IMAGE_LOG
		if(ofGetElapsedTimef() - lastSaved > 30) {
			ofSaveImage(kinect.getPixelsRef(), ofToDataPath(ofGetTimestampString() + ".png"));
			lastSaved = ofGetElapsedTimef();
		}
#endif
		colorImg = toCv(kinect.getPixelsRef());
		cvtColor(colorImg, colorImg, CV_RGB2YCrCb);
		vector<Mat> planes;
		split(colorImg, planes);
		colorImg = planes.at(0);
		
		tracker.update(colorImg);
		
		if(!tracker.getFound()) return;
		depthImg = toCv(kinect.getDistancePixelsRef());
		
		threshold(depthImg, 200);
		dilate(depthImg);
		dilate(depthImg);
		
		m0.clear();
		mC.clear();
		
		ofMesh objectMesh = tracker.getObjectMesh();
		ofMesh imageMesh = tracker.getImageMesh();
		for(int i = 0; i < objectMesh.getNumVertices(); i++) {
			ofVec3f v = imageMesh.getVertex(i);
			ofVec3f p = kinect.getWorldCoordinateAt(v.x, v.y, depthImg.at<float>(v.y, v.x));
			
			if(p.z <= 0) continue;
			
			m0.addVertex(objectMesh.getVertex(i));
			mC.addVertex(p);
		}
		
		// find rigid transformation
		// http://nghiaho.com/?page_id=671
		int n = m0.getNumVertices();
		if(n < 3) return;
		ofVec3f c0 = m0.getCentroid();
		ofVec3f cC = mC.getCentroid();
		cv::Mat centroid0 = (cv::Mat1f(3, 1) << c0.x, c0.y, c0.z);
		cv::Mat centroidC = (cv::Mat1f(3, 1) << cC.x, cC.y, cC.z);
		
		float scale = 0.0f;
		cv::Mat H = cv::Mat_<float>::zeros(3, 3);
		for( int i = 0; i < n; i++ ) {
			cv::Mat p0 = (cv::Mat1f(3, 1) << m0.getVertex(i).x, m0.getVertex(i).y, m0.getVertex(i).z);
			cv::Mat pC = (cv::Mat1f(3, 1) << mC.getVertex(i).x, mC.getVertex(i).y, mC.getVertex(i).z);
			p0 = p0 - centroid0;
			pC = pC - centroidC;
			H = H + p0 * pC.t();
			
			scale += mC.getVertex(i).distance(cC) / m0.getVertex(i).distance(c0);
		}
		scale = scale / n;
		
		// predict Euler angle
		cv::SVD svd(H);
		cv::Mat R = svd.vt.t() * svd.u.t();
		mat.set(R.at<float>(0, 0), R.at<float>(1, 0), R.at<float>(2, 0), 0,
				R.at<float>(0, 1), R.at<float>(1, 1), R.at<float>(2, 1), 0,
				R.at<float>(0, 2), R.at<float>(1, 2), R.at<float>(2, 2), 0,
				0, 0, 0, 1);
		
		kalmanPosition.update(cC);
		cC = kalmanPosition.getEstimation();
		kalmanEuler.update(mat.getRotate());
		ofQuaternion qPredicted = kalmanEuler.getEstimation();
		mat.setRotate(qPredicted);
		
		mat.scale(scale, scale, scale);
		mat.setTranslation(cC);
		
		ofxOscMessage m;
		m.setAddress("/osceleton/face_mesh");
		m.addIntArg(0);
		m.addIntArg(0);
		
		faceMesh = objectMesh;
		ofMatrix4x4 mt = ofMatrix4x4::getTransposedOf(mat);
		for(int i = 0; i < objectMesh.getNumVertices(); i++) {
			ofVec4f v = objectMesh.getVertex(i);
			v.w = 1;
			v = mt * v;
			m.addFloatArg(-v.x / 1000);
			m.addFloatArg(-v.y / 1000);
			m.addFloatArg(v.z / 1000);
			
			ofVec3f uv = imageMesh.getVertex(i);
			ofVec3f p = kinect.getWorldCoordinateAt(uv.x, uv.y, depthImg.at<float>(uv.y, uv.x));
			
			if(p.z <= 0) {
				faceMesh.setVertex(i, v);
			} else {
				faceMesh.setVertex(i, p);
			}
		}
		sender.sendMessage(m);
		m.clear();
		
		m.setAddress("/osceleton/face");
		m.addIntArg(0);
		m.addIntArg(0);
		
		m.addFloatArg(-mat.getTranslation().x / 1000);
		m.addFloatArg(-mat.getTranslation().y / 1000);
		m.addFloatArg(mat.getTranslation().z / 1000);
		ofVec3f euler = mat.getRotate().getEuler();
		m.addFloatArg(euler.x);
		m.addFloatArg(euler.y);
		m.addFloatArg(-euler.z);
		sender.sendMessage(m);
	}
	
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	if(tracker.getFound())
		ofBackground(0, 255, 0);
	else
		ofBackground(255, 0, 0);
	
#ifdef SKIP_DRAW
	return;
#endif
	
	ofSetColor(255, 255, 255);
	
	if(bDrawPointCloud) {
		easyCam.begin();
		drawPointCloud();
		easyCam.end();
	} else {
		//ofxCv::drawMat(depthImg, 0, 0);
		ofxCv::drawMat(colorImg, 0, 0);
		ofSetColor(255, 255);
		tracker.getImageMesh().drawWireframe();
	}
	
	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
	
	reportStream << "fps: " << ofGetFrameRate() << endl;
	ofDrawBitmapString(reportStream.str(), 20, 652);
}

void ofApp::drawPointCloud() {
	ofBackground(50);
	int w = 640;
	int h = 480;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect.getDistanceAt(x, y) > 0) {
				mesh.addColor(kinect.getColorAt(x,y));
				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
			}
		}
	}
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards' 
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
	mesh.drawVertices();
//	glMultMatrixf((GLfloat*)mat.getPtr());
	ofPushStyle();
	ofSetColor(255, 128);
	faceMesh.draw();
	ofPopStyle();
	ofDisableDepthTest();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::exit() {
	kinect.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
	switch (key) {
		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{}
