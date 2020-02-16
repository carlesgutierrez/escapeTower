#pragma once

#include "ofMain.h"
#include "characterClass.h"
#include "doorClass.h"


#ifdef _MAC
#include "ofxSyphon.h"
#else
//#include "ofxNDISender.h"
//#include "ofxNDISendStream.h"
#endif

#include "ofxMidi.h"

#define MIDIIN

class ofApp : public ofBaseApp, public ofxMidiListener{

	public:
		void setup();
		void resetGame();
		void update();
		void draw();

		void drawFboGame();

		void drawGame();

		void swapAllColors(int _id);


		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    	void exit();

		//Control Vars
		int levelGame = 0;

		///////////////////
		//Main Screen Vars
		//120x43
		float scaleChar = 5;// 10; // 1
		int heightFloor = 4 * scaleChar;
		int widthLedWindow = 1 * scaleChar;
		int numFloors = 35;
		int numWindowsPerFloor = 120; // 140;

		/*
		float scale = 1;
		float scaleCharacter = 5;
		int heightFloor = 20* scaleCharacter;
		int widthLedWindow = 10* scaleCharacter;
		int numFloors = 43* scale;
		int numWindowsPerFloor = 120* scale; // 140;
		*/

		/////////
		//Setup Screen
		int totalHeight = heightFloor * numFloors;
		int totalWidth = widthLedWindow * numWindowsPerFloor;

		///////////////////////////////////////////////
		//Characters
		vector<characterClass> myEnemies;
		vector<characterClass> myBonus;

		int numEnemies = 3;
		int numBonus = 2;

		int numActiveUsers = 1;
		characterClass mySnake;

		doorClass myDoor;
		
		///////////////////////////
		//GFX VARS
		//8 bits red, 8 bits green, 8 bits blue, from 0 to 255 in 256 steps
		ofColor myBKFadeColor;
		ofColor myBKFadeColor_Energy;
		ofColor myBKFadeColor_Normal;
		ofFbo rgbaFbo; // with alpha
		int fadeAmnt;
		bool bFadeAmount = true;

        ///////////////////////////////
        //// Conectivity
		//Syphon
		//ofxSyphonServer mainOutputSyphonServer;
		#ifdef _MAC
				ofxSyphonServer mainOutputSyphonServer;
		#endif

		#ifdef _MAC
			ofxSyphonServer mainOutputSyphonServer;
		#else
			//ofxNDISender sender_;
			////ofxNDISendVideo video_;
		#endif
    
        bool bDrawMidiIn = false;

#ifdef MIDIIN
    //MIDI
    void drawMidiIn();
    void setupMidi();
    void newMidiMessage(ofxMidiMessage& eventArgs);
    ofxMidiIn midiIn;
    std::vector<ofxMidiMessage> midiMessages;
    std::size_t maxMessages = 10; //< max number of messages to keep track of
#else

#endif

};
