/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EventModelInterface.h
 * Author: chrisrk192
 *
 * Created on October 30, 2017, 8:50 PM
 */

#ifndef EVENTMODELINTERFACE_H
#define EVENTMODELINTERFACE_H

template <class I, class O>
class EventModelInterface {
public:
    EventModelInterface();
    EventModelInterface(const EventModelInterface& orig);
    virtual ~EventModelInterface();
    
    virtual O OutPut();    
    virtual void StateTransitionExt(I, double);
    virtual void StateTransitionInt();
    virtual void StateTransitionCon(I);
    virtual double TimeAdvance();
    virtual O GetStatus();
private:

};
/**
 public interface EventModelInterface<I, O>{
    O OutPut();    
    void StateTransitionExt(I inputItems, double e) throws CantPerformAction;
    void StateTransitionInt() throws CantPerformAction;
    void StateTransitionCon(I inputItems) throws CantPerformAction;
    double TimeAdvance();
    public O GetStatus();
    
}

 */
#endif /* EVENTMODELINTERFACE_H */

