/*
https://github.com/micuat/sharedFace

Naoto Hieda <micuat@gmail.com> 2014
*/

#pragma once

#include "ofMain.h"

//#define ENABLE_BOX2D

#include "ofxCv.h"
#include "ofxOsc.h"
#include "ofxFluid.h"
#ifdef ENABLE_BOX2D
#include "ofxBox2d.h"
#endif

//#define USE_MSSDK

#ifndef USE_MSSDK
#define USE_FACETRACKER
#endif

#define PORT 57121

#ifdef USE_MSSDK
#define NUM_POINTS 121
#define FACE_ANIMATION_SIZE 6
#define FACE_POSE_SIZE 6
#endif

#ifdef USE_FACETRACKER
#define NUM_POINTS 66
#define FACE_ANIMATION_SIZE 0
#define FACE_POSE_SIZE 0
#endif

class ofApp : public ofBaseApp {
public:
	void setup();
	void init();
	void update();
	void draw();
	void keyPressed(int);
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void dragEvent(ofDragInfo);
	void audioIn(float *input, int bufferSize, int nChannels);
	void drawPolygon(float x, float y);

	vector<string> rootDir;

private:
	int projector_width, projector_height;
	float projector_horizontal_center;
	ofMesh faceMesh;
	vector<float> faceAnimation, facePose, facePoseOld;
	ofEasyCam cam;
	cv::Mat proIntrinsic, proExtrinsic;
	ofxCv::Intrinsics proCalibration;
	cv::Size proSize;

	enum CameraMode { EASYCAM_MODE, PRO_MODE, CAM_MODE };
	CameraMode cameraMode;

	ofxOscReceiver receiver;

	ofSoundStream soundStream;
	float smoothedVol;

	ofFbo faceFbo;
	ofShader shader;

	vector<ofMesh> lines;
	vector<ofPoint> stampPoints;

	ofVec2f moveKey;

	bool pathLoaded;
	bool toStoreMesh;

	enum RenderMode { BASIC_MODE, BOX2D_MODE, FLUID_MODE, FAKE3D_MODE };
	RenderMode renderMode;

#ifdef ENABLE_BOX2D
	ofxBox2d box2d;
	vector <ofPtr<ofxBox2dCircle> > circles;
	ofPtr <ofxBox2dEdge> edge;
#endif

	ofxFluid fluid;

	ofxCv::KalmanPosition kalmanPosition;
	ofMatrix4x4 predictedMatrix;
	
	ofFloatColor penColor;
	bool canvasMirror;
};

#ifdef USE_MSSDK
static const int faceTriangles[206][3] = { { 11, 0, 1 },
{ 1, 0, 34 },
{ 34, 0, 44 },
{ 12, 11, 1 },
{ 34, 44, 45 },
{ 12, 1, 13 },
{ 13, 1, 2 },
{ 2, 1, 34 },
{ 46, 2, 34 },
{ 46, 34, 45 },
{ 14, 12, 13 },
{ 14, 13, 15 },
{ 15, 13, 16 },
{ 13, 2, 16 },
{ 16, 2, 17 },
{ 17, 2, 3 },
{ 3, 2, 50 },
{ 50, 2, 49 },
{ 49, 2, 46 },
{ 49, 46, 48 },
{ 48, 46, 47 },
{ 46, 45, 47 },
{ 29, 14, 15 },
{ 29, 15, 20 },
{ 15, 18, 19 },
{ 16, 18, 15 },
{ 18, 16, 17 },
{ 18, 17, 19 },
{ 23, 17, 77 },
{ 17, 3, 77 },
{ 78, 3, 50 },
{ 56, 78, 50 },
{ 52, 50, 51 },
{ 50, 49, 51 },
{ 49, 48, 51 },
{ 51, 48, 52 },
{ 53, 48, 62 },
{ 48, 47, 62 },
{ 28, 29, 27 },
{ 29, 20, 27 },
{ 26, 24, 25 },
{ 58, 57, 59 },
{ 60, 53, 62 },
{ 60, 62, 61 },
{ 26, 111, 33 },
{ 26, 75, 111 },
{ 25, 75, 26 },
{ 59, 76, 58 },
{ 112, 76, 59 },
{ 66, 112, 59 },
{ 33, 6, 7 },
{ 7, 6, 66 },
{ 32, 9, 10 },
{ 10, 9, 65 },
{ 76, 6, 5 },
{ 5, 6, 75 },
{ 77, 3, 78 },
{ 33, 7, 79 },
{ 79, 7, 81 },
{ 81, 7, 87 },
{ 80, 7, 66 },
{ 82, 7, 80 },
{ 87, 7, 82 },
{ 82, 80, 89 },
{ 89, 80, 64 },
{ 88, 79, 81 },
{ 31, 79, 88 },
{ 79, 26, 33 },
{ 31, 26, 79 },
{ 66, 59, 80 },
{ 80, 59, 64 },
{ 88, 83, 85 },
{ 88, 85, 31 },
{ 83, 8, 85 },
{ 83, 40, 8 },
{ 8, 40, 84 },
{ 8, 84, 86 },
{ 89, 86, 84 },
{ 64, 86, 89 },
{ 9, 85, 8 },
{ 9, 8, 86 },
{ 85, 32, 31 },
{ 9, 32, 85 },
{ 65, 86, 64 },
{ 65, 9, 86 },
{ 26, 27, 24 },
{ 30, 90, 32 },
{ 32, 90, 31 },
{ 90, 30, 28 },
{ 90, 26, 31 },
{ 90, 27, 26 },
{ 90, 28, 27 },
{ 60, 59, 57 },
{ 65, 91, 63 },
{ 64, 91, 65 },
{ 91, 61, 63 },
{ 91, 64, 59 },
{ 91, 59, 60 },
{ 91, 60, 61 },
{ 77, 92, 23 },
{ 23, 92, 25 },
{ 25, 92, 75 },
{ 56, 93, 78 },
{ 76, 93, 58 },
{ 58, 93, 56 },
{ 77, 94, 92 },
{ 92, 94, 75 },
{ 75, 94, 5 },
{ 5, 94, 76 },
{ 76, 94, 93 },
{ 93, 94, 78 },
{ 78, 94, 77 },
{ 95, 20, 15 },
{ 97, 20, 95 },
{ 101, 20, 99 },
{ 27, 20, 101 },
{ 19, 95, 15 },
{ 21, 95, 19 },
{ 97, 95, 21 },
{ 27, 101, 24 },
{ 24, 101, 22 },
{ 22, 101, 99 },
{ 23, 103, 17 },
{ 23, 105, 103 },
{ 23, 109, 107 },
{ 23, 25, 109 },
{ 17, 103, 19 },
{ 19, 103, 21 },
{ 21, 103, 105 },
{ 107, 109, 22 },
{ 22, 109, 24 },
{ 24, 109, 25 },
{ 104, 56, 50 },
{ 106, 56, 104 },
{ 110, 56, 108 },
{ 58, 56, 110 },
{ 52, 104, 50 },
{ 54, 104, 52 },
{ 106, 104, 54 },
{ 55, 110, 108 },
{ 57, 110, 55 },
{ 58, 110, 57 },
{ 48, 53, 96 },
{ 53, 98, 96 },
{ 100, 53, 102 },
{ 102, 53, 60 },
{ 48, 96, 52 },
{ 52, 96, 54 },
{ 54, 96, 98 },
{ 100, 102, 55 },
{ 55, 102, 57 },
{ 57, 102, 60 },
{ 6, 111, 75 },
{ 33, 111, 6 },
{ 76, 112, 6 },
{ 6, 112, 66 },
{ 74, 73, 70 },
{ 70, 73, 69 },
{ 68, 67, 72 },
{ 72, 67, 71 },
{ 69, 53, 70 },
{ 74, 56, 73 },
{ 71, 23, 72 },
{ 68, 20, 67 },
{ 69, 98, 53 },
{ 69, 54, 98 },
{ 73, 54, 69 },
{ 106, 54, 73 },
{ 56, 106, 73 },
{ 108, 56, 74 },
{ 74, 55, 108 },
{ 70, 55, 74 },
{ 100, 55, 70 },
{ 53, 100, 70 },
{ 67, 20, 97 },
{ 67, 97, 21 },
{ 67, 21, 71 },
{ 71, 21, 105 },
{ 71, 105, 23 },
{ 99, 20, 68 },
{ 99, 68, 22 },
{ 72, 22, 68 },
{ 107, 22, 72 },
{ 23, 107, 72 },
{ 63, 61, 113 },
{ 61, 62, 113 },
{ 113, 62, 114 },
{ 62, 47, 114 },
{ 114, 47, 115 },
{ 47, 45, 115 },
{ 113, 114, 116 },
{ 114, 115, 116 },
{ 28, 30, 117 },
{ 29, 28, 117 },
{ 29, 117, 118 },
{ 14, 29, 118 },
{ 14, 118, 119 },
{ 12, 14, 119 },
{ 118, 117, 120 },
{ 119, 118, 120 },
{ 88, 81, 83 },
{ 81, 40, 83 },
{ 82, 89, 84 },
{ 87, 82, 84 },
{ 81, 87, 40 },
{ 87, 84, 40 }
};

static const float faceUv[121][2] = {
{ 513.663, 123.687 }, //     0
{ 470.813, 194.481 }, //     1
{ 512.645, 259.29 }, //     2
{ 512.273, 322.227 }, //     3
{ 512.193, 337.751 }, //     4
{ 511.527, 441.914 }, //     5
{ 511.508, 452.605 }, //     6
{ 511.333, 480.006 }, //     7
{ 511.087, 523.206 }, //     8
{ 511.029, 537.769 }, //     9
{ 510.702, 598.089 }, //    10
{ 459.363, 128.863 }, //    11
{ 399.54, 161.85 }, //    12
{ 406.454, 235.247 }, //    13
{ 360.868, 253.817 }, //    14
{ 384.831, 321.544 }, //    15
{ 417.838, 299.096 }, //    16
{ 480.946, 322.117 }, //    17
{ 417.764, 311.669 }, //    18
{ 438.551, 334.04 }, //    19
{ 397.208, 352.141 }, //    20
{ 438.531, 339.117 }, //    21
{ 438.412, 359.019 }, //    22
{ 480.629, 352.963 }, //    23
{ 438.389, 363.382 }, //    24
{ 485.358, 426.81 }, //    25
{ 469.526, 447.746 }, //    26
{ 417.651, 412.808 }, //    27
{ 374.672, 450.531 }, //    28
{ 360.544, 351.256 }, //    29
{ 394.097, 537.886 }, //    30
{ 456.38, 492.632 }, //    31
{ 468.551, 588.985 }, //    32
{ 501.065, 476.771 }, //    33
{ 555.333, 194.979 }, //    34
{ 512.645, 259.29 }, //    35
{ 512.273, 322.227 }, //    36
{ 512.193, 337.751 }, //    37
{ 511.527, 441.914 }, //    38
{ 511.508, 452.605 }, //    39
{ 511.203, 506.285 }, //    40
{ 511.133, 514.967 }, //    41
{ 511.061, 531.031 }, //    42
{ 510.715, 594.5 }, //    43
{ 567.901, 129.508 }, //    44
{ 627.276, 163.203 }, //    45
{ 619.293, 236.499 }, //    46
{ 664.866, 255.603 }, //    47
{ 639.906, 323.031 }, //    48
{ 607.029, 300.203 }, //    49
{ 543.595, 322.483 }, //    50
{ 606.956, 312.774 }, //    51
{ 585.95, 334.901 }, //    52
{ 627.184, 353.478 }, //    53
{ 585.91, 339.975 }, //    54
{ 585.794, 359.875 }, //    55
{ 543.657, 353.328 }, //    56
{ 585.77, 364.238 }, //    57
{ 538.041, 427.114 }, //    58
{ 553.626, 448.23 }, //    59
{ 605.98, 413.895 }, //    60
{ 648.8, 452.106 }, //    61
{ 664.062, 353.02 }, //    62
{ 628.369, 539.216 }, //    63
{ 566.265, 493.262 }, //    64
{ 553.019, 589.462 }, //    65
{ 521.634, 476.889 }, //    66
{ 427.663, 339.907 }, //    67
{ 427.541, 360.582 }, //    68
{ 596.797, 340.892 }, //    69
{ 596.677, 361.565 }, //    70
{ 447.835, 340.024 }, //    71
{ 447.713, 360.699 }, //    72
{ 576.625, 340.776 }, //    73
{ 576.509, 361.448 }, //    74
{ 492.401, 441.587 }, //    75
{ 530.71, 441.805 }, //    76
{ 506.898, 337.629 }, //    77
{ 517.51, 337.688 }, //    78
{ 484.027, 480.486 }, //    79
{ 538.704, 480.799 }, //    80
{ 489.22, 493.925 }, //    81
{ 533.374, 494.177 }, //    82
{ 489.141, 503.63 }, //    83
{ 533.35, 503.882 }, //    84
{ 483.803, 515.435 }, //    85
{ 538.541, 515.746 }, //    86
{ 511.264, 493.755 }, //    87
{ 466.909, 493.819 }, //    88
{ 555.752, 494.326 }, //    89
{ 426.523, 498.487 }, //    90
{ 596.116, 499.455 }, //    91
{ 496.261, 382.169 }, //    92
{ 527.643, 382.351 }, //    93
{ 511.983, 372.672 }, //    94
{ 417.995, 339.521 }, //    95
{ 606.488, 340.619 }, //    96
{ 417.976, 343.174 }, //    97
{ 606.469, 344.271 }, //    98
{ 417.892, 357.787 }, //    99
{ 606.387, 358.882 }, //   100
{ 417.794, 359.952 }, //   101
{ 606.462, 361.048 }, //   102
{ 459.567, 339.939 }, //   103
{ 564.88, 340.552 }, //   104
{ 459.547, 343.578 }, //   105
{ 564.861, 344.192 }, //   106
{ 459.463, 358.138 }, //   107
{ 564.777, 358.75 }, //   108
{ 459.405, 360.296 }, //   109
{ 564.815, 360.908 }, //   110
{ 482.668, 452.566 }, //   111
{ 540.366, 452.897 }, //   112
{ 661.207, 454.219 }, //   113
{ 677.23, 351.881 }, //   114
{ 675.219, 253.568 }, //   115
{ 680.038, 350.68 }, //   116
{ 362.64, 452.505 }, //   117
{ 347.795, 349.965 }, //   118
{ 350.949, 251.661 }, //   119
{ 345.414, 348.733 }, //   120
};
#endif

#ifdef USE_FACETRACKER
static const int faceTriangles[91][3] = { {20, 21, 23},
	{21, 22, 23},
	{0, 1, 36},
	{15, 16, 45},
	{0, 17, 36},
	{16, 26, 45},
	{17, 18, 37},
	{25, 26, 44},
	{17, 36, 37},
	{26, 44, 45},
	{18, 19, 38},
	{24, 25, 43},
	{18, 37, 38},
	{25, 43, 44},
	{19, 20, 38},
	{23, 24, 43},
	{20, 21, 39},
	{22, 23, 42},
	{20, 38, 39},
	{23, 42, 43},
	{21, 22, 27},
	{21, 27, 39},
	{22, 27, 42},
	{27, 28, 42},
	{27, 28, 39},
	{28, 42, 47},
	{28, 39, 40},
	{1, 36, 41},
	{15, 45, 46},
	{1, 2, 41},
	{14, 15, 46},
	{28, 29, 40},
	{28, 29, 47},
	{2, 40, 41},
	{14, 46, 47},
	{2, 29, 40},
	{14, 29, 47},
	{2, 3, 29},
	{13, 14, 29},
	{29, 30, 31},
	{29, 30, 35},
	{3, 29, 31},
	{13, 29, 35},
	{30, 32, 33},
	{30, 33, 34},
	{30, 31, 32},
	{30, 34, 35},
	{3, 4, 31},
	{12, 13, 35},
	{4, 5, 48},
	{11, 12, 54},
	{5, 6, 48},
	{10, 11, 54},
	{6, 48, 59},
	{10, 54, 55},
	{6, 7, 59},
	{9, 10, 55},
	{7, 58, 59},
	{9, 55, 56},
	{8, 57, 58},
	{8, 56, 57},
	{7, 8, 58},
	{8, 9, 56},
	{4, 31, 48},
	{12, 35, 54},
	{31, 48, 49},
	{35, 53, 54},
	{31, 49, 50},
	{35, 52, 53},
	{31, 32, 50},
	{34, 35, 52},
	{32, 33, 50},
	{33, 34, 52},
	{33, 50, 51},
	{33, 51, 52},
	{48, 49, 60},
	{49, 60, 50},
	{50, 60, 61},
	{50, 51, 61},
	{51, 52, 61},
	{61, 62, 52},
	{52, 53, 62},
	{53, 54, 62},
	{54, 55, 63},
	{55, 56, 63},
	{56, 63, 64},
	{56, 57, 64},
	{64, 65, 57},
	{57, 58, 65},
	{58, 59, 65},
	{48, 59, 65}
};

static const float faceUv[66][2] = {
	{ 353.937, 358.816 },
	{ 352.839, 401.835 },
	{ 354.62, 444.923 },
	{ 359.899, 487.658 },
	{ 373.801, 528 },
	{ 398.689, 562.388 },
	{ 431.468, 589.713 },
	{ 468.807, 610.397 },
	{ 510.953, 615.982 },
	{ 553.161, 610.882 },
	{ 590.735, 590.629 },
	{ 623.827, 563.682 },
	{ 649.109, 529.583 },
	{ 663.474, 489.403 },
	{ 669.244, 446.732 },
	{ 671.521, 403.668 },
	{ 670.917, 360.639 },
	{ 387.373, 320.956 },
	{ 403.18, 303.112 },
	{ 425.828, 294.739 },
	{ 450.301, 296.145 },
	{ 474.195, 301.875 },
	{ 551.322, 302.319 },
	{ 575.281, 296.864 },
	{ 599.768, 295.739 },
	{ 622.317, 304.371 },
	{ 637.919, 322.396 },
	{ 512.516, 344.255 },
	{ 512.358, 371.746 },
	{ 512.201, 399.089 },
	{ 512.044, 426.312 },
	{ 482.19, 447.129 },
	{ 496.806, 450.706 },
	{ 511.894, 452.458 },
	{ 527, 450.88 },
	{ 541.657, 447.471 },
	{ 421.495, 351.523 },
	{ 436.76, 341.415 },
	{ 455.496, 341.44 },
	{ 470.801, 351.178 },
	{ 454.617, 356.395 },
	{ 437.369, 356.935 },
	{ 554.148, 351.657 },
	{ 569.565, 342.096 },
	{ 588.3, 342.286 },
	{ 603.448, 352.57 },
	{ 587.513, 357.799 },
	{ 570.272, 357.06 },
	{ 456.617, 503.663 },
	{ 472.6, 491.082 },
	{ 491.459, 482.673 },
	{ 511.701, 485.974 },
	{ 531.98, 482.906 },
	{ 550.741, 491.531 },
	{ 566.578, 504.295 },
	{ 549.931, 511.941 },
	{ 531.243, 517.505 },
	{ 511.511, 519.037 },
	{ 491.797, 517.278 },
	{ 473.175, 511.5 },
	{ 492.195, 495.846 },
	{ 511.636, 497.226 },
	{ 531.093, 496.07 },
	{ 531.035, 502.461 },
	{ 511.592, 504.929 },
	{ 492.179, 502.237 }
};
#endif