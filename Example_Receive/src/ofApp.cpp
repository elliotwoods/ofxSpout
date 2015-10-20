#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Receiver");

	receiver.init();

	//optionally you can specify a channel name e.g.
	//receiver.init("Camera");
}
	
//--------------------------------------------------------------
void ofApp::update(){
	receiver.receive(texture);
}

//--------------------------------------------------------------
void ofApp::draw(){
	texture.draw(0, 0);
	ofDrawBitmapString("Channel : " + receiver.getChannelName(), 20, 20);
	ofDrawBitmapString("Press [SPACE] to select another source", 20, 40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		this->receiver.selectSenderPanel();
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
