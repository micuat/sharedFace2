//
//  ofxAppleRemote.mm
//  emptyExample
//
//  Created by kyriacos on 2/17/15.
//
//
// **** OBJ_C CODE BELONGS TO Martin Kahr ***
// **** OBJ_C CODE BELONGS TO Martin Kahr ***
// **** OBJ_C CODE BELONGS TO Martin Kahr ***


#include "ofxAppleRemote.h"
#import <Cocoa/Cocoa.h>
#include "AppleRemote.h"



ofxAppleRemote::    ofxAppleRemote(){
    AppleRemote * remoteControl;
    remoteControl = [AppleRemote new];
    remoteHandl = remoteControl;
    [remoteControl setOpenInExclusiveMode:YES];
    [remoteControl startListening:nil];
}
ofxAppleRemote::    ~ofxAppleRemote(){
    [(AppleRemote *)remoteHandl autorelease];
}

appleRemoteKey ofxAppleRemote::getKey(){

    appleRemoteKey result_ = appleRemoteKey_nil;

    if(BUTTON_REMOTE_PRESSED_KEY==NULL)return result_;

    
	if (floor(NSAppKitVersionNumber) <= NSAppKitVersionNumber10_4) {
		// 10.4.x Tiger
        if(strcmp("14_12_11_6_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyPlus;
        }else
        if(strcmp("14_13_11_6_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyMinus;
        }else
        if(strcmp("14_7_6_14_7_6_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyMenu;
        }else
        if(strcmp("14_8_6_14_8_6_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyPlay;
        }else
        if(strcmp("14_9_6_14_9_6_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyRight;
        }else
        if(strcmp("14_10_6_14_10_6_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyLeft;
        }else
        if(strcmp("14_6_4_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyRight_Hold;
        }else
        if(strcmp("14_6_3_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyLeft_Hold;
        }else
        if(strcmp("14_6_14_6_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyMenu_Hold;
        }else
        if(strcmp("18_14_6_18_14_6_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyPlay_Hold;
        }else{
            result_ = appleRemoteKey_nil;
        }

        
        
	} else if ((floor(NSAppKitVersionNumber) <= NSAppKitVersionNumber10_5)) {
		// 10.5.x Leopard
         
         if(strcmp("31_29_28_19_18_",BUTTON_REMOTE_PRESSED_KEY)==0){
         result_ =appleRemoteKeyPlus;
         }else
         if(strcmp("31_30_28_19_18_",BUTTON_REMOTE_PRESSED_KEY)==0){
         result_ =appleRemoteKeyMinus;
         }else
         if(strcmp("31_20_19_18_31_20_19_18_",BUTTON_REMOTE_PRESSED_KEY)==0){
         result_ =appleRemoteKeyMenu;
         }else
         if(strcmp("31_21_19_18_31_21_19_18_",BUTTON_REMOTE_PRESSED_KEY)==0){
         result_ =appleRemoteKeyPlay;
         }else
         if(strcmp("31_22_19_18_31_22_19_18_",BUTTON_REMOTE_PRESSED_KEY)==0){
         result_ =appleRemoteKeyRight;
         }else
         if(strcmp("31_23_19_18_31_23_19_18_",BUTTON_REMOTE_PRESSED_KEY)==0){
         result_ =appleRemoteKeyLeft;
         }else
         if(strcmp("31_19_18_4_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
         result_ =appleRemoteKeyRight_Hold;
         }else
         if(strcmp("31_19_18_3_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
         result_ =appleRemoteKeyLeft_Hold;
         }else
         if(strcmp("31_19_18_31_19_18_",BUTTON_REMOTE_PRESSED_KEY)==0){
         result_ =appleRemoteKeyMenu_Hold;
         }else
         if(strcmp("35_31_19_18_35_31_19_18_",BUTTON_REMOTE_PRESSED_KEY)==0){
         result_ =appleRemoteKeyPlay_Hold;
         }else{
             result_ = appleRemoteKey_nil;
         }

    } else {
		
        if(strcmp("33_31_30_21_20_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyPlus;
        }else
        if(strcmp("33_32_30_21_20_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyMinus;
        }else
        if(strcmp("33_22_21_20_2_33_22_21_20_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyMenu;
        }else
        if(strcmp("33_23_21_20_2_33_23_21_20_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyPlay;
        }else
        if(strcmp("33_24_21_20_2_33_24_21_20_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyRight;
        }else
        if(strcmp("33_25_21_20_2_33_25_21_20_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyLeft;
        }else
        if(strcmp("33_21_20_14_12_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyRight_Hold;
        }else
        if(strcmp("33_21_20_13_12_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyLeft_Hold;
        }else
        if(strcmp("33_21_20_2_33_21_20_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyMenu_Hold;
        }else
        if(strcmp("37_33_21_20_2_37_33_21_20_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyPlay_Hold;
        }else
        if(strcmp("33_21_20_8_2_33_21_20_8_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyPlay;
        }else
        if(strcmp("33_21_20_3_2_33_21_20_3_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyPlay;
        }else
        if(strcmp("33_21_20_11_2_33_21_20_11_2_",BUTTON_REMOTE_PRESSED_KEY)==0){
            result_ =appleRemoteKeyPlay_Hold;
        }else{
            result_ = appleRemoteKey_nil;
        }
	}
    return result_;
}