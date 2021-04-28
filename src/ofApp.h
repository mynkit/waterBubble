#pragma once
#include "ofMain.h"
#include "ofVbo.h"

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        bool bakuretuModeOn;
        bool reverseModeOn;
        int numBubbles;
	    vector<float> bubbleSizes;
		ofShader bubbleShader;
		ofImage bubble;
		ofVboMesh bubbleParticles;
    
        int numSprites;
        vector<float> spriteSizes;
        vector<float> spriteAzim;
        vector<float> spriteElev;
        ofShader spriteShader;
        ofImage sprite;
        ofVboMesh spriteParticles;
        int sphereRadius;
    
        ofCamera cam;
        ofSpherePrimitive sphere;
        float cameraPositionX;
        float cameraPositionY;
        float cameraPositionZ;
        float spherePositionX;
        float spherePositionY;
        float spherePositionZ;
        int azimuth;
        int elevation;
        ofPlanePrimitive plane;
};









