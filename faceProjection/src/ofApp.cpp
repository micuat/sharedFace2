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

		rootDir.push_back("./");
		init();
		pathLoaded = true;
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
		faceMesh.addTexCoord(ofVec2f(faceUv[i][0], faceUv[i][1]));
	}

	for (int i = 0; i < sizeof(faceTriangles) / sizeof(faceTriangles[0]); i++) {
		faceMesh.addIndex(faceTriangles[i][0]);
		faceMesh.addIndex(faceTriangles[i][1]);
		faceMesh.addIndex(faceTriangles[i][2]);
	}
	faceAnimation.resize(FACE_ANIMATION_SIZE, 0);
	facePose.resize(FACE_POSE_SIZE, 0);

	faceFbo.allocate(1024, 768);

	stampPoints.resize(3);

	//toStoreMesh = true;
	toStoreMesh = false;

#define STRINGIFY(A) #A
	const char *src1v = STRINGIFY
	(
	uniform float dist;
	uniform vec2 ppoint;
	uniform float elapsedTime;
	void main(){

		gl_TexCoord[0] = gl_MultiTexCoord0;

		// projection as usual
		vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
		gl_Position = pos;

		// xy with principal point origin
		vec2 shiftPos = pos.xy - ppoint;

		// lens distortion
		gl_Position.xy = shiftPos * (1.0 / (1.0 - dist * length(shiftPos))) + ppoint;

		vec4 col = gl_Color;
		gl_FrontColor = col;
		gl_TexCoord[0] = gl_MultiTexCoord0;
	}
	);

	const char *src1f = STRINGIFY
	(
	uniform float elapsedTime;
	uniform sampler2DRect texture1;
	//generate a random value from four points
	vec4 rand(vec2 A, vec2 B, vec2 C, vec2 D){

		vec2 s = vec2(12.9898, 78.233);
		vec4 tmp = vec4(dot(A, s), dot(B, s), dot(C, s), dot(D, s));

		return fract(sin(tmp) * 43758.5453)* 2.0 - 1.0;
	}

	//this is similar to a perlin noise function
	float noise(vec2 coord, float d){

		vec2 C[4];

		float d1 = 1.0 / d;

		C[0] = floor(coord*d)*d1;

		C[1] = C[0] + vec2(d1, 0.0);

		C[2] = C[0] + vec2(d1, d1);

		C[3] = C[0] + vec2(0.0, d1);


		vec2 p = fract(coord*d);

		vec2 q = 1.0 - p;

		vec4 w = vec4(q.x*q.y, p.x*q.y, p.x*p.y, q.x*p.y);

		return dot(vec4(rand(C[0], C[1], C[2], C[3])), w);
	}

	vec3 rgb2hsv(vec3 c) {
		vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
		vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
		vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

		float d = q.x - min(q.w, q.y);
		float e = 1.0e-10;
		return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
	}

	vec3 hsv2rgb(vec3 c) {
		vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
		vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
		return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
	}
	void main() {
		vec2 pos = gl_TexCoord[0].st;

		vec4 col = texture2DRect(texture1, gl_TexCoord[0].st);
		gl_FragColor = col;
	}
	);

	shader.setupShaderFromSource(GL_VERTEX_SHADER, src1v);
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, src1f);
	shader.linkProgram();

	shader.begin();
	shader.setUniform1f("dist", 0);
	shader.end();
}

//--------------------------------------------------------------
void ofApp::update(){
	while (receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		if (m.getAddress() == "/osceleton/face_mesh"){
			
			for (int i = 0; i < NUM_POINTS; i++) {
				float x = - m.getArgAsFloat(i * 3 + 2);
				float y = - m.getArgAsFloat(i * 3 + 3);
				float z = m.getArgAsFloat(i * 3 + 4);
				faceMesh.setVertex(i, ofVec3f(x, y, z));
			}

			if (toStoreMesh) {
				toStoreMesh = false;
				faceMesh.save("faceMesh.ply");
			}
		}
		else if (m.getAddress() == "/osceleton/face_animation") {
			for (int i = 0; i < FACE_ANIMATION_SIZE; i++) {
				faceAnimation.at(i) = m.getArgAsFloat(i + 2);
			}
		}
		else if (m.getAddress() == "/osceleton/face") {
			for (int i = 0; i < FACE_POSE_SIZE; i++) {
				facePose.at(i) = m.getArgAsFloat(i + 2);
			}
		}
		else if (m.getAddress() == "/sharedFace/canvas/pen/coord") {
			int id = m.getArgAsInt32(0);
			float x = m.getArgAsFloat(1);
			float y = m.getArgAsFloat(2);
			float r = m.getArgAsFloat(3);
			float g = m.getArgAsFloat(4);
			float b = m.getArgAsFloat(5);
			while (id >(int)lines.size() - 1) {
				lines.push_back(ofMesh());
				lines.back().setMode(OF_PRIMITIVE_LINE_STRIP);
			}
			lines.at(id).addVertex(ofVec3f(x, y, 0));
			lines.at(id).addColor(ofFloatColor(r, g, b));
		}
		else if (m.getAddress() == "/sharedFace/canvas/stamp/coord") {
			int id = m.getArgAsInt32(0);
			float x = m.getArgAsFloat(1);
			float y = m.getArgAsFloat(2);
			stampPoints.at(id) = ofPoint(x, y);
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawPolygon(float xPct, float yPct){
	// from oF polygonExample
	int nTips = 5 + xPct * 60;
	int nStarPts = nTips * 2;
	float angleChangePerPt = TWO_PI / (float)nStarPts;
	float innerRadius = 0 + yPct * 60;
	float outerRadius = 60;
	float angle = 0;

	ofBeginShape();
	for (int i = 0; i < nStarPts; i++){
		if (i % 2 == 0) {
			// inside point:
			float x = innerRadius * cos(angle);
			float y = innerRadius * sin(angle);
			ofVertex(x, y);
		}
		else {
			// outside point
			float x = outerRadius * cos(angle);
			float y = outerRadius * sin(angle);
			ofVertex(x, y);
		}
		angle += angleChangePerPt;
	}
	ofEndShape();
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (pathLoaded) {
		faceFbo.begin();
		ofDisableDepthTest();
		ofBackground(0);
		ofSetColor(255);

		// flipped?
		ofTranslate(ofGetWidth() / 2.0f, 0, 0);
		ofScale(-1, 1, 1);
		ofTranslate(-ofGetWidth() / 2.0f, 0, 0);

		for (int i = 0; i < lines.size(); i++) {
			lines.at(i).draw();
		}
		ofEnableBlendMode(OF_BLENDMODE_SCREEN);
		for (int i = 0; i < stampPoints.size(); i++) {
			ofFill();
			ofPushMatrix();
			if (i == 0)
				ofSetColor(255, 0, 0);
			else if (i == 1)
				ofSetColor(0, 255, 0);
			else
				ofSetColor(0, 0, 255);
			
			ofTranslate(stampPoints.at(i));
			drawPolygon(ofMap(faceAnimation.at(3), 0, 1, 0, 0.3, true) , 0.6f);
			ofPopMatrix();
		}
		ofDisableBlendMode();
		
		faceFbo.end();

		ofEnableDepthTest();

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

			ofViewport(moveKey.x, moveKey.y);


			ofScale(1000, 1000, 1000);
		}

		ofSetColor(255, 255);
		ofSetLineWidth(1);
		glPointSize(3);


		shader.begin();
		shader.setUniform2f("ppoint", proIntrinsic.at<double>(0, 2) / ofGetWidth(), proIntrinsic.at<double>(1, 2) / ofGetHeight());
		shader.setUniform1f("elapsedTime", ofGetElapsedTimef());
		shader.setUniformTexture("texture1", faceFbo.getTextureReference(), 0);

		faceMesh.draw();

		shader.end();

		if (cameraMode == EASYCAM_MODE) {
			cam.end();
		}

		if (cameraMode == CAM_MODE) {
			ofDisableDepthTest();
			faceFbo.draw(0, 0);
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

	if (key == OF_KEY_UP) {
		moveKey.y -= 1;
	}
	if (key == OF_KEY_DOWN) {
		moveKey.y += 1;
	}
	if (key == OF_KEY_LEFT) {
		moveKey.x -= 1;
	}
	if (key == OF_KEY_RIGHT) {
		moveKey.x += 1;
	}
	ofLogWarning() << moveKey;
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
