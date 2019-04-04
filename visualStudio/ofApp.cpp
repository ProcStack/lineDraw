#include "ofApp.h"
#include <iostream>

//float PI = 3.14159265358979;
const int lineCountBase = 10000;
int lineCount = lineCountBase;
int lineCountPrev = 0;
float runner = 0;
float absRunner = 0;
float curTimer = 0;
int sW = 500;
int sH = 500;
int mouseX = 0;
int mouseY = 0;
float mouseXNorm = 0;
float mouseYNorm = 0;
float caps = 0;

//float xPosArr[1421] = {};
//float yPosArr[1421] = {};
float xPosArr[lineCountBase][2] = {};
float yPosArr[lineCountBase][2] = {};


//--------------------------------------------------------------
void ofApp::setup() {
	sW = ofGetWidth();
	sH = ofGetHeight();
	runner = 0;
	mouseXNorm = sW * .5;
	mouseYNorm = sH * .5;

	//lineCount = 378;
	//lineCount = 1421;
}

//--------------------------------------------------------------
void ofApp::update() {
	runner += (ofGetMouseX() / float(sW) - .5)*.65;
	absRunner += 1;

	caps = min(1., max(0., cos(absRunner*.01) * 3 + .5));
	curTimer += (ofGetMouseX() / float(sW) - .5)*.65*(1 - caps) + .2 * caps;
}

//--------------------------------------------------------------

void delta(float *arr, float px1, float py1, float px2, float py2) {
	float distX = px2 - px1;
	float distY = py2 - py1;
	arr[0] = distX;
	arr[1] = distY;
}

float distance(float p1[2], float p2[2]) {
	float distX = p2[0] - p1[0];
	float distY = p2[1] - p1[1];
	float ret = sqrt(distX*distX + distY * distY);
	return ret;
}
float distance(float px1, float py1, float px2, float py2) {
	float distX = px2 - px1;
	float distY = py2 - py1;
	float ret = sqrt(distX*distX + distY * distY);
	return ret;
}
float distance(float px, float py) {
	float ret = sqrt(px*px + py * py);
	return ret;
}
/*float distance(float pos[2]) {
	float ret = sqrt(pos[0]* pos[0] + pos[1] * pos[1]);
	return ret;
}*/
void normalize(float *arr) {
	float dist = distance(arr[0], arr[1]);
	float distX = arr[0] / dist;
	float distY = arr[1] / dist;
	arr[0] = distX;
	arr[1] = distY;
}
float sign(float val) {
	float ret = float((0 < val) - (val < 0));
	if (ret == 0) {
		ret = 1;
	}
	return ret;
}

void ofApp::draw() {
	ofBackgroundGradient(ofColor(0, 0, 0), ofColor(10, 19, 31), OF_GRADIENT_CIRCULAR);
	ofSetColor(255, 0, 0);

	int curX = sW * .5;
	int curY = sH * .5;
	float swMult = .4;
	float xMult = PI * 2 * (5 + 40 * min(0., max(1., cos(runner*2.) * 3 + 2)));

	float runStop = (float)lineCount;
	caps = min(1., max(0., cos(absRunner*.01) * 3 + .5));

	for (int x = 0; x < runStop; ++x) {
		float rate = ((float)x / (float(runStop) - 1));
		float fadeMult = 5;
		double blend = min(float(rate*fadeMult*.5), float(fadeMult - (rate*fadeMult)));
		blend = max(0.0, min(1.0, blend));
		fadeMult = 50;
		double blendEnd = min(rate*fadeMult, float(fadeMult - (rate*fadeMult)));
		blendEnd = max(0.0, min(1.0, blendEnd));

		float mouseBlend = 1 - (abs(mouseXNorm - .5) / .5);

		float mouseXSign = float((0 < (mouseXNorm - .5)) - ((mouseXNorm - .5) < 0));
		if (mouseXSign == 0) {
			mouseXSign = 1;
		}
		float freqTwo = sin(float(x)*.004 + runner * swMult*.1*min(1., max(-1., (mouseXNorm - .5)*2.))) * 1 * caps;
		int toX = (int)(((sin(rate*xMult*.5 + runner * swMult + cos(rate + .1) + mouseXNorm) * (mouseXNorm * 2 - 1) + (1. - mouseXNorm * .5))*(1 - caps) + freqTwo) * ((float)sW*sin(rate*xMult + runner * swMult*.7)*.3 + mouseXNorm * .1) + sW * .5);
		int toY = (int)((cos(rate*xMult + curTimer * swMult*.7 + (mouseYNorm - .5) * 3) + sin(rate*PI + PI) + sin(mouseXNorm*PI) + cos(mouseYNorm*PI))  * (float)sH*.5 * sin(rate + float(x + 15.15)*.001*(.1*(mouseYNorm) * 4 + .7))*.4 + (float)sH*.5);

		if (x < lineCountPrev) {
			float xyPush[2] = {};
			delta(xyPush, toX, toY, sW*.5, sH*.5);
			float dist = distance(xyPush[0], xyPush[1]);
			normalize(xyPush);
			dist = max(0., min(1., dist / sqrt(sW*sW + sH * sH)));

			/*if (x == int(runStop*.9)) {
				ofSetColor(255, 0, 0);
				ofDrawBitmapString(ofToString(x) + " -- " + ofToString(dist) + " -- " + ofToString(xyPush[0]) + " -- " + ofToString(xyPush[1]) + " -- " + ofToString(toX) + " -- " + ofToString(sW*.5), 20, 20);
			}*/

			float R = 255 * (1 - abs(sin(rate*xMult + runner * swMult*.5))) * (1 - caps) + 5 * caps;
			float G = 80 * (1 - min(1., abs(sin(rate*xMult + runner * swMult*.5))*1.2))*rate - 30 * caps;
			float B = 90 * caps + 165 * (1 - max(0., abs(sin(rate*xMult*2. + runner * swMult)*1.2 - .2))) * caps;
			float A = 255 * blendEnd * (R / 255.*(1 - caps) + 1 * caps);
			ofSetColor(R, G, B, A);



			ofDrawTriangle(xPosArr[x][1], yPosArr[x][1], toX, toY, curX, curY);
			if (x < runStop - 1) {
				ofDrawTriangle(xPosArr[x][1], yPosArr[x][1], toX, toY, xPosArr[x + 1][1], yPosArr[x + 1][1]);
			}
			//
			/*
			ofDrawTriangle(xPosArr[x][0], yPosArr[x][0], toX, toY, xPosArr[x][1], yPosArr[x][1]);
			if (x < runStop - 1) {
				ofDrawTriangle(xPosArr[x+1][0], yPosArr[x+1][0], xPosArr[x][1], yPosArr[x][1] , xPosArr[x + 1][1], yPosArr[x + 1][1]);
			}
			*/
			curX = toX;
			curY = toY;
			float toXOrig = (toX - sW * .5);
			float toYOrig = (toY - sH * .5);
			toXOrig = max(0., abs(toXOrig) - 1.) * sign(toXOrig) * (1 + (1 + sin(rate*PI * 2 + curTimer * .6 + (toX - sW * .5) / sW * PI * 2 + (toY - sH * .5) / sH * PI * 2))*(1 - caps))*.1;
			toYOrig = max(0., abs(toYOrig) - 1.) * sign(toYOrig) * (1 + (1 + sin(rate*PI * 2 + curTimer * .6 + (toX - sW * .5) / sW * PI * 2 + (toY - sH * .5) / sH * PI * 2)) * (1 - caps))*.1;
			toX = int(float(toX) + toXOrig);
			toY = int(float(toY) + toYOrig);

			float tempX = xPosArr[x][0];
			float tempY = yPosArr[x][0];
			xPosArr[x][0] = (toX + xPosArr[x][0])*.5;
			yPosArr[x][0] = (toY + yPosArr[x][0])*.5;
			xPosArr[x][1] = (xPosArr[x][1] + tempX + toX)*.3333 - 2;
			yPosArr[x][1] = (yPosArr[x][1] + tempY + toY)*.3333 - 2;
		}
		else {
			xPosArr[x][0] = sW * .5;
			yPosArr[x][0] = sH * .5;
			xPosArr[x][1] = sW * .5;
			yPosArr[x][1] = sH * .5;
		}
	}
	lineCountPrev = lineCount;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	mouseX = (mouseX + ofGetMouseX())*.5;
	mouseY = (mouseY + ofGetMouseY())*.5;
	mouseY = sH - mouseY;
	mouseXNorm = float(mouseX) / float(sW);
	mouseYNorm = float(mouseY) / float(sH);
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
	sW = ofGetWidth();
	sH = ofGetHeight();

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}