/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EventScheduler.h
 * Author: chrisrk192
 *
 * Created on November 16, 2017, 11:40 PM
 */

#ifndef EVENTSCHEDULER_H
#define EVENTSCHEDULER_H

#include "Event.h"
#include <vector>
#include <iostream>
template<class I, class O>
class EventScheduler {
    
public:
    EventScheduler() {
        theQueue;// = new ArrayList<>();
    }
    ~EventScheduler(){
        for(int i = 0; i < theQueue.size(); i++){
            delete theQueue.at(i);
        }
    }
    
    bool isEmpty(){
        return theQueue.size() == 0;
    }
    
    void Add(Event<I, O>* e){
        FilterIn(e);
    }
    Event<I, O>* Peek(){
        MergeIntExt();
        return theQueue.front();
    }
    Event<I, O>* Pop(){
        MergeIntExt();
        Event<I, O>* ret = theQueue.at(0);
        theQueue.erase(theQueue.begin());
        return ret;
    }
      
    //should be called on Add
    void MergeIntExt(){
        //for each real time merge a model's 
        //internal and external event to a confluent
            //System.out.println("--------------------Before Merge----------------------------");
            //Print();
        
            int j,initialIndex;
            j = 0;
            initialIndex = 0;
            bool gotInternal = false;
            bool gotExternal = false;
            Event<I, O>* iEvent = theQueue.at(initialIndex);
            if(iEvent->typeOfEvent == Event<I,O>::internal){
                gotInternal = true;
            }else if(iEvent->typeOfEvent == Event<I,O>::external){
                gotExternal = true; 
            }
            TwoDTime* iTime = iEvent->myTime;
            
            
            while(j < theQueue.size() && theQueue.at(j)->myTime->compareTo(iTime) == 0){
                //if the model is the same, and have internal and external events
                if(iEvent->model == theQueue.at(j)->model){
                    if(theQueue.at(j)->typeOfEvent == Event<I,O>::internal)
                        gotInternal = true;
                    else if(theQueue.at(j)->typeOfEvent == Event<I,O>::external)
                        gotExternal = true; 
                    if(gotExternal && gotInternal){
                        std::cout << "SCHEDULE FOUND CONFLUENT";
                        //System.out.println("initialIndex = "+initialIndex+" j="+j);
                        Event<I, O>* conEvent;// = new Event();
                        conEvent->typeOfEvent = Event<I,O>::confluent;
                        conEvent->model = theQueue.at(j)->model;
                        conEvent->myTime = theQueue.at(j)->myTime;
                        if(theQueue.at(j)->typeOfEvent == Event<I,O>::internal)
                            conEvent->timeQueued = theQueue.at(j)->timeQueued;
                        else
                            conEvent->input = theQueue.at(j)->input;
                        
                        if(theQueue.at(initialIndex)->typeOfEvent == Event<I,O>::internal)
                            conEvent->timeQueued = theQueue.at(initialIndex)->timeQueued;
                        else
                            conEvent->input = theQueue.at(initialIndex)->input;
                            
                        if(initialIndex > j){
                            theQueue.erase(theQueue.begin()+j);
                            theQueue.erase(theQueue.begin()+(initialIndex-1));
                        } else{   
                            theQueue.erase(theQueue.begin()+initialIndex);
                            theQueue.erase(theQueue.begin()+(j-1));
                        }
                        std::cout << "Filtering In";
                        
                        theQueue.insert(theQueue.begin(),conEvent);
                        gotExternal = false;
                        gotInternal = false;
                    }
                }  
                j++;
            }
            //System.out.println("--------------------After Merge----------------------------");
            //Print();
    }
    //for deleting old internal events
    Event<I,O>* DeleteOldInternals(EventModelInterface<I,O>* e){
        Event<I,O>* ret = NULL;
        
         for(int i = 0; i < theQueue.size();i++){
             Event<I, O>* ev = theQueue.at(i);
             if(ev->model == e && ev->typeOfEvent == Event<I,O>::internal){
                 ret = theQueue.at(i);
                 theQueue.erase(theQueue.begin()+i);
             }            
         }    
         return ret;
    }
    
    void Print(){
        for(int i = 0;i < theQueue.size();i++){
            std::cout << theQueue.at(i)->Print() << std::endl;
        }
    
    }
    
private: 
    std::vector<Event<I, O>*> theQueue;
    //further times are at the end
    void FilterIn(Event<I, O>* e){
        if(theQueue.size() == 0){
            theQueue.push_back(e);
            return;
        }
        for(int i = 0; i < theQueue.size();i++){
            if(theQueue.at(i)->myTime->compareTo(e->myTime) > 0){
                theQueue.insert(theQueue.begin()+i,e);
                return;
            }
        }   
        theQueue.insert(theQueue.end(), e);
    }
};

#endif /* EVENTSCHEDULER_H */

