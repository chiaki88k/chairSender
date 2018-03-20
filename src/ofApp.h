#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDatGui.h"
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
    void mousePressed(int x,int y,int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void reMapMesh();
    void drawInfo();
    void saveXmlF(bool load);
    void saveXmlS(bool load);
    void loadXmlF();
    void loadXmlS();
    void exit();
    void howtoButtonPressed();
    void viewButtonPressed();
    void modelPressed();
    void cornerLockXY();
    void savePressed();
    void resetPressed();
    void resolution(int r);
    void guiSetUp1();
    void guiSetUp2();
    void chairLoad(string path);
    void easingCam();
    
    ofxBezierSurface front,side;
    ofVboMesh mesh,warpF,warpS;
    ofxAssimpModelLoader chair,human;
    ofxOscSender sender;
    ofEasyCam Ecam;
    ofCamera cam;
    ofLight light;
    ofVec3f nextCam,easing;
    ofXml xml,xmlS,xmlF;
    ofImage ht;
    
    int res = 120 ;//warpMeshSize
    int ctrP = 5;//warpControlPoints
    int frontW = 500,frontH = 500;//
    
    int sideW = 500,sideH = 500;//sw550 sh780

    ofVec2f area0,area1,area2,area3,pMouse;
    
    int dW,dH,dD;//display size
    int  viewNum = 0,modelNum = 0;
    
    float wfx,wfy,wsy,wsz;
    float alpha=1;
    string info,view,name;
    bool showGui=false;
    bool grid=false;
    bool easingView = false;
    bool howto = false;
    int LockXY = 0;//0=lockX,1=lockY
    float EcamX=450,EcamY=630,EcamZ=960;
    ofxPanel gui,gui2;
    ofxIntSlider lpX,lpY,lpZ,warpCol,Res,lineAlpha;
    ofxFloatSlider camX,camY,camZ;
    ofxColorSlider ambient,diffuse,bg;
    ofxButton viewButton,cornerLock,saveButton,resetButton,modelButton,howtoButton;
    
    int rY=-90,rX=-90,time,ltime;//rotateX/rotateY
    ofColor warpC=ofColor(210);
    
};


 
