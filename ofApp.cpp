#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 8 == 0) {

		vector<glm::vec3> tmp;
		tmp.push_back(glm::vec3());
		this->location_list.push_back(tmp);

		auto direction = glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
		direction = glm::normalize(direction) * 5;
		this->direction_list.push_back(direction);

		this->noise_seed_list.push_back(glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000)));
		this->color_list.push_back(ofColor(39));
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		auto angle_x = ofMap(ofNoise(this->noise_seed_list[i].x, ofGetFrameNum() * 0.01), 0, 1, -PI, PI);
		auto angle_y = ofMap(ofNoise(this->noise_seed_list[i].y, ofGetFrameNum() * 0.01), 0, 1, -PI, PI);
		auto angle_z = ofMap(ofNoise(this->noise_seed_list[i].z, ofGetFrameNum() * 0.01), 0, 1, -PI, PI);

		auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
		auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
		auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

		glm::vec3 tmp_direction = glm::vec4(this->direction_list[i], 0) * rotation_z * rotation_y * rotation_x;
		
		this->location_list[i].push_back(this->location_list[i].back() + tmp_direction);

		auto len = glm::length(this->location_list[i].back());
		this->color_list[i] = len < 200 ? 39 : ofMap(len, 200, 300, 39, 239);

		if (glm::length(this->location_list[i].back()) > 300) {

			this->location_list.erase(this->location_list.begin() + i);
			this->direction_list.erase(this->direction_list.begin() + i);
			this->noise_seed_list.erase(this->noise_seed_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}

		while (this->location_list[i].size() > 30) {

			this->location_list[i].erase(this->location_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.5);

	for (int i = 0; i < this->location_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		ofNoFill();
		ofBeginShape();
		ofVertices(this->location_list[i]);
		ofEndShape();

		ofFill();
		ofDrawSphere(this->location_list[i].back(), 3);
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}