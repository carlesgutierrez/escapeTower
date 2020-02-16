#pragma once

#include "ofMain.h"

///////////////////////////////////////////////////////////
//Enum Characters Modes
enum characterMode {
	CHAR_ENEMY = 0, 
	CHAR_BONUS = 1, 
	CHAR_SNAKE = 2  
};

//enum MODE_COLORS {
//	MODE_COLOR_1 = 0,
//	MODE_COLOR_2 = 1,
//	MODE_COLOR_3 = 2
//};



class characterClass {
private:
	ofColor myColor = ofColor::blue;

public:
	int id = -1;
	ofVec2f position;
	ofVec2f size;

	//Bonus or Enemy Params
	int pointsDamage = 0;
	int pointsKilled = 0;

	//Snake Params
    float scaleSnake = 2;//2 or 1
	float scaleRegularCharacter = 4;
	int pointsEnergy = 0; // only for snake mode
	float minTime2UpdatePos = 0.025;
	int maxTime2UpdatePos = 300;

	////
	ofColor colorEnergy = ofColor::green;
	ofColor colorSnake = ofColor::cyan;
	ofColor colorEnemy = ofColor::red;

	int updateTimerPosition(int incMillis) {

		updatePositionTime = updatePositionTime - incMillis;
		int statusVelocity = 0;
		if (updatePositionTime < minTime2UpdatePos) { //TODO variable of DEFINE
			updatePositionTime = minTime2UpdatePos; //TODO variable of DEFINE
			statusVelocity = 1;
		}
		if (maxTime2UpdatePos > 300) { //TODO variable of DEFINE
			maxTime2UpdatePos = 300; //TODO variable of DEFINE
			statusVelocity = -1;
		}
		else statusVelocity = 0; //so it will keep trying to get faster

		return statusVelocity;
	}

private:
	characterMode type = CHAR_SNAKE; // CHAR_ENEMY, CHAR_BONUS, CHAR_NEUTRAL
	//MODE_COLORS typeColors = MODE_COLOR_1;
	int typeColors = 0;
	float updatePositionTime = 300;
	float lastUpdatePositionTime = 0;
	bool bMaxVelocityActive = false;
	bool bMinVelocityActive = false;
	//TODO global
	int numFloors = 0;
	int numWindowsPerFloor = 0;
	//Var control
	int statusGame = 0;
	int statusCharacter = 0;

public:
	characterClass() {
		//required setup
	}

	//-------------------------------------------------
	void setup(int id, int _x, int _y, int _w, int _h, int _pointsDamage, int _pointsKilled, int _numFloors, int _numWindowsPerFloor) {
		id = id;
		position.x = _x;
		position.y = _y;
		size.x = _w;
		size.y = _h;
		pointsDamage = _pointsDamage; // positive will be Bonus , Negative will be enemies
		pointsKilled = _pointsKilled;

		//Global?
		numWindowsPerFloor = _numWindowsPerFloor;
		numFloors = _numFloors;

		//Choose color 
		if (pointsDamage > 0) {
			myColor = colorEnergy;//ofColor colorEnergia ofColor::green;// lightGreen; // green
			type = CHAR_BONUS; // energia

		}
		else if (pointsDamage == 0) { // neutral character..
			myColor = colorSnake; // ofColor colorEnergia ofColor::cyan; //blue, blueSteel, darkBlue
			type = CHAR_SNAKE; // personaje
		}
		else {
			myColor = colorEnemy; //ofColor colorEnergia ofColor::red;
			type = CHAR_ENEMY; // enemigo
		}
	}

	//-------------------------------------------------
	void swapColors(int _id) {

		int idColor = -1;

		if (_id == 0) {
			typeColors++;
			if (typeColors > 3)typeColors = 1;
			idColor = typeColors;
		}
		else {
			idColor = _id;
		}

		cout << "idChar =" << id << " typeColors = " << typeColors  << endl;
		
		if (idColor == 1) {
			colorEnergy = ofColor::green;
			colorSnake = ofColor::cyan;
			colorEnemy = ofColor::red;
			
		}
		else if (idColor == 2) {
			colorEnergy = ofColor::lightSeaGreen;
			colorSnake = ofColor::cadetBlue;
			colorEnemy = ofColor::paleVioletRed;
		}
		else if (idColor == 3) {
			colorEnergy = ofColor::yellow;
			colorSnake = ofColor::violet;
			colorEnemy = ofColor::orangeRed;
		}

		//Update my Class colors
		if (type == CHAR_BONUS) {
			myColor = colorEnergy;//ofColor colorEnergia ofColor::green;// lightGreen; // green
			cout << "CHAR_BONUS " << type << " idChar =" << id << " myColor = " << myColor << endl;
		}
		else if (type == CHAR_SNAKE) { // neutral character..
			myColor = colorSnake; // ofColor colorEnergia ofColor::cyan; //blue, blueSteel, darkBlue
			cout << "CHAR_SNAKE =" << type << " idChar =" << id << " myColor = " << myColor << endl;
		}
		else if(type == CHAR_ENEMY){
			myColor = colorEnemy; //ofColor colorEnergia ofColor::red;
			cout << "CHAR_ENEMY =" << type << " idChar =" << id << " myColor = " << myColor << endl;
		}

	}

	//-------------------------------------------------
	void update() {

		//TODO
		if (type == CHAR_SNAKE) {
			//Update X position every X millis()

			if (ofGetElapsedTimeMillis() - lastUpdatePositionTime > updatePositionTime) {
				udpdatePosition(1, 0); // update a new obligatory movement.. For now 1 by 1 right
				lastUpdatePositionTime = ofGetElapsedTimeMillis();
				if (statusCharacter == 1) {
					cout << "statusCharacter 1. You win!!" << endl;
				}
			}
		}
	}

	////-------------------------------------------------
	bool update(ofRectangle _myRect) {
		bool bSnakeInsideDoor = false;

		//TODO
		//Update Colision with Snake or Array of Snakes!! Send a *?!
		ofRectangle auxCharacterRect = ofRectangle(position.x*size.x, position.y*size.y, size.x*scaleRegularCharacter, size.y*scaleRegularCharacter);
		if (auxCharacterRect.inside(_myRect) == true) { // if snake is inside de DOOR....
			bSnakeInsideDoor = true;
			cout << "Energy o bonus bSnakeInsideDoor !!! " << endl;
		}
		else {
		}

		return bSnakeInsideDoor;
	}

	//-------------------------------------------------
	void draw() {
		ofSetColor(myColor);

		//TODO special draw
		if (type == CHAR_ENEMY) {
            ofDrawRectangle(position.x *size.x, position.y *size.y, size.x*scaleRegularCharacter, size.y*scaleRegularCharacter);}
		else if (type == CHAR_BONUS) {
            ofDrawRectangle(position.x *size.x, position.y *size.y, size.x*scaleRegularCharacter, size.y*scaleRegularCharacter);
        }

		else if (type == CHAR_SNAKE) {
            ofDrawRectangle(position.x *size.x, position.y *size.y, size.x*scaleSnake, size.y*scaleSnake);
        }

		
        
	}

	//-------------------------------------------------
	void udpdatePosition(int incX, int incY) {
		//Increment positions
		position.x = position.x + incX;
		position.y = position.y + incY;

		if (position.x > numWindowsPerFloor) {
			position.x = 0;
			movePosUpByTheEnd();
		}
		else if(position.x < 0) {
			position.x = numWindowsPerFloor;
		}
	}

	//---------------------------------------
	void increaseVelocity() {
		//one stair UP so try to increase vel
		int velStatus = updateTimerPosition(40); //TODO Scale it related to level status ?
		if (velStatus == 1) {
			//cout << "already max velocity" << endl;
		}
		else {
			//cout << "new timer -> updatePositionTime = " << updatePositionTime << endl;
		}
	}

	//---------------------------------------
	void decreaseVelocity() {
		//one stair UP so try to increase vel
		int velStatus = updateTimerPosition(-40); //TODO Scale it related to level status ?
		if (velStatus == 1) {
			//cout << "already max velocity" << endl;
		}
		else { 
			//cout << "new timer -> updatePositionTime = " << updatePositionTime << endl;
		}
	}

	//------------------------------------
	void movePosUpByTheFace() {
		position.y++;
		if (position.y > numFloors - 1 * scaleSnake)position.y = numFloors - 1 * scaleSnake;
		//TODO check colisions
	}
	//------------------------------------
	void movePosDownByTheFace() {
		position.y--;
		if (position.y < 0)position.y = 0;
		//TODO check colisions
	}
	//------------------------------------
	void movePosUpByTheEnd() {
		position.y++;
		if (position.y > numFloors - 1* scaleSnake)position.y = 0;//numFloors-1;
		if (position.y < 0)position.y = 0;//NOT necesary but...

		if (position.y > numFloors) { // & bReadyForWIN
			cout << "You win the TOP!" << endl;
			statusCharacter = 1;
			//TODO Create a STATUS character: waiting, playing, dying, Winner. 
			//TODO create Machine status from one stage to another
		}
		else {
			increaseVelocity();
		}
	}
};
