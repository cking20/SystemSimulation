/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EventModelInterface.h
 * Author: chrisrk192
 *
 * Created on November 7, 2017, 2:16 PM
 */
#include <string>
#include <vector>
#ifndef EVENTMODELINTERFACE_H
#define EVENTMODELINTERFACE_H

class EventModelInterface {
public:
    EventModelInterface();
    EventModelInterface(const EventModelInterface& orig);
    virtual ~EventModelInterface();
    
    virtual std::vector<std::string> OutPut();    
    virtual void StateTransitionExt(std::vector<std::string>, double);
    virtual void StateTransitionInt();
    virtual void StateTransitionCon(std::vector<std::string>);
    virtual double TimeAdvance();
    virtual std::string GetStatus();
private:

};

struct CantPerformEventAction : public std::exception {
public:
    CantPerformEventAction(std::string msg) : m_message(msg) { }
    std::string getMessage(){
        return m_message;
    }
private:
    std::string m_message;
};
#endif /* EVENTMODELINTERFACE_H */

