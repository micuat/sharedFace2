//
//  ofxAppleRemote.h
//  
//
//  Created by kyriacos Kousoulides on 2/17/15.
//
// **** OBJ_C CODE BELONGS TO Martin Kahr ***
// **** OBJ_C CODE BELONGS TO Martin Kahr ***
// **** OBJ_C CODE BELONGS TO Martin Kahr ***
// **** OBJ_C CODE BELONGS TO Martin Kahr ***

#ifndef __kyriacos__ofxAppleRemote__
#define __kyriacos__ofxAppleRemote__

#include <iostream>
enum appleRemoteKey{
    appleRemoteKeyPlus				=1<<1,
	appleRemoteKeyMinus				=1<<2,
	appleRemoteKeyMenu				=1<<3,
	appleRemoteKeyPlay				=1<<4,
	appleRemoteKeyRight				=1<<5,
	appleRemoteKeyLeft				=1<<6,
	
	// hold events
	appleRemoteKeyPlus_Hold			=1<<7,
	appleRemoteKeyMinus_Hold		=1<<8,
	appleRemoteKeyMenu_Hold			=1<<9,
	appleRemoteKeyPlay_Hold			=1<<10,
	appleRemoteKeyRight_Hold		=1<<11,
	appleRemoteKeyLeft_Hold			=1<<12,
    appleRemoteKey_nil
};

class ofxAppleRemote{
    void * remoteHandl;
public:
    ofxAppleRemote();
    ~ofxAppleRemote();
    appleRemoteKey getKey();
    
};





#endif /* defined(__emptyExample__ofxAppleRemote__) */
