#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "ofxBezierSurface.h"
#include "ofxOsc.h"
//#define HOST"169.254.74.154"//IP adress
#define HOST"localhost"//
#define PORT 8000//変えない

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void reMapMesh();
    void drawInfo();
    void saveXmlF();
    void saveXmlS(bool load);
    void loadXmlF();
    void loadXmlS();
    void exit();
    void viewButtonPressed();
    void savePressed();
    void resetPressed();
    void resolution(int r);//解像度の変更
    

    ofxBezierSurface front,side;
    ofVboMesh mesh,warpF,warpS;
    ofxAssimpModelLoader chair,human;
    ofxOscSender sender;
    ofEasyCam Ecam;
    ofCamera cam;
    ofLight light;
    ofVec3f nextCam,easing;
    ofXml xml,xmlS,xmlF;
    
    int res = 50 ;//warpMeshSize
    int ctrP = 5;//warpControlPoints
    int frontW = 350,frontH = 350;//warpのサイズ550,halfは280
    int sideW = 350,sideH = 450;//sw550 sh780

    ofVec2f area0,area1,area2,area3;
    
    int dW,dH,dD;//display size
    int  viewMode = 0;
    float wfx,wfy,wsy,wsz;//レスポンシブ用
    string info,view;
    bool showGui=false;
    bool grid=false;
    ofxPanel gui,gui2;
    ofxIntSlider lpX,lpY,lpZ,warpCol,Res,lineAlpha;
    ofxFloatSlider camX,camY,camZ;
    ofxColorSlider ambient,diffuse,bg;
    ofxButton viewButton,saveButton,resetButton;
    
    int rY=-90,rX=-90,time;//rotateX/rotateY
    ofColor warpC=ofColor(210);
    
};


 
