#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	
    remote = new ofxAppleRemote;
	lastCount = 0;
	lastKey = appleRemoteKey_nil;
	
	sender.setup("localhost", PORT);
	ofBackground(255, 255, 255);
}

ofApp::~ofApp(){
    delete remote;
}

//--------------------------------------------------------------
void ofApp::update(){
	
	appleRemoteKey currentKey = remote->getKey();
	if( currentKey == appleRemoteKey_nil ) {
		if( ofGetFrameNum() - lastCount > 30 ) {
			ofBackground(255, 255, 255);
			lastCount = ofGetFrameNum();
			lastKey = currentKey;
		}
	} else {
		if( ofGetFrameNum() - lastCount < 4 ) {
			return;
		}
		bool twice = false;
		if( currentKey == lastKey ) {
			ofBackground(0, 255, 0);
			twice = true;
		}
		else ofBackground(255, 255, 255);

		lastCount = ofGetFrameNum();
		lastKey = currentKey;
		
		ofxOscMessage m;
		
		switch (lastKey) {
			case appleRemoteKeyPlus:
				m.setAddress("/sharedFace/canvas/remote/color");
				m.addFloatArg(1);
				m.addFloatArg(0);
				m.addFloatArg(0);
				break;
			case appleRemoteKeyMinus:
				m.setAddress("/sharedFace/canvas/remote/color");
				m.addFloatArg(0);
				m.addFloatArg(1);
				m.addFloatArg(0);
				break;
			case appleRemoteKeyRight:
				m.setAddress("/sharedFace/canvas/remote/color");
				m.addFloatArg(0);
				m.addFloatArg(0);
				m.addFloatArg(1);
				break;
			case appleRemoteKeyLeft:
				m.setAddress("/sharedFace/canvas/remote/color");
				m.addFloatArg(0);
				m.addFloatArg(0);
				m.addFloatArg(0);
				break;
			case appleRemoteKeyPlay:
				m.setAddress("/sharedFace/canvas/remote/mirror");
				m.addIntArg(0);
				break;
			case appleRemoteKeyPlay_Hold:
				m.setAddress("/sharedFace/canvas/remote/mirror");
				m.addIntArg(1);
				break;
			case appleRemoteKeyMenu_Hold:
				m.setAddress("/sharedFace/canvas/remote/clear");
				m.addIntArg(1);
				break;
			default: break;
		}
		sender.sendMessage(m);

	}
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(0);
    switch (lastKey) {
        case appleRemoteKey_nil:
            ofDrawBitmapString(".....", ofGetWidth()/2, ofGetHeight()/2);
            break;
        case appleRemoteKeyPlus:
            ofDrawBitmapString("appleRemoteKeyPlus", ofGetWidth()/2, ofGetHeight()/2);
            break;
        case appleRemoteKeyMinus:
            ofDrawBitmapString("appleRemoteKeyMinus", ofGetWidth()/2, ofGetHeight()/2);
            break;
        case appleRemoteKeyMenu:
            ofDrawBitmapString("appleRemoteKeyMenu", ofGetWidth()/2, ofGetHeight()/2);
            break;
        case appleRemoteKeyPlay:
            ofDrawBitmapString("appleRemoteKeyPlay", ofGetWidth()/2, ofGetHeight()/2);
            break;
        case appleRemoteKeyRight:
            ofDrawBitmapString("appleRemoteKeyRight", ofGetWidth()/2, ofGetHeight()/2);
            break;
        case appleRemoteKeyLeft:
            ofDrawBitmapString("appleRemoteKeyLeft", ofGetWidth()/2, ofGetHeight()/2);
            break;
        case appleRemoteKeyPlus_Hold:
            ofDrawBitmapString("appleRemoteKeyPlus_Hold", ofGetWidth()/2, ofGetHeight()/2);
            break;
        case appleRemoteKeyMinus_Hold:
            ofDrawBitmapString("appleRemoteKeyMinus_Hold", ofGetWidth()/2, ofGetHeight()/2);
            break;
        case appleRemoteKeyMenu_Hold:
            ofDrawBitmapString("appleRemoteKeyMenu_Hold", ofGetWidth()/2, ofGetHeight()/2);
            break;
        case appleRemoteKeyPlay_Hold:
            ofDrawBitmapString("appleRemoteKeyPlay_Hold", ofGetWidth()/2, ofGetHeight()/2);
            break;
        case appleRemoteKeyRight_Hold:
            ofDrawBitmapString("appleRemoteKeyRight_Hold", ofGetWidth()/2, ofGetHeight()/2);
            break;
        case appleRemoteKeyLeft_Hold:
            ofDrawBitmapString("appleRemoteKeyLeft_Hold", ofGetWidth()/2, ofGetHeight()/2);
            break;
        default:
            break;
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){


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
