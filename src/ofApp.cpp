#include "ofApp.h"

const float ofApp::KINECT_CAM_SHRINK = 3.0f;

//--------------------------------------------------------------
void ofApp::setup(){
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(30);
	vidGrabber.initGrabber(CAM_WIDTH, CAM_HEIGHT);

	ofColor.allocate(CAM_WIDTH, CAM_HEIGHT, OF_IMAGE_COLOR);
	ofCropped.allocate(CAM_WIDTH, CROP_HEIGHT, OF_IMAGE_COLOR);
	
	ofSetVerticalSync(true);

	cv::namedWindow("from oF to CV", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);

	// SETUP KINECT
	HRESULT hr = GetDefaultKinectSensor(&kinectSensor);
	if (kinectSensor){
		IColorFrameSource* colorFrameSource = NULL;
		hr = kinectSensor->Open();

		if (SUCCEEDED(hr)) {
			hr = kinectSensor->get_ColorFrameSource(&colorFrameSource);
		}

		if (SUCCEEDED(hr)) {
			hr = colorFrameSource->OpenReader(&colorFrameReader);
		}

		SafeRelease(colorFrameSource);
	}

	ofColorFromK.allocate(KINECT_CAM_WIDTH, KINECT_CAM_HEIGHT, OF_IMAGE_COLOR_ALPHA);
	bufForColorFromKinect = new unsigned char[KINECT_CAM_WIDTH * KINECT_CAM_HEIGHT * 4];
	cv::namedWindow("from Kinect to CV", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
}

//--------------------------------------------------------------
void ofApp::update(){
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

	vidGrabber.update();

	if (vidGrabber.isFrameNew()){
		//ššš in oF, ofVideoGrabber to ofImage ššš
		ofColor.setFromPixels(vidGrabber.getPixels(), CAM_WIDTH, CAM_HEIGHT, OF_IMAGE_COLOR);
		ofColor.update();

		//ššš from oF(ofImage) to OpenCV(cv::Mat) ššš
		//cv::Mat cvColor = cv::Mat(CAM_HEIGHT, CAM_WIDTH, CV_8UC3, vidGrabber.getPixels()); //’¼Ú‚±‚¤‚â‚Á‚Ä‚à‚æ‚¢
		cv::Mat cvColor = cv::Mat(CAM_HEIGHT, CAM_WIDTH, CV_8UC3, ofColor.getPixels());
		cvtColor(cvColor, cvColor, CV_RGB2BGR);

		// operate in OpenCV
		cv::Rect roiRect = cv::Rect(0, CROP_Y, CAM_WIDTH, CROP_HEIGHT);
		cv::Mat cvCropped = cv::Mat(cvColor, roiRect);
		cv::imshow("from oF to CV", cvCropped);

		//ššš from OpenCV(cv::Mat) to oF(ofImage) ššš
		//cvtColor(cvCropped, cvCropped, CV_BGR2RGB);
		// setFromPixels()'s 4th parameter 'false' converts BGR image to RGB
		ofCropped.setFromPixels(cvCropped.ptr(), cvCropped.cols, cvCropped.rows, OF_IMAGE_COLOR, false);

	}

	if (!colorFrameReader) {
		return;
	}

	IColorFrame* colorFrame = NULL;
	HRESULT hr = colorFrameReader->AcquireLatestFrame(&colorFrame);

	if (SUCCEEDED(hr)) {
		IFrameDescription* pFrameDescription = NULL;
		int nWidth = 0;
		int nHeight = 0;
		ColorImageFormat imageFormat = ColorImageFormat_None;
		UINT nBufferSize = 0;
		BYTE *data = ofColorFromK.getPixels();

		if (SUCCEEDED(hr)) {
			hr = colorFrame->get_FrameDescription(&pFrameDescription);
		}

		if (SUCCEEDED(hr)) {
			hr = pFrameDescription->get_Width(&nWidth);
		}

		if (SUCCEEDED(hr)) {
			hr = pFrameDescription->get_Height(&nHeight);
		}

		if (SUCCEEDED(hr)) {
			nBufferSize = KINECT_CAM_WIDTH * KINECT_CAM_HEIGHT * 4;

			//ššš from Kinect(IColorFrame) to CV(cv::Mat)
			hr = colorFrame->CopyConvertedFrameDataToArray(nBufferSize, bufForColorFromKinect, ColorImageFormat_Bgra);
			cv::Mat cvColorFromK = cv::Mat(KINECT_CAM_HEIGHT, KINECT_CAM_WIDTH, CV_8UC4, bufForColorFromKinect);
			cv::Mat cvColorFromKResized = cv::Mat(KINECT_CAM_HEIGHT / KINECT_CAM_SHRINK, KINECT_CAM_WIDTH / KINECT_CAM_SHRINK, cvColorFromK.type());
			cv::resize(cvColorFromK, cvColorFromKResized, cvColorFromKResized.size() );
			cv::imshow("from Kinect to CV", cvColorFromKResized);

			//ššš from Kinect(IColorFrame) to oF(ofImage)
			hr = colorFrame->CopyConvertedFrameDataToArray(nBufferSize, data, ColorImageFormat_Rgba);
		}
		if (SUCCEEDED(hr)) {
			ofColorFromK.update();
		}
	}

	SafeRelease(colorFrame);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(100, 100, 100);
	//vidGrabber.draw(0, 0);

	// inside oF
	ofColor.draw(0, 0);
	
	// oF -> CV -> oF
	ofCropped.draw(0, 0 + 20 + CAM_HEIGHT);
	
	// Kinect -> oF
	ofColorFromK.draw(CAM_WIDTH, 0, KINECT_CAM_WIDTH / KINECT_CAM_SHRINK, KINECT_CAM_HEIGHT / KINECT_CAM_SHRINK);
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
