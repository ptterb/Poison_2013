//
//  Page.cpp
//  RFIDBook
//
//  Created by Brett Peterson on 6/10/13.
//
//

#include "Page.h"

Page::Page(){
    doDrag = false;
}
Page::~Page(){
    media.clear();
}

void Page::setup(){
    
    validInputs.push_back('H');
    validInputs.push_back('J');
    validInputs.push_back('R'); // running
    validInputs.push_back('C'); // clear all params
    
    touchMedia0.push_back(5);
    touchMedia0.push_back(6);
    touchMedia0.push_back(7);
    touchMedia0.push_back(10);
    
    touchMedia1.push_back(1);

    touchMedia2.push_back(1);
    
    touchMedia3.push_back(1);
}

void Page::setDrag(bool _doDrag){
    doDrag = _doDrag;
}

// Update all media elements on page


void Page::hideAllBorders(){
    for (int i = 0; i < media.size(); i++) {
        media.at(i)->setDraggable(false);
    }
}

void Page::dragUpdate(){

    // add toggleable draggy stuff
    if(!doDrag){
        // cout << this;
        ofPoint mousePos = ofPoint(ofGetMouseX(),ofGetMouseY());
        float nearestDist = 99999;
        int nearestID = -1;
        for (int i = 0; i < media.size(); i++) {
            ofPoint thisOrigin = media.at(i)->getPosition();
           // cout << thisOrigin.x << ", " << thisOrigin.y << endl;
            float thisDist = ofDist(thisOrigin.x,thisOrigin.y,mousePos.x,mousePos.y);
           // cout << "this dist is: " << thisDist << endl;
            if(thisDist<nearestDist){
                nearestID = i;
                nearestDist = thisDist;
            }
            media.at(i)->setDraggable(false);
        }
       // cout << "nearest dist is: " << nearestDist << endl;
        if(nearestID>-1){
            media.at(nearestID)->setDraggable(true);
            selectedMedia = media.at(nearestID);
        }
    } else {
      //  cout << "I am dragging now." << endl;
        if(selectedMedia!=NULL){
            selectedMedia->moveTo(ofGetMouseX(),ofGetMouseY());
        }
    }
}
void Page::update(){
    for (int i = 0; i < media.size(); i++) {
        media.at(i)->update();
    }
    
    
}

void Page::draw(float originX, float originY, float scale){
    
    if (scale == 1.0f) {
        
        // Run the normal draw method for each media element
        for (int i = 0; i < media.size(); i++) {
            media.at(i)->draw();
        }
        
    }
    else {
        // Run the scaled draw method for each media element
        for (int i = 0; i < media.size(); i++) {
            media.at(i)->draw(scale);
        }
    }
    
}

void Page::addMedia(string fileName, ofVec2f position, int autoplay, string tapId, int loopback){
    
    Media * newMedia = new Media();
    
    string sub = fileName.substr(fileName.length()-3,3);
    if (sub == "png") {
        
        // run the setup for a media element that is just an image
        newMedia->setup(fileName, position.x, position.y);
        
    }
    else if (sub == "mov"){
        
        // Run the setup for a media element that is a video and an image
        string imageFile = fileName;
        imageFile.replace(fileName.length() -3, 3, "png");
        newMedia->setup(imageFile, fileName, position.x, position.y, autoplay, tapId, loopback);
        
    }
    else {
        
        ofLogNotice() << "unrecognized media file extension: " << fileName;
    
    }
    
    media.push_back(newMedia);
    ofLogNotice() << "Added new media element " << fileName << " to page at position " << position.x << "," << position.y;
    
}

void Page::receiveInput(char touchId_in, int pageNum_in){

    int position = -1;
    
    // Look for character in valid inputs vector
    for (int i = 0; i < validInputs.size(); i++) {

        // Hide and elements according to which page we're on and which sensor was touched.
        
        if (validInputs.at(i) == touchId_in){
            
            // TODO HARDCODE EACH MEDIA ELEMENT THAT NEEDS TO FADE OUT OR PLAY ON TOUCH
                
            if (pageNum_in == 0){
                
                if (touchActive == false){
                    for (int i=0; i < touchMedia0.size(); i++){
                        media.at(touchMedia0[i])->img.fadeOut();
                        media.at(touchMedia0[i])->vid.fadeOut();
                    }

                    if (touchId_in == 'H'){
                        currentTouch = 'H';
                        touchActive = true;
                        activeMedia = 12;

                        media.at(activeMedia)->vid.fadeIn();
                        media.at(activeMedia)->playVid();
                       
                
                    } else if (touchId_in == 'J') {
                        currentTouch = 'J';
                        touchActive = true;
                        activeMedia = 13;
                        
                        media.at(activeMedia)->playVid();
                        media.at(activeMedia)->vid.fadeIn();

                    }
                }
                
                if (touchActive){
                    if (currentTouch == 'H' || currentTouch == 'J' || currentTouch == 'R'){
                        
                        int currentFrame = media.at(activeMedia)->vid.getCurrentFrame();
                        int lastFrame = media.at(activeMedia)->vid.getTotalNumFrames();
                        
                        if (currentFrame == lastFrame){
                            media.at(activeMedia)->vid.fadeOut();
                            media.at(activeMedia)->pauseVid();
                            media.at(activeMedia)->vidState = 0;
                            
                            for (int i=0; i < touchMedia0.size(); i++){
                                media.at(touchMedia0[i])->img.fadeIn();
                                media.at(touchMedia0[i])->vid.fadeIn();
                            }
                            touchActive = false;
                            currentTouch = 'R';
                        }
                    }
                }
            }
            
            
          currentTouch = touchId_in;
        }

    
    
  
    }
}





void Page::fade(int dir){
    
    // Loop through media elements and fade them in or out
    float minFadeIn = 4.0f;
    float maxFadeIn = 64.0f;
    float minFadeOut = 2.0f;
    float maxFadeOut = 8.0f;
    
    if (dir == 1) {
        for (int i = 0; i < media.size(); i++) {
            // TODO - handle video fade in as well
            float fadeVal = ofRandomuf()*(maxFadeIn-minFadeIn)+minFadeIn;
            //cout << fadeVal << endl;
            media.at(i)->img.fadeIn(fadeVal);
            media.at(i)->vid.fadeIn(fadeVal);
            
            // If autoplay is on for the video, start playing
            
            if (media.at(i)->hasVid == true){
                if (media.at(i)->autoplay == 1){
                    media.at(i)->playVid();
                }
            }
        }
    }
    else {
        for (int i = 0; i < media.size(); i++) {
            // TODO - handle video fade out as well
           
            float fadeVal = ofRandomuf()*(maxFadeOut-minFadeOut)+minFadeOut;
            // cout << fadeVal << endl;
            
            media.at(i)->img.fadeOut(fadeVal);
            media.at(i)->vid.fadeOut(fadeVal);
            media.at(i)->vidState = 0;
            // stop all video
            media.at(i)->pauseVid();
        }
    }
    
}
ofxXmlSettings Page::getXML(){
    ofxXmlSettings xml;
    xml.addTag("Page");
    xml.pushTag("Page");
    for(int i=0;i<media.size();i++){
        xml.addTag("Media");
        ofPoint pt = media.at(i)->getPosition();
        xml.setAttribute("Media", "x", (int)pt.x,i);
        xml.setAttribute("Media", "y", (int)pt.y,i);
        xml.setAttribute("Media", "src", media.at(i)->getFileName(),i);
        xml.setAttribute("Media", "auto", (int)media.at(i)->autoplay, i);
        xml.setAttribute("Media", "tapId", (string)media.at(i)->tapId, i);
        xml.setAttribute("Media", "loopback", (int)media.at(i)->loopback, i);

        
        
    }
    return xml;
}



