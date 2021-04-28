#include "ofApp.h"

#define NUM_BUBBLES 50

//--------------------------------------------------------------
void ofApp::setup() {
    bakuretuModeOn = true;
    numBubbles = NUM_BUBBLES;
    bubbleParticles.getVertices().resize(numBubbles);
    bubbleParticles.getColors().resize(numBubbles);
    bubbleParticles.getNormals().resize(numBubbles,glm::vec3(0));
    bubbleSizes.resize(numBubbles);

	// ------------------------- billboard particles
	for (int i=0; i<numBubbles; i++) {
        bubbleParticles.getVertices()[i] = {ofRandom(15, 25),ofRandom(2, 27),ofRandom(-5, 5)};
        bubbleParticles.getColors()[i].set(ofColor(255, 255, 255));
        bubbleSizes[i] = ofRandom(5, 100);
	}

    bubbleParticles.setUsage( GL_DYNAMIC_DRAW );
    bubbleParticles.setMode(OF_PRIMITIVE_POINTS);

	if(ofIsGLProgrammableRenderer()){
        bubbleShader.load("shadersGL3/particle");
	}else{
        bubbleShader.load("shadersGL2/particle");
	}

    ofEnableNormalizedTexCoords();
    ofEnableAlphaBlending();
	// we need to disable ARB textures in order to use normalized texcoords
	ofDisableArbTex();
    bubble.load("awa.png");
    ofEnableArbTex();
    
    
    cameraPositionX = 10;
    cameraPositionY = 4;
    cameraPositionZ = 20;
    sphere.set(10, 32);
    sphere.setPosition(-20, 4, 0);
    plane.set(10, 10);
    plane.setPosition(20, 2, 0);
    plane.rotate(90, ofVec3f(1, 0, 0));
    
    cam.setScale(ofVec3f(0.01, 0.01, 0.01));
    azimuth = 0;
    elevation = 0;
}

//--------------------------------------------------------------
void ofApp::update() {
    
    cam.setPosition(cameraPositionX, cameraPositionY, cameraPositionZ);

	for (int i=0; i<numBubbles; i++) {
        if(bubbleParticles.getVertices()[i].y > 27){
            bubbleParticles.getVertices()[i].y = 2;
        }
        float heightRate = ofMap(bubbleParticles.getVertices()[i].y, 2, 27, 0, 1);
        bubbleParticles.getVertices()[i] += ofVec3f(0, 0.1 * (1.f + 5.f * heightRate * heightRate), 0);
        if(bakuretuModeOn){
            bubbleParticles.setNormal(i,glm::vec3(bubbleSizes[i] * sqrt(heightRate),0,0));
        }else{
            if(i % 10 == 0){
                bubbleParticles.setNormal(i,glm::vec3(bubbleSizes[i] * sqrt(heightRate),0,0));
            }else{
                bubbleParticles.setNormal(i,glm::vec3(0,0,0));
            }
        }
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(189, 205, 238);
    
    cam.begin();
    
    bubbleShader.begin();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnablePointSprites(); // not needed for GL3/4
    bubble.bind();
    bubbleParticles.draw();
    bubble.unbind();
	ofDisablePointSprites(); // not needed for GL3/4
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    bubbleShader.end();
    
    sphere.drawWireframe();
    plane.draw();
    
    cam.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    float positionVerocity = 1;
    int degreeVerocity = 10;
    switch (key) {
    case 'b':
        if(bakuretuModeOn){
            bakuretuModeOn = false;
        }else{
            bakuretuModeOn = true;
        }
        break;
        case 'w':
            cameraPositionZ -= cos(ofDegToRad(azimuth)) * positionVerocity;
            cameraPositionX -= sin(ofDegToRad(azimuth)) * positionVerocity;
            break;
        case 'a':
            cameraPositionX -= cos(ofDegToRad(azimuth)) * positionVerocity;
            cameraPositionZ += sin(ofDegToRad(azimuth)) * positionVerocity;
            break;
        case 's':
            cameraPositionZ += cos(ofDegToRad(azimuth)) * positionVerocity;
            cameraPositionX += sin(ofDegToRad(azimuth)) * positionVerocity;
            break;
        case 'd':
            cameraPositionX += cos(ofDegToRad(azimuth)) * positionVerocity;
            cameraPositionZ -= sin(ofDegToRad(azimuth)) * positionVerocity;
            break;
        case OF_KEY_UP:
            if(elevation<90){
                cam.rotate(degreeVerocity, ofVec3f(cos(ofDegToRad(azimuth)), 0, -sin(ofDegToRad(azimuth))));
                elevation += degreeVerocity;
            }
            break;
        case OF_KEY_DOWN:
            if(elevation>-90){
                cam.rotate(-degreeVerocity, ofVec3f(cos(ofDegToRad(azimuth)), 0, -sin(ofDegToRad(azimuth))));
                elevation -= degreeVerocity;
            }
            break;
        case OF_KEY_LEFT:
            cam.rotate(degreeVerocity, ofVec3f(0, 1, 0));
            azimuth += degreeVerocity;
            break;
        case OF_KEY_RIGHT:
            cam.rotate(-degreeVerocity, ofVec3f(0, 1, 0));
            azimuth -= degreeVerocity;
            break;
        case 'k':
            cameraPositionY += positionVerocity;
            break;
        case 'j':
            cameraPositionY -= positionVerocity;
            break;
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
