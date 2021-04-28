#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    bakuretuModeOn = true;
    reverseModeOn = false;
    numBubbles = 50;
    bubbleParticles.getVertices().resize(numBubbles);
    bubbleParticles.getColors().resize(numBubbles);
    bubbleParticles.getNormals().resize(numBubbles,glm::vec3(0));
    bubbleSizes.resize(numBubbles);
    
    numSprites = 50;
    spriteParticles.getVertices().resize(numSprites);
    spriteParticles.getColors().resize(numSprites);
    spriteParticles.getNormals().resize(numSprites,glm::vec3(0));
    spriteSizes.resize(numSprites);
    spriteAzim.resize(numSprites);
    spriteElev.resize(numSprites);
    sphereRadius = 10;
    
    cameraPositionX = 10;
    cameraPositionY = 4;
    cameraPositionZ = 20;
    sphere.set(sphereRadius, 32);
    spherePositionX = -20;
    spherePositionY = 4;
    spherePositionZ = 0;
    sphere.setPosition(spherePositionX, spherePositionY, spherePositionZ);
    plane.set(10, 10);
    plane.setPosition(20, 2, 0);
    plane.rotate(90, ofVec3f(1, 0, 0));
    
    cam.setScale(ofVec3f(0.01, 0.01, 0.01));
    azimuth = 0;
    elevation = 0;

	for (int i=0; i<numBubbles; i++) {
        bubbleParticles.getVertices()[i] = {ofRandom(15, 25),ofRandom(2, 27),ofRandom(-5, 5)};
        bubbleParticles.getColors()[i].set(ofColor(255, 255, 255));
        bubbleSizes[i] = ofRandom(5, 150);
	}
    
    for (int i=0; i<numSprites; i++) {
        spriteParticles.getColors()[i].set(ofColor(ofRandom(121, 255), ofRandom(76, 255), ofRandom(0, 255)));
        spriteSizes[i] = ofRandom(5, 150);
        spriteAzim[i] = ofRandom(360);
        spriteElev[i] = ofRandom(-90, 90);
        spriteParticles.getVertices()[i] = {
            spherePositionX + sphereRadius * ofRandomf() * cos(ofDegToRad(spriteAzim[i])) * cos(ofDegToRad(spriteElev[i])),
            spherePositionY + sphereRadius * ofRandomf() * sin(ofDegToRad(spriteElev[i])),
            spherePositionZ + sphereRadius * ofRandomf() * sin(ofDegToRad(spriteAzim[i]) * cos(ofDegToRad(spriteElev[i])))
        };
    }

    bubbleParticles.setUsage( GL_DYNAMIC_DRAW );
    bubbleParticles.setMode(OF_PRIMITIVE_POINTS);
    
    spriteParticles.setUsage( GL_DYNAMIC_DRAW );
    spriteParticles.setMode(OF_PRIMITIVE_POINTS);

	if(ofIsGLProgrammableRenderer()){
        bubbleShader.load("shadersGL3/particle");
        spriteShader.load("shadersGL3/particle");
	}else{
        bubbleShader.load("shadersGL2/particle");
        spriteShader.load("shadersGL2/particle");
	}

    ofEnableNormalizedTexCoords();
    ofEnableAlphaBlending();
	// we need to disable ARB textures in order to use normalized texcoords
	ofDisableArbTex();
    bubble.load("awa.png");
    sprite.load("sprite.jpg");
    ofEnableArbTex();

}

//--------------------------------------------------------------
void ofApp::update() {
    
    cam.setPosition(cameraPositionX, cameraPositionY, cameraPositionZ);

	for (int i=0; i<numBubbles; i++) {
        if(bubbleParticles.getVertices()[i].y >= 27){
            bubbleParticles.getVertices()[i].y = 2;
        }else if(bubbleParticles.getVertices()[i].y <= 2){
            bubbleParticles.getVertices()[i].y = 27;
        }
        float heightRate = ofMap(bubbleParticles.getVertices()[i].y, 2, 27, 0, 1);
        if(reverseModeOn){
            bubbleParticles.getVertices()[i] -= ofVec3f(0, 0.1 * (1.f + 5.f * heightRate * heightRate), 0);
        }else{
            bubbleParticles.getVertices()[i] += ofVec3f(0, 0.1 * (1.f + 5.f * heightRate * heightRate), 0);
        }
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
    for (int i=0; i<numSprites; i++) {
        float dist = ofDist(
                            spriteParticles.getVertices()[i].x, spriteParticles.getVertices()[i].y, spriteParticles.getVertices()[i].z,
                            spherePositionX, spherePositionY, spherePositionZ);
        float velocity = 0.1f * ofMap(dist, 0, sphereRadius * 1.5f, 1, 0.5);
        if(reverseModeOn){
            spriteParticles.getVertices()[i] -= velocity * ofVec3f(
                                                        cos(ofDegToRad(spriteAzim[i])) * cos(ofDegToRad(spriteElev[i])),
                                                        sin(ofDegToRad(spriteElev[i])),
                                                        sin(ofDegToRad(spriteAzim[i])) * cos(ofDegToRad(spriteElev[i]))
                                                        );
        }else{
            spriteParticles.getVertices()[i] += velocity * ofVec3f(
                                                        cos(ofDegToRad(spriteAzim[i])) * cos(ofDegToRad(spriteElev[i])),
                                                        sin(ofDegToRad(spriteElev[i])),
                                                        sin(ofDegToRad(spriteAzim[i])) * cos(ofDegToRad(spriteElev[i]))
                                                        );
        }
        float newDist = ofDist(
                            spriteParticles.getVertices()[i].x, spriteParticles.getVertices()[i].y, spriteParticles.getVertices()[i].z,
                            spherePositionX, spherePositionY, spherePositionZ);
        if(!reverseModeOn && newDist >= sphereRadius * 1.5f){
            spriteParticles.getVertices()[i] = ofVec3f(spherePositionX, spherePositionY, spherePositionZ);
            newDist = 0;
        }
        if(reverseModeOn && dist <= newDist){
            // 球の中心からの距離が、新しいほうが遠ければ円の中心を通り抜けてる
            spriteParticles.getVertices()[i] = ofVec3f(
                                                       spherePositionX + sphereRadius * 1.5f * cos(ofDegToRad(spriteAzim[i])) * cos(ofDegToRad(spriteElev[i])),
                                                       spherePositionY + sphereRadius * 1.5f * sin(ofDegToRad(spriteElev[i])),
                                                       spherePositionZ + sphereRadius * 1.5f * sin(ofDegToRad(spriteAzim[i])) * cos(ofDegToRad(spriteElev[i])));
            newDist = sphereRadius * 1.5f;
        }
        spriteParticles.getColors()[i].set(
                                           spriteParticles.getColors()[i][0],
                                           spriteParticles.getColors()[i][1],
                                           spriteParticles.getColors()[i][2],
                                           ofMap(newDist, 0, sphereRadius * 1.5f, 1, 0));
        if(bakuretuModeOn){
            spriteParticles.setNormal(i,glm::vec3(spriteSizes[i],0,0));
        }else{
            if(i % 10 == 0){
                spriteParticles.setNormal(i,glm::vec3(spriteSizes[i],0,0));
            }else{
                spriteParticles.setNormal(i,glm::vec3(0,0,0));
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
    
    spriteShader.begin();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites(); // not needed for GL3/4
    sprite.bind();
    spriteParticles.draw();
    sprite.unbind();
    ofDisablePointSprites(); // not needed for GL3/4
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    spriteShader.end();
    
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
    case 'm':
        if(reverseModeOn){
            reverseModeOn = false;
        }else{
            reverseModeOn = true;
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
