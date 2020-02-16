#pragma once

#include "ofMain.h"


class doorClass {
private:
	ofColor myColor = ofColor::grey;
	ofColor myColorBorder = ofColor::green;

public:
	ofVec2f position;
	ofVec2f size;
	int energy = 0;
    float scaleDoor = 2;//2 or 1
	bool bRandomizedPos = false;

	////
	ofColor colorEnergy = ofColor::green;
	ofColor colorNormal = ofColor::grey;
	ofColor colorBorderPasive = ofColor::white;
	ofColor colorBorderActive = ofColor::red;
	
	ofColor colorStrobe = ofColor::yellow;

	int updateTimerPosition(int incMillis) {

		updatePositionTime = updatePositionTime - incMillis;
		int statusVelocity = 0;
		if (updatePositionTime < 300) { //TODO variable of DEFINE
			updatePositionTime = 300; //TODO variable of DEFINE
			statusVelocity = 1;
		}
		if (updatePositionTime > 600) { //TODO variable of DEFINE
			updatePositionTime = 600; //TODO variable of DEFINE
			statusVelocity = -1;
		}
		else statusVelocity = 0; //so it will keep trying to get faster

		return statusVelocity;
	}

private:

	float updatePositionTime = 600;
	float lastUpdatePositionTime = 0;
	bool bMaxVelocityActive = false;
	bool bMinVelocityActive = false;

	//
	int level = 0;

public:

	//-------------------------------------------------
	void setup(int _level) {
		
		position.x = ofRandom(100, ofGetWidth() - 200);
		position.y = ofRandom(100, ofGetHeight() - 100);
		size.x = ofRandom(40, 60);
		size.y = ofRandom(60, 70);
		level = _level;

		//update color
		myColorBorder = colorBorderPasive;
	}

	//-------------------------------------------------
	void update(float _energy4ThisDoor) {
		energy = _energy4ThisDoor; // Copy and save it

		if (ofGetElapsedTimeMillis() - lastUpdatePositionTime > updatePositionTime) {
			if(level > 0 && !bRandomizedPos)udpdateRandomPosition(); // update a new obligatory movement.. For now 1 by 1 right
			lastUpdatePositionTime = ofGetElapsedTimeMillis();
		}

		//TODO
		//Update Colision with Snake or Array of Snakes!! Send a *?!
		myColor = colorNormal.lerp(colorEnergy, energy);
	}

	//-------------------------------------------------
	void draw() {
	
		ofPushStyle();
		ofSetColor(myColor);
		//cout << "position.x = " << position.x << "position.x = " << position.x << "position.x = " << position.x << endl;
		//FILL
		ofFill();
		ofDrawRectangle(position.x, position.y, size.x*scaleDoor, size.y*scaleDoor);
		//NO FILL
		ofNoFill();
		ofSetColor(myColorBorder);
		ofSetLineWidth(4);//Depending on SIZE thicker contour...
		ofDrawRectangle(position.x, position.y, size.x*scaleDoor, size.y*scaleDoor);

		ofPopStyle();

	}

	//-------------------------------------------------
	void udpdateRandomPosition() {
		//Increment positions

		int randomX = int(ofRandom(200, ofGetWidth() - 500));
		int randomY = int(ofRandom(100, ofGetHeight() - 200));
		position.x = randomX;
		position.y = randomY;
		bRandomizedPos = true;
	}

	//---------------------------------------
	void increaseVelocity() {
		//one stair UP so try to increase vel
		int velStatus = updateTimerPosition(40); //TODO Scale it related to level status ?
		if (velStatus == 1) {
			cout << "already max velocity" << endl;
		}
		else {
			cout << "new timer -> updatePositionTime = " << updatePositionTime << endl;
		}
	}

	//---------------------------------------
	void decreaseVelocity() {
		//one stair UP so try to increase vel
		int velStatus = updateTimerPosition(-40); //TODO Scale it related to level status ?
		if (velStatus == 1) {
			cout << "already max velocity" << endl;
		}
		else { 
			cout << "new timer -> updatePositionTime = " << updatePositionTime << endl;
		}
	}

};
