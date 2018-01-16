/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EventModelInterface.cpp
 * Author: chrisrk192
 * 
 * Created on October 30, 2017, 8:50 PM
 */

#include "EventModelInterface.h"
template<class I, class O>
EventModelInterface<I,O>::EventModelInterface() {
}
template<class I, class O>
EventModelInterface<I,O>::EventModelInterface(const EventModelInterface& orig) {
}
template<class I, class O>
EventModelInterface<I,O>::~EventModelInterface() {
}

template<class I, class O>
O EventModelInterface<I,O>::OutPut(){

}   

template<class I, class O>
void EventModelInterface<I, O>::StateTransitionExt(I inputItems, double e){

}

template<class I, class O>
void EventModelInterface<I, O>::StateTransitionInt(){

}

template<class I, class O>
void EventModelInterface<I, O>::StateTransitionCon(I inputItems){

}

double TimeAdvance(){
}

template<class I, class O>
O EventModelInterface<I, O>::GetStatus(){

}