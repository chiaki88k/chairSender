#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //of_settting
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofEnableDepthTest();
    ofBackground(250);
    
    dW=ofGetWidth();
    dH=ofGetHeight();
    dD=dW;
    
    //chair_settting

    chairLoad("model/zig/zig.stl");
    
    //gui_set
    viewButton.addListener(this, &ofApp::viewButtonPressed);
    cornerLock.addListener(this, &ofApp::cornerLockXY);
    saveButton.addListener(this, &ofApp::savePressed);
    resetButton.addListener(this, &ofApp::resetPressed);
    modelButton.addListener(this, &ofApp::modelPressed);
    
    guiSetUp1();
    
    guiSetUp2();
    
    sender.setup(HOST, PORT);
    
    //cam_set
    cam.setPosition(1000, 1000, 2000);
    cam.lookAt(chair.getPosition());
    Ecam.setPosition(-960, 690, 930);
    Ecam.lookAt(chair.getPosition());
    
    //light_set(x,z,y)
    light.setPosition(ofPoint(0,0,0));
    light.lookAt(ofVec3f(0,0,0));
    light.setSpecularColor(ofFloatColor(1.0,1.0,1.0));
    light.enable();
    
    //warp_set
    //warpF
    front.setup(frontW,frontH,ctrP,res,dW/3,dH/3);//2560/3,1440/3
    front.addListeners(false);
    wfx = frontW/2+dW/3;
    wfy = frontH/2+dH/3;
    
    warpF.setMode(OF_PRIMITIVE_POINTS);
    vector<ofVec3f> warpf = front.getVertices();
    for(int i=0;i<warpf.size();i++){
        warpF.addColor(ofFloatColor(warpC));
        warpF.addVertex(ofVec3f(warpf[i].x-wfx,warpf[i].y-wfy,warpf[i].z));
    }
    //warpS
    side.setup(sideW,sideH,ctrP,res,dW/3,dH/3);
    side.addListeners(false);
    wsy = sideW/2+dW/3;
    wsz = sideH+dH/3;
    
    warpS.setMode(OF_PRIMITIVE_POINTS);
    vector<ofVec3f> warps = side.getVertices();
    for(int i=0;i<warps.size();i++){
        warpS.addColor(ofFloatColor(warpC));
        warpS.addVertex(ofVec3f(warps[i].z+frontW/2,warps[i].x-wsy,warps[i].y-wsz));
    }
    
    saveXmlS(false);
    
    glPointSize(1.0);
    
    reMapMesh();

}

//--------------------------------------------------------------
void ofApp::update(){
    if(viewNum==0){
        front.update();
    }else if(viewNum==1){
        side.update();
    }else{
    }
    
    (ofGetMouseX()==pMouse.x ? time++ : time = 0);
    
    if(time>500){
        easingCam();
    }else if((time-ltime) < 0){
        if(view=="view Side")viewNum-=1;
        if(view=="view Top")viewNum-=2;
        if(view=="view Purse")viewNum=viewNum;
        ltime=0;
    }
    
    light.setPosition(ofPoint(lpX,lpZ,lpY));
    light.setAmbientColor(ofColor(ambient));
    light.setDiffuseColor(ofColor(diffuse));
    
    cam.setPosition(camX, camY, camZ);
    cam.lookAt(ofVec3f(0));
    
//    if(res != Res){
//        resolution(Res);
//    }
    
    warpF.clear();
    warpS.clear();
    
    vector<ofVec3f> warpf = front.getVertices();
    for(int i=0;i<warpf.size();i++){
        warpF.addVertex(ofVec3f(warpf[i].x-wfx,warpf[i].y-wfy,warpf[i].z));
        warpF.addColor(ofFloatColor(warpC));
    }
    vector<ofVec3f> warps = side.getVertices();
    for(int i=0;i<warps.size();i++){
        for(int j=0;j<res;j++){
            if(res*j<=i && i<res*(j+1)){
                warps[i].z = warpf[j+(res*res-res)].x-wfx;
                warps[i].x = warpf[j+(res*res-res)].y-wfy;
            }
        }
        warpS.addVertex(ofVec3f(warps[i].z,warps[i].x,warps[i].y-wsz));
        warpS.addColor(ofFloatColor(warpC));
    }
    
    pMouse.x = ofGetPreviousMouseX();

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor(bg));

    //grid
    if(grid==1){
        int x;
        for(x=0;x<dW;x+=20){
                ofDrawLine(x, 0, x, dH);
                ofDrawLine(0, x, dW, x);
            }
    }
    
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    ofPushMatrix();
    
        if(viewNum==0){
            cam.begin(ofRectangle(dW/3*2, 0, dW/3*1, dH));
        }else if(viewNum==1){
            cam.begin(ofRectangle(dW/3*2, 0, dW/3*1, dH));
        }else if(viewNum==2){
            Ecam.begin();
        }
    
    ofRotate(90,0,0,1);
    ofRotate(rX,0,1,0);
    ofRotate(rY,0,0,1);

    mesh.draw();

    glDisable(GL_LIGHTING);
   
    warpF.draw();
    warpS.draw();
    
    glDisable(GL_DEPTH_TEST);

    if(viewNum==0 || viewNum==1)cam.end();
    if(viewNum==2)Ecam.end();
    
    
    ofPopMatrix();
    
    if(viewNum==0){
        front.drawControls(warpCol,warpCol);
        if(LockXY==0)front.drawWireframe(false,true,false);//(lockX,lockY,lockH)
        if(LockXY==1)front.drawWireframe(true,false,false);//(lockX,lockY,lockH)

        ofSetColor(0,0,255,lineAlpha);
            ofNoFill();
                ofSetLineWidth(1.0);
                ofDrawRectangle(dW/3,dH/3,frontW,frontH);
            ofFill();
        ofSetColor(warpCol);
        
    }else if(viewNum==1){
        side.drawControls(warpCol,warpCol);
        side.drawWireframe(false,false,true);
        
        ofSetColor(0,0,255,lineAlpha);
            ofNoFill();
                ofSetLineWidth(1.0);
                ofDrawRectangle(dW/3,dH/3,frontW,sideH);
            ofFill();

        ofSetColor(warpCol);
    }
    
    if(showGui){
        drawInfo();
        gui.draw();
    }
    gui2.draw();

    ofDrawBitmapStringHighlight(name + "\n" + view,23, dH/16*1);
}

void ofApp::chairLoad(string path){
    chair.clear();
    chair.loadModel(path);//virtex10000
    chair.setPosition(0,0,0);
    chair.setScaleNormalization(false);
}
//--------------------------------------------------------------
void ofApp::reMapMesh(){
    mesh = chair.getCurrentAnimatedMesh(0);

    ofPushMatrix();
    
    vector<ofVec3f>& v = mesh.getVertices();
    vector<ofVec3f>& w = warpF.getVertices();
    vector<ofVec3f>& s = warpS.getVertices();
    
    int fw = frontW/2;
    int fh = frontH/2;
    
    area0 = ofVec2f(w[0].x,w[0].y);
    area1 = ofVec2f(w[res-1].x,w[res-1].y);
    area2 = ofVec2f(w[res*res-1].x,w[res*res-1].y);
    area3 = ofVec2f(w[res*(res-1)].x,w[res*(res-1)].y);
    
    int res1 = res-1;
    int res2 = res*res-res;
    int res3 = res1/2;
    int res4 = res2/2;

    //mesh vertices loop
    for(unsigned int i=0; i<v.size(); i+=1){

        //front meshX map
        for(unsigned int j=0;j<res1;j++){
            if(w[j].y<v[i].y && v[i].y<=w[j+1].y){
                v[i].x = ofMap(v[i].x, -fw, fw, w[j].x,w[(res2)+j].x,true);
            }
        }
        //front meshY map
        for(unsigned int k=0;k<res2;k+=res){
            if(w[k].x<v[i].x && v[i].x<=w[k+res].x){
                v[i].y = ofMap(v[i].y, -fh, fh, w[k].y, w[k+res1].y,true);
            }
        }

        for(unsigned int l=(res2);l>0;l-=res){
            if(s[l].y>=v[i].y && v[i].y>s[l-res].y){
                v[i].z = ofMap(v[i].z, 0, sideH,s[l+res1].z,s[l].z*-1);
            }
            if(l == res && s[0].y>v[i].y){
                v[i].z = ofMap(v[i].z, 0, sideH,s[0+res1].z,s[0].z*-1);
            }
        }
    }
    ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::drawInfo(){
    
    info="info\n";
    info+="fps "+ofToString(ofGetFrameRate())+"\n";
    info+="size "+ofToString(mesh.getNumVertices())+"\n";
    info+="mouse "+ofToString(mouseX)+" _ "+ofToString(mouseY)+"\n";
    
    info+="viewPoint"+ofToString(viewNum)+"\n";
    info+="timer"+ofToString(time)+"\n";
    info+="nextCam"+ofToString(nextCam.x)+","+ofToString(nextCam.y)+","+ofToString(nextCam.z)+"\n";
    info+="Dist"+ofToString(ofDist(camX,camY,camZ,nextCam.x,nextCam.y,nextCam.z))+"\n";
    
    info+="area0,"+ofToString(area0.x)+","+ofToString(area0.y)+"\n";
    info+="area1,"+ofToString(area1.x)+","+ofToString(area1.y)+"\n";
    info+="area2,"+ofToString(area2.x)+","+ofToString(area2.y)+"\n";
    info+="area3,"+ofToString(area3.x)+","+ofToString(area3.y);
    //    ofSetColor(200);
    ofDrawBitmapStringHighlight(info, dW/16*13, dH/16*12);
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case'g':
            grid=!grid;
            break;
        case 'h':
            showGui=!showGui;
            break;
        case'f':
            ofToggleFullscreen();
            break;
    }
}
//--------------------------------------------------------------
void ofApp::saveXmlF(bool load){
    xmlF.clear();
    xmlF.addChild("points");
    xmlF.setTo("points");
    vector<ofVec3f> vecF = front.getControlPnts();
    
    for (int i = 0; i<vecF.size(); i++) {
        xmlF.addChild("point");
        xmlF.setToChild(i);
        xmlF.setAttribute("xyz", ofToString(vecF[i].x) + "," + ofToString(vecF[i].y) + "," + ofToString(vecF[i].z));
        xmlF.setToParent();
    }
    
    xmlF.setToParent();
    xmlF.save("settingF.xml");
    if(load==true)loadXmlS();
}
//--------------------------------------------------------------
void ofApp::saveXmlS(bool load){
    xmlS.clear();
    xmlS.addChild("points");
    xmlS.setTo("points");
    vector<ofVec3f> vecS = side.getControlPnts();
    
    for (int i = 0; i<vecS.size(); i++) {
        xmlS.addChild("point");
        xmlS.setToChild(i);
        xmlS.setAttribute("xyz", ofToString(vecS[i].x) + "," + ofToString(vecS[i].y) + "," + ofToString(vecS[i].z));
        xmlS.setToParent();
    }
    
    xmlS.setToParent();
    xmlS.save("settingS.xml");
    
    if(load==true)loadXmlF();
}
//--------------------------------------------------------------
void ofApp::loadXmlF(){
    string str;
    vector<ofVec3f> vecF;
    xmlF.load("settingF.xml");
    
    for (int j = 0; j<xmlF.getNumChildren(); j++) {
        if (xmlF.exists("point["+ofToString(j)+"][@xyz]")) {
            str = xmlF.getAttribute("point["+ofToString(j)+"][@xyz]");
            int x = ofToInt(ofSplitString(str, ",")[0]);
            int y = ofToInt(ofSplitString(str, ",")[1]);
            int z = ofToInt(ofSplitString(str, ",")[2]);
            vecF.push_back(ofVec3f(x,y,z));
        }
    }
    front.setControlPnts(vecF);
}
//--------------------------------------------------------------
void ofApp::loadXmlS(){
    string strS,strF;
    vector<ofVec3f> vecF;
    vector<ofVec3f> vecS;
    xmlF.load("settingF.xml");
    xmlS.load("settingS.xml");
    
    for (int i = 0; i<xmlF.getNumChildren(); i++) {
        if (xmlF.exists("point["+ofToString(i)+"][@xyz]")) {
            strF = xmlF.getAttribute("point["+ofToString(i)+"][@xyz]");
            int x = ofToInt(ofSplitString(strF, ",")[0]);
            int y = ofToInt(ofSplitString(strF, ",")[1]);
            int z = ofToInt(ofSplitString(strF, ",")[2]);
            vecF.push_back(ofVec3f(x,y,z));
        }
    }
    
    for (int j = 0; j<xmlS.getNumChildren(); j++) {
        if (xmlS.exists("point["+ofToString(j)+"][@xyz]")) {
            strS = xmlS.getAttribute("point["+ofToString(j)+"][@xyz]");
            int x = ofToInt(ofSplitString(strS, ",")[0]);
            int y = ofToInt(ofSplitString(strS, ",")[1]);
            int z = ofToInt(ofSplitString(strS, ",")[2]);
            vecS.push_back(ofVec3f(x,y,z));
        }
        //vecFはfrontのcontrolPoint
        int addX = dW/3-dH/3;
        
        for(int k = 0;k<ctrP;k++){//0-3 ctrP=4
            for (int l = 0; l<vecS.size(); l++) {//0-15 ctrP=4
                if(ctrP*k<=l && l<ctrP*(k+1)){
                    vecS[l].x = vecF[k].y+addX;
                }
            }
        }
        side.setControlPnts(vecS);
    }
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x,int y,int button){
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    reMapMesh();
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::viewButtonPressed(){
    viewNum+=1;
    if(viewNum>2)viewNum=0;
    switch(viewNum){
        case 1:{
            view = "view Side";
            saveXmlF(true);
            camX = -1830;
            camY = 210;
            camZ = 30;
        }break;
            
        case 2:{
            view = "view Purse";
            saveXmlS(true);
            Ecam.setPosition(-960, 690, 930);
            Ecam.lookAt(chair.getPosition());
        }break;
            
        case 0:{
            view = "view Top";
            camX = 0;
            camY = 1860;
            camZ = 0;
            loadXmlF();
        }break;
    }
}

void ofApp::easingCam(){
    viewNum=2;
    
    if(time-ltime>200){
        nextCam = ofVec3f(ofRandom(-1500,1500),ofRandom(-1500,1500),ofRandom(-1500,1500));
        easing = ofVec3f(ofRandom(0.006,0.08),ofRandom(0.006,0.08),ofRandom(0.006,0.08));
        ltime=time;
    }
    
    EcamX = EcamX+(nextCam.x-EcamX)*easing.x;
    EcamY = EcamY+(nextCam.y-EcamY)*easing.y;
    EcamZ = EcamZ+(nextCam.z-EcamZ)*easing.z;
    
    Ecam.setPosition(EcamX, EcamY, EcamZ);
    Ecam.lookAt(ofVec3f(0));
}

void ofApp::modelPressed(){
    modelNum+=1;
    if(modelNum>4)modelNum=0;
    switch(modelNum){
            
        case 1:{
            name="model Thonet";
            chairLoad("model/tnt/tnt.stl");
        }break;
        case 2:{
            name="model Ant";
            chairLoad("model/ant/ant.stl");
        }break;
        case 3:{
            name = "model Panton";
            chairLoad("model/ptn/ptn.stl");
        }break;
        case 4:{
            name = "no Model";
            chairLoad("model/simple/simple2.stl");
        }break;
        case 0:{
            name = "mdoel Zigzag";
            chairLoad("model/zig/zig.stl");
        }break;
    }
    reMapMesh();
}
//--------------------------------------------------------------
void ofApp::cornerLockXY(){
    LockXY+=1;
    if(LockXY>1)LockXY=0;
    front.reset();
    side.reset();
    warpF.clear();
    warpS.clear();
    saveXmlF(false);
    saveXmlS(false);
    mesh = chair.getCurrentAnimatedMesh(0);
}

void ofApp::guiSetUp1(){
    gui.setup();
    gui.setPosition(dW/16,dH/16*4);
    gui.add(lpX.setup("lightPX",-780,-1500,1500));
    gui.add(lpY.setup("lightPY",950,-1500,1500));
    gui.add(lpZ.setup("lightPZ",1300,-1500,1500));
    gui.add(camX.setup("camPX",0,-3000,3000));
    gui.add(camY.setup("camPY",1830,-3000,3000));
    gui.add(camZ.setup("camPZ",1730,-3000,3000));
    gui.add(warpCol.setup("warp",230,0,255));
    gui.add(lineAlpha.setup("lineAlpha",70,0,255));
    gui.add(ambient.setup("ambient",ofColor(0,53,53,255),ofColor(0),ofColor(255)));
    gui.add(diffuse.setup("diffuse",ofColor(127,150,200,255),ofColor(0),ofColor(255)));
    gui.add(bg.setup("background",ofColor(253,255,255),ofColor(0,0,0),ofColor(255,255,255)));
}

void ofApp::guiSetUp2(){
    gui2.setup();
    gui2.setPosition(20, dH/16*1.5);
    gui2.add(modelButton.setup("Model"));
    gui2.add(viewButton.setup("View"));
    gui2.add(cornerLock.setup("CornerLock"));
    gui2.add(saveButton.setup("Save"));
    gui2.add(resetButton.setup("Reset"));
    
    //    gui2.add(Res.setup("Resolution",res,5,200));

}

//--------------------------------------------------------------
void ofApp::resolution(int r){
    front.setup(frontW,frontH,ctrP,r,dW/3,dH/3);
    side.setup(sideW,sideH,ctrP,r,dW/3,dH/5);
    glPointSize(ofMap(r, 5, 200, 5.0, 1.0));
    res=r;
    mesh = chair.getCurrentAnimatedMesh(0);
}

//--------------------------------------------------------------
void ofApp::savePressed(){
    for(int i=0;i<mesh.getNumVertices();i++){
        mesh.addColor(ofFloatColor(255,255,255));
    }
    string name = ofGetTimestampString();

//    mesh.save("/Volumes/idd-design06/Desktop/chairRecieve/data/save/"+name+".ply");//受信マシンのdeskTopの絶対パス
//    mesh.save("/Users/sakamotosenakira/Desktop/of_v0.9.8_osx_release/apps/master_03/chairRecieve/bin/data/save/"+name+".ply");//localパス
    
    mesh.save("save/"+name+".ply");
    warpF.save("warpFront/"+name+".ply");
    warpS.save("warpSide/"+name+".ply");
    mesh.clearColors();
    
//    ofxOscMessage m;
//    m.setAddress("/save/name");
//    m.addStringArg(name);
//    sender.sendMessage(m);
    
}

//--------------------------------------------------------------
void ofApp::resetPressed(){
    front.reset();
    side.reset();
    warpF.clear();
    warpS.clear();
    saveXmlF(false);
    saveXmlS(false);

    mesh = chair.getCurrentAnimatedMesh(0);
}

//--------------------------------------------------------------
void ofApp::exit(){
}
