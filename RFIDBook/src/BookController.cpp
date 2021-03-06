//
//  BookController.cpp
//  RFIDBook
//
//  Created by Cameron Browning on 5/16/13.
//
//

#include "BookController.h"


BookController::BookController(){
    isSetup = false;
    useRFID = true;
    forcedState = "A";
    //lastTouchPage = -1;
    checkedForRFIDTimeout = false;
    for(int i=0;i<NUM_TOUCHES;i++){
        touchStates[i] = prevTouchStates[i] = false;
    }
}
BookController::~BookController(){
    
}

void BookController::setup(DeviceController * deviceController_in, BookView * bookView_in){
    deviceController = deviceController_in;
    bookView = bookView_in;
    isSetup = true;
}

void BookController::mousePressed(){
    bookView->mousePressed();
}
void BookController::mouseReleased(){
    bookView->mouseReleased();
}

void BookController::update(){
    
    ///////////////////////////////////////////
    // RFID / Page turning stuff
    ///////////////////////////////////////////
    // if after a number of seconds defined in the header
    // don't use the RFID sensors (for testing).
    ///////////////////////////////////////////
    
    if(ofGetElapsedTimeMillis()>(RFID_TIMEOUT*1000) && !checkedForRFIDTimeout){
        if(!deviceController->hasSeenRFID()){
            ofLogWarning() << "Haven't seen the RFID sensors for " << RFID_TIMEOUT << " second(s). Using manual mode.";
            useRFID = false;
            checkedForRFIDTimeout = true;
        }
    }
    
    string currentSitation;
    currentSitation = whatSituation();
    
    
    //currentTouch = touchSituation();
    
    if(currentSitation.length() == 1){
        // page is landed
        
        // take A-D and make 0-3
        char curSit_char = currentSitation[0];
        int whichPageNum = (int)curSit_char - 65;

        //bookView->mediaPages.at(whichPageNum)->receiveInput(currentTouch, whichPageNum);
        
        // send the view activation.
        bookView->activate(whichPageNum);
        
        } else if(currentSitation.length()==2){
        // you're on an interstitial
        
        bookView->deactivate();
        
        
        } else {
        // seems like an error...
        cout << "Error: Current Book Situation has a strange length: " << currentSitation.length() << endl;
    }
    //lastTouchPage = currentSitation;
    
    ///////////////////////////////////////////
    ///////////////////////////////////////////
    
    //
    ///////////////////////////////////////////
    // TOUCH STUFF
    ///////////////////////////////////////////
    //
    for(int i=0;i<NUM_TOUCHES;i++){
        // Check for touches if the kit is connected
        if (deviceController->kitConnected == true){
            touchStates[i] = deviceController->getTouchBool(i);
            if(touchStates[i]!=prevTouchStates[i]){
                // a sensor changed.
                if(touchStates[i]){
                    // it was a touch
                    bookView->touch(i);
                } else {
                    // it was a release
                    bookView->release(i);
                }
            }
            prevTouchStates[i] = touchStates[i];
        }
    }

    


}
bool BookController::isPageLanded(){
   int howManyActive = deviceController->getActiveSensorCount();  
    
    if(howManyActive==3){
        return true;
    } else {
        return false;
    }
}
string BookController::whatSituation(){
    string returnval_str = "";

    if (useRFID){
        
        if(deviceController->getSensor("top-right")->hasTag()){
            // if page one is there, then it's definitely on a page...
            returnval_str = "A";
            //bookView->activate(0);
        } else if(deviceController->getSensor("middle-right")->hasTag()){
            // page one not down, but page 2 is.
            if(deviceController->getSensor("top-left")->hasTag()){
                returnval_str = "B";
                //bookView->activate(1);
            } else {
                returnval_str = "AB";
            }
        } else if(deviceController->getSensor("bottom-right")->hasTag()){
            // first two pages are not on the right, but the third is.
            if(deviceController->getSensor("middle-left")->hasTag()){
                returnval_str = "C";
               // bookView->activate(2);
            } else if(deviceController->getSensor("top-left")->hasTag()){
                returnval_str = "BC";
            } else {
                returnval_str = "AC";
            }
        } else if(deviceController->getSensor("bottom-left")->hasTag()){
            returnval_str = "D";
            //bookView->activate(3);
        } else if(deviceController->getSensor("middle-left")->hasTag()){
            returnval_str = "CD";
        } else if(deviceController->getSensor("top-left")->hasTag()){
            returnval_str = "BD";
        } else {
            returnval_str = "AD";
        }
    } else {
        returnval_str = forcedState;
    }
    
    return returnval_str;
}

char BookController::touchSituation(){
    char returnval_char = ' ';
    
    if (useRFID){
        if (deviceController->hasTouch(0) == true){
            returnval_char = 'H';
        } else if (deviceController->hasTouch(1) == true){
            returnval_char = 'J';
        } else {
            returnval_char = '0';

        }
    } else {
        returnval_char = forcedTouchState;
    }
    
    return returnval_char;
}

string BookController::getReport(){
    string report_str;
    if(isPageLanded()){
        report_str = "pages landed";
    } else {
        report_str = "fade out pages";
    }
    return report_str;
}

void BookController::forcedInput(char _keypress){     //represent RFID actions with keypress
  
    switch(_keypress){
        case 'a':
        case 'A':
            
        forcedState = "A";
        break;
            
        case 'b':
        case 'B':
            
        forcedState = "B";
        break;
            
        case 'c':
        case 'C':
            
        forcedState = "C";
        break;
            
        case 'd':
        case 'D':
            
        forcedState = "D";
        break;
        
        case 'e':
        case 'E':
            
        forcedState = "AB";
            
        break;
            
        // Fake touch input
        case 'h':
        case 'H':

        forcedTouchState = 'H';
        break;
            
        case 'j':
        case 'J':
            
        forcedTouchState = 'J';
        break;

        
        case 'x':
        case 'X':
        forcedState = "AB";
        useRFID = !useRFID;
        break;
            
        
            
    }
}