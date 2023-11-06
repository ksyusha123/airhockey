#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

class Puck {
    public:
        cv::Point2f pos;
        cv::Point2f velocity;

        void reset(cv::Point2f startPos){
            pos = startPos;
            velocity = cv::Point2f(0, 0);
        }

        bool move(cv::Point2f flow, int sourceWidth, int sourceHeight) {
			velocity += 2 * flow;
            pos += velocity;
            pos.x = ofClamp(pos.x, 0, sourceWidth - 1);
            pos.y = ofClamp(pos.y, 0, sourceHeight - 1);
			bool hit = false;

            if (pos.y == 0 || pos.y == sourceHeight - 1){
                velocity.y *= -1;
				hit = true;
            }

            velocity.x *= 0.975;
            velocity.y *= 0.975;

			return hit;
        }
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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

		void updateScores();
		
		ofVideoGrabber vidGrabber;
		int initWidth = 1280;
		int initHeight = 720; 
		ofxCvColorImage	colorImg;
		ofxCvGrayscaleImage grayImage;
		ofxCvGrayscaleImage currentImage;
		cv::Mat previousMat;
		cv::Mat flowMat;
		ofTrueTypeFont font;
		ofSoundPlayer soundPlayer;

		int blurAmount = 25;
		int cvDownScale = 8;

		Puck puck;
		int leftPlayerScore;
		int rightPlayerScore;
};
