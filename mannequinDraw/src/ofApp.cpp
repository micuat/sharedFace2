#include "ofApp.h"

/* Note on OS X, you must have this in the Run Script Build Phase of your project.
 where the first path ../../../addons/ofxLeapMotion2/ is the path to the ofxLeapMotion addon.
 
 cp -f ../../../addons/ofxLeapMotion2/libs/lib/osx/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libLeap.dylib"; install_name_tool -change ./libLeap.dylib @executable_path/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";
 
 If you don't have this you'll see an error in the console: dyld: Library not loaded: @loader_path/libLeap.dylib
 */

//--------------------------------------------------------------


void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_WARNING);
    
	leap.open();
    
	cam.setOrientation(ofPoint(-20, 0, 0));
    
	mesh.load(ofToDataPath("mesh.ply"));
	polylines.resize(3);

	sender.setup("localhost", PORT);
}


//--------------------------------------------------------------
void ofApp::update(){
	fingersFound.clear();
	
	//here is a simple example of getting the hands and drawing each finger and joint
	//the leap data is delivered in a threaded callback - so it can be easier to work with this copied hand data
	
	//if instead you want to get the data as it comes in then you can inherit ofxLeapMotion and implement the onFrame method.
	//there you can work with the frame data directly.
    
    
    
    //Option 1: Use the simple ofxLeapMotionSimpleHand - this gives you quick access to fingers and palms.
    
    
    simpleHands = leap.getSimpleHands();
    
    if( leap.isFrameNew() && simpleHands.size() ){
        
        //leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
		//leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        //leap.setMappingZ(-150, 150, -200, 200);
        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
        for(int i = 0; i < simpleHands.size(); i++){
            for (int f=0; f<5; f++) {
                int id = simpleHands[i].fingers[ fingerTypes[f] ].id;
                ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp; // metacarpal
                ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip; // proximal
                ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip; // distal
                ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip; // fingertip
                fingersFound.push_back(id);

				if(f >= 1 && f <= 1 ) {
//					if(tip.distance(simpleHands[i].handPos) > 100)
					ofVec3f mp = mcp - pip;
					ofVec3f td = tip - dip;
					
					int curIndex = polylines.size() / 3 - 1;
					polylines.at(curIndex * 3 + simpleHands[i].isLeft).addVertex(tip);
					ofxOscMessage m;
					m.setAddress("/sharedFace/canvas/leap/index/coord");
					m.addIntArg(curIndex * 3 + simpleHands[i].isLeft);
					m.addFloatArg(ofMap(tip.x, -75, 75, 345, 680));
					m.addFloatArg(ofMap(tip.y, 115, 295, 598, 124));
					m.addFloatArg(tip.z);
					if(simpleHands[i].isLeft) {
						m.addFloatArg(0.0f);
						m.addFloatArg(0.0f);
						m.addFloatArg(0.0f);
					} else {
						m.addFloatArg(1.0f);
						m.addFloatArg(1.0f);
						m.addFloatArg(1.0f);
					}
					m.addIntArg(i);
					sender.sendMessage(m);
					if(tip.z > -5) {
						if(polylines.at(curIndex * 3 + simpleHands[i].isLeft).size() > 0) {
							polylines.push_back(ofPolyline());
							polylines.push_back(ofPolyline());
							polylines.push_back(ofPolyline());
						}
					}
				}
            }
        }
    }
    
    
    
    //Option 2: Work with the leap data / sdk directly - gives you access to more properties than the simple approach
    //uncomment code below and comment the code above to use this approach. You can also inhereit ofxLeapMotion and get the data directly via the onFrame callback.
    /*
     vector <Hand> hands = leap.getLeapHands();
     if( leap.isFrameNew() && hands.size() ){
     
     //leap returns data in mm - lets set a mapping to our world space.
     //you can get back a mapped point by using ofxLeapMotion::getMappedofPoint with the Leap::Vector that tipPosition returns
     leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
     leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
     leap.setMappingZ(-150, 150, -200, 200);
     
     fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
     
     for(int i = 0; i < hands.size(); i++){
     for(int j = 0; j < 5; j++){
     ofPoint pt;
     
     const Finger & finger = hands[i].fingers()[ fingerTypes[j] ];
     
     //here we convert the Leap point to an ofPoint - with mapping of coordinates
     //if you just want the raw point - use ofxLeapMotion::getofPoint
     pt = leap.getMappedofPoint( finger.tipPosition() );
     pt = leap.getMappedofPoint( finger.jointPosition(finger.JOINT_DIP) );
     
     fingersFound.push_back(finger.id());
     }
     }
     }
     */
    
	//IMPORTANT! - tell ofxLeapMotion that the frame is no longer new.
	leap.markFrameAsOld();
}

//--------------------------------------------------------------
void ofApp::draw(){
#ifdef NO_WINDOW_APP
	return;
#endif
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	
    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
	
	ofSetColor(200);
	ofDrawBitmapString("ofxLeapMotion - Example App\nLeap Connected? " + ofToString(leap.isConnected()), 20, 20);
    
	cam.begin();
    
	ofPushMatrix();
    ofRotate(90, 0, 0, 1);
    ofSetColor(20);
    ofDrawGridPlane(800, 20, false);
	ofPopMatrix();
    
	ofSetLineWidth(2);
	ofSetColor(ofColor::pink);
	for( int i = 0 ; i < polylines.size(); i++ ) {
		polylines.at(i).draw();
	}
	
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    
    for(int i = 0; i < simpleHands.size(); i++){
        bool isLeft        = simpleHands[i].isLeft;
        ofPoint handPos    = simpleHands[i].handPos;
        ofPoint handNormal = simpleHands[i].handNormal;
        
        ofSetColor(0, 0, 255);
        ofDrawSphere(handPos.x, handPos.y, handPos.z, 10);
        ofSetColor(255, 255, 0);
        ofDrawArrow(handPos, handPos + 100*handNormal);
        
        for (int f=0; f<5; f++) {
            ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
            ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
            ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
            ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
            
            ofSetColor(0, 255, 0);
            ofDrawSphere(mcp.x, mcp.y, mcp.z, 6);
            ofDrawSphere(pip.x, pip.y, pip.z, 6);
            ofDrawSphere(dip.x, dip.y, dip.z, 6);
            ofDrawSphere(tip.x, tip.y, tip.z, 6);
            
            ofSetColor(255, 0, 0);
            ofSetLineWidth(5);
            ofLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
            ofLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
            ofLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);

			if(f == 1) {
				ofLogWarning() << tip;
			}
        }
    }
	ofSetLineWidth(1);
	ofSetColor(255, 255);
	ofScale(1, -1, 1);
	ofTranslate(0, -200, 0);
	mesh.drawWireframe();
	cam.end();

//	ofImage image;
//	leap.getImage(image, 0);
//	image.draw(0, 0);
//	leap.getImage(image, 1);
//	image.draw(0, image.getHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 'c') {
		polylines.clear();
		polylines.resize(3);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}
