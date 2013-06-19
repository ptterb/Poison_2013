//
//  BookLoader.h
//  RFIDBook
//
//  Created by Brett Peterson on 6/18/13.
//
//

#ifndef __RFIDBook__BookLoader__
#define __RFIDBook__BookLoader__

#include <iostream>
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "LanguageController.h"


struct XmlPage {
    vector<string> media;
    vector<ofVec2f> position;
    
};

class BookLoader{
    
public:
    
    BookLoader();
    ~BookLoader();
    
    vector<XmlPage> load();
    
    ofxXmlSettings bookElements;
    
    string fileName = "settings/book.xml";
    
    vector<XmlPage> pages;
    
    LanguageController lang;
    
    
};

#endif /* defined(__RFIDBook__BookLoader__) */