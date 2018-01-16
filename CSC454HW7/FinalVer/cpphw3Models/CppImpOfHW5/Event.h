/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Event.h
 * Author: chrisrk192
 *
 * Created on November 16, 2017, 11:40 PM
 */

#ifndef EVENT_H
#define EVENT_H

#include "TwoDTime.h"
#include "EventModelInterface.h"
#include <string>


template<class I, class O>
class Event{
public:
    enum type{internal,external,confluent};
    TwoDTime *myTime;
    EventModelInterface<I,O>* model;
    type typeOfEvent;
    I input;
    TwoDTime *timeQueued;
    Event(){
       // myTime;
        model = NULL;
        input;
        //timeQueued;
        myTime = NULL;
        timeQueued = NULL;
    }
    Event(const Event& orig){}
    virtual ~Event(){
        if(timeQueued != NULL){
        delete timeQueued;
        }
        if(myTime != NULL){
            delete myTime;
        }
    }
    
    std::string Print(){
        
        return "Event. Time: "
                +std::to_string(myTime->getRealTime())+","
                +std::to_string(myTime->getDiscreteTime())//+" Model: "
                +" Status: "
                +model->GetStatus()+". Type: "
                +std::to_string(typeOfEvent);//+" Input: "
                //+std::to_string(input);
    }
private:
};



#endif /* EVENT_H */

