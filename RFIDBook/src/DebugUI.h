//
//  DebugUI.h
//  RFIDBook
//
//  Created by Cameron Browning on 6/14/13.
//
//

#ifndef __RFIDBook__DebugUI__
#define __RFIDBook__DebugUI__

#include "ofxSimpleGuiToo.h"
#include "ofMain.h"
#include "TextField.h"
#include "PositionUI.h"
#include "BarGraph.h"
#include "DeviceController.h"
#include "BookController.h"
#include "BookView.h"
#include "BookLoader.h"
#include <iostream>

class DebugUI{
  
public:
    DebugUI();
    ~DebugUI();
    
    void setup(DeviceController * _devices, BookController * _book, BookView * _bookView, BookLoader *_loader);
    void update();
    void draw();
    
    void show();
    void hide();
    void toggle();
    
    bool getIsVisible();
    bool getDragSetting();
    void toggleFullScreen();
     
private:
    BookView * bookView;
    bool bDragUIcheckbox;
    bool bDragUIcheckbox_prev;
    bool saveXMLbtn;
    bool loadXMLbtn;
    bool defaultXMLbtn;
    bool isVisible;
    bool bFullScreencheckbox;
    bool bFullScreencheckbox_prev;
    
    DeviceController * devices;
    BookController * book;
    BookLoader * loader;
    TextField tfield;
    PositionUI pos_ui;

};

#endif /* defined(__RFIDBook__DebugUI__) */
