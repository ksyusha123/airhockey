#include "ofApp.h"
#include "ofxOpenCv.h"


void ofApp::updateScores(){
    if (puck.pos.x == 0){
        rightPlayerScore += 1;
        puck.reset(cv::Point2f(initWidth / 2, initHeight / 2));
    }

    else if (puck.pos.x == initWidth - 1) {
        leftPlayerScore += 1;
        puck.reset(cv::Point2f(initWidth / 2, initHeight / 2));
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(initWidth, initHeight);
    ofSetFrameRate(60);

    vidGrabber.setVerbose(true);
    vidGrabber.setup(initWidth, initHeight);
    puck.reset(cv::Point2f(initWidth / 2, initHeight / 2));

    font.load("Monoton-Regular.ttf", 50);
    soundPlayer.load("hit.wav");
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();

    colorImg.setFromPixels(vidGrabber.getPixels());
    bool newFrame = vidGrabber.isFrameNew();

    int scaledWidth = initWidth / cvDownScale;
	int scaledHeight = initHeight / cvDownScale;

    if (currentImage.getWidth() != scaledWidth || currentImage.getHeight() != scaledHeight){
        ofLogNotice() << "Allocating current image to: " << scaledWidth << " x " << scaledHeight << " | " << ofGetFrameNum() << endl;
		currentImage.clear();
		currentImage.allocate(scaledWidth, scaledHeight);
		currentImage.set(0);
		previousMat.release();
		currentImage.getCvMat().copyTo(previousMat);
		flowMat.release();
		flowMat = cv::Mat(scaledHeight, scaledWidth, CV_32FC2);
    }

    if (newFrame){
        grayImage = colorImg;
        currentImage.scaleIntoMe(grayImage);
        currentImage.contrastStretch();
        currentImage.mirror(false, true);
        if (blurAmount > 0) {
            currentImage.blurGaussian(blurAmount);
        }

        cv::Mat currentMat = currentImage.getCvMat();
        cv::calcOpticalFlowFarneback(previousMat,
                                    currentMat,
                                    flowMat,
                                    0.5,
                                    5,
                                    15,
                                    3,
                                    7,
                                    1.5,
                                    cv::OPTFLOW_FARNEBACK_GAUSSIAN);
        currentMat.copyTo(previousMat);

        const cv::Point2f& flowAtPos = flowMat.at<cv::Point2f>(puck.pos.y / cvDownScale, puck.pos.x / cvDownScale);
        bool hit = puck.move(flowAtPos, initWidth, initHeight);
        if (hit) {
            soundPlayer.play();
        }
        updateScores();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor( 255 );
    colorImg.mirror(false, true);
    colorImg.draw(0, 0);
    ofSetColor(0, 0, 255);
    ofDrawCircle(puck.pos.x, puck.pos.y, 40);
    
    ofSetColor(255, 255, 255);
    ofDrawRectangle(glm::vec2(initWidth - 200, 0), 200, 100);
    ofSetColor(0, 0, 255);
    font.drawString(std::format("{}:{}", leftPlayerScore, rightPlayerScore), initWidth - 175, 75);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
