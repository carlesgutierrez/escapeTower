#include "ofApp.h"

#ifdef MIDIIN
//-----------------------------------------------------
void ofApp::setupMidi() {
	// print input ports to console
	midiIn.listInPorts();
	/*
	// open port by number (you may need to change this)
	midiIn.openPort(1);
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port

	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);

	// add ofApp as a listener
	midiIn.addListener(this);

	// print received messages to the console
	midiIn.setVerbose(true);
	*/
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {

	// add the latest message to the message queue
	midiMessages.push_back(msg);

	// remove any old messages if we have too many
	while (midiMessages.size() > maxMessages) {
		midiMessages.erase(midiMessages.begin());
	}
}


//--------------------------------------------------------------
void ofApp::drawMidiIn() {
	for (unsigned int i = 0; i < midiMessages.size(); ++i) {

		ofxMidiMessage &message = midiMessages[i];
		int x = 10;
		int y = i * 40 + 40;

		// draw the last recieved message contents to the screen,
		// this doesn't print all the data from every status type
		// but you should get the general idea
		stringstream text;
		text << ofxMidiMessage::getStatusString(message.status);
		while (text.str().length() < 16) { // pad status width
			text << " ";
		}

		ofSetColor(127);
		if (message.status < MIDI_SYSEX) {
			text << "chan: " << message.channel;
			if (message.status == MIDI_CONTROL_CHANGE) {
				text << "\tctl: " << message.control;
				ofDrawRectangle(x + ofGetWidth()*0.2, y + 12,
					ofMap(message.control, 0, 127, 0, ofGetWidth()*0.2), 10);
			}
			else if (message.status == MIDI_PITCH_BEND) {
				text << "\tval: " << message.value;
				ofDrawRectangle(x + ofGetWidth()*0.2, y + 12,
					ofMap(message.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()*0.2), 10);
			}
			else {
				text << "\tpitch: " << message.pitch;
				ofDrawRectangle(x + ofGetWidth()*0.2, y + 12,
					ofMap(message.pitch, 0, 127, 0, ofGetWidth()*0.2), 10);

				text << "\tvel: " << message.velocity;
				ofDrawRectangle(x + (ofGetWidth()*0.2 * 2), y + 12,
					ofMap(message.velocity, 0, 127, 0, ofGetWidth()*0.2), 10);
			}
			text << " "; // pad for delta print
		}

		text << "delta: " << message.deltatime;
		ofSetColor(0);
		ofDrawBitmapString(text.str(), x, y);
		text.str(""); // clear
	}
}


#else

#endif


//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowShape(totalWidth, totalHeight);
	ofSetFrameRate(120);

	//allocate our fbos.
	//providing the dimensions and the format for the,
	rgbaFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA); // with alpha, 8 bits red, 8 bits green, 8 bits blue, 8 bits alpha, from 0 to 255 in 256 steps

															//myEnemies.resize(numEnemies); // allocate space for 100 ints in vector
															//myBonus.resize(numBonus);

	for (int i = 0; i < numEnemies; i++) {
		//myEnemies
		myEnemies.push_back(characterClass());
		myEnemies[i].setup(i, int(ofRandom(0, numWindowsPerFloor)), int(ofRandom(0, numFloors)), widthLedWindow, heightFloor, -3, 1, numFloors, numWindowsPerFloor);
		cout << "Enemies => Pos X =" << myEnemies[i].position.x << " Pos Y =" << myEnemies[i].position.y << endl;
	}

	for (int i = 0; i < numBonus; i++) {
		//myBonuts
		myBonus.push_back(characterClass());
		//TODO Check Filled Spaces First... If Ocupied then Change to another location. In godot That might be setup fixed positions per level
		myBonus[i].setup(i, int(ofRandom(0, numWindowsPerFloor)), int(ofRandom(0, numFloors)), widthLedWindow, heightFloor, 3, 0, numFloors, numWindowsPerFloor);
		cout << "myBonus => Pos X =" << myBonus[i].position.x << " Pos Y =" << myBonus[i].position.y << endl;

	}

	//Main Character
	mySnake.setup(0, 1, 1, widthLedWindow, heightFloor, 0, 0, numFloors, numWindowsPerFloor);
	myDoor.setup(levelGame);

	//FBO Clears
	myBKFadeColor = myBKFadeColor_Normal;

	fadeAmnt = 15;
	rgbaFbo.begin();
	ofClear(255, 255, 255, 0);
	rgbaFbo.end();

	//Syphon
#ifdef _MAC
	mainOutputSyphonServer.setName("EscapeTower Spyhon Output");
#else
	//if (sender_.setup("ofxNDISender example")) {
	//	video_.setup(sender_);
	//	video_.setAsync(true);
	//}
#endif

#ifdef MIDIIN
	setupMidi();
#else

#endif

}

//--------------------------------------------------------------
void ofApp::update() {

	/////////////////////////////
	//Particles
	mySnake.update();

	//TODO get the energy from all snakes
	float energyAux = mySnake.pointsEnergy;
	//TODO map color interaction....from one color to another... 
	float energyMap = ofMap(energyAux, 0, 10 * numActiveUsers, 0, 1);

	myDoor.update(energyMap);

	///////////////////////////////
	//GAME
	if (bFadeAmount) {
		rgbaFbo.begin();
		drawFboGame();
		rgbaFbo.end();
	}

	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

}

//--------------------------------------------------------------
void ofApp::draw() {
	if (bDrawMidiIn) {
		ofBackground(ofColor::white);

#ifdef MIDIIN
		drawMidiIn();
#else

#endif

	}
	else {
		ofBackground(ofColor::black);

		if (bFadeAmount) {
			ofSetColor(255, 255, 255);
			rgbaFbo.draw(0, 0);
		}
		else {
			drawGame();
		}

#ifdef _MAC
		mainOutputSyphonServer.publishScreen();
#else

#endif

	}


}

//-------------------------------------------------
void ofApp::drawFboGame() {

	//we clear the fbo if c is pressed.
	//this completely clears the buffer so you won't see any trails
	if (ofGetKeyPressed('c')) {
		ofClear(255, 255, 255, 0);
	}

	//1 - Fade Fbo

	//this is where we fade the fbo
	//by drawing a rectangle the size of the fbo with a small alpha value, we can slowly fade the current contents of the fbo.
	ofFill();
	ofSetColor(myBKFadeColor, fadeAmnt);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	//2 - Draw graphics
	drawGame();
}

//--------------------------------------------------------------
void ofApp::drawGame() {

	ofSetColor(255);

	for (int i = 0; i < myEnemies.size(); i++) {
		myEnemies[i].draw();
	}

	for (int i = 0; i < myBonus.size(); i++) {
		myBonus[i].draw();
	}


	myDoor.draw();
	mySnake.draw();
}

//------------------------------------------------------------
void ofApp::swapAllColors(int idCol) {

	for (int i = 0; i < myEnemies.size(); i++) {
		myEnemies[i].swapColors(idCol);
	}

	for (int i = 0; i < myBonus.size(); i++) {
		myBonus[i].swapColors(idCol);
	}

	mySnake.swapColors(idCol);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == OF_KEY_DOWN) {
		//mySnake.movePosUpByTheFace();
	}
	switch (key)
	{
	case OF_KEY_DOWN:
		mySnake.movePosUpByTheFace();
		break;
	case OF_KEY_UP:
		mySnake.movePosDownByTheFace();
		break;
	case OF_KEY_RIGHT:
		mySnake.increaseVelocity();
		break;
	case OF_KEY_LEFT:
		mySnake.decreaseVelocity();
		break;
	case '-':
		mySnake.pointsEnergy--;
		if (mySnake.pointsEnergy < 0) mySnake.pointsEnergy = 0;
		break;
	case '+':
		mySnake.pointsEnergy++;
		if (mySnake.pointsEnergy > 10) mySnake.pointsEnergy = 10;
		break;
	case 'c':
		swapAllColors(0);
		break;
	case 'f':
		bFadeAmount = !bFadeAmount;
		break;
	case '1':
		swapAllColors(1);
		break;
	case '2':
		swapAllColors(2);
		break;
	case '3':
		swapAllColors(3);
		break;
	case 'q':
		fadeAmnt = 5;
		cout << "fadeAmnt 1= " << fadeAmnt << endl;
		break;
	case 'w':
		fadeAmnt = 10;
		cout << "fadeAmnt 1= " << fadeAmnt << endl;
		break;
	case 'e':
		fadeAmnt = 15;
		cout << "fadeAmnt 1= " << fadeAmnt << endl;
		break;
	case 'm':
		bDrawMidiIn = !bDrawMidiIn;
		break;
	case 'd':
		myDoor.udpdateRandomPosition();
		break;
	case 'M':
#ifdef MIDIIN
		midiIn.listInPorts();
#endif
		break;
	default:
		break;
	}
	/*
	if (key =='1') {
	fadeAmnt = 2;
	cout << "fadeAmnt 1= " << fadeAmnt << endl;
	}
	else if (ofGetKeyPressed('2')) {
	fadeAmnt = 5;
	cout << "fadeAmnt 2= " << fadeAmnt << endl;
	}
	else if (ofGetKeyPressed('3')) {
	fadeAmnt = 15;
	cout << "fadeAmnt 3= " << fadeAmnt << endl;
	}
	*/
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//--------------------------------------------------------------
void ofApp::exit() {

#ifdef MIDIIN
	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
#endif

}
