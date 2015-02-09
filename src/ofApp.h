#pragma once

#include "ofMain.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Kinect.h"
#include "util.h"

class ofApp : public ofBaseApp{

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
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

private:
	ofVideoGrabber	vidGrabber;
	static const int CAM_WIDTH = 640;
	static const int CAM_HEIGHT = 480;
	static const int CROP_Y = 230;
	static const int CROP_HEIGHT = 20;

	ofImage			ofColor;
	ofImage			ofCropped;

	IKinectSensor* kinectSensor;
	IColorFrameReader* colorFrameReader;

	static const int KINECT_CAM_WIDTH = 1920;
	static const int KINECT_CAM_HEIGHT = 1080;
	static const float KINECT_CAM_SHRINK;	// defined in cpp file
	ofImage			ofColorFromK;
	unsigned char* bufForColorFromKinect;

};
