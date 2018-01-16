/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EventModelInterface.cpp
 * Author: chrisrk192
 * 
 * Created on November 7, 2017, 2:16 PM
 */
#include <string>
#include <vector>
#include "EventModelInterface.h"

EventModelInterface::EventModelInterface() {
}

EventModelInterface::EventModelInterface(const EventModelInterface& orig) {
}

EventModelInterface::~EventModelInterface() {
}

std::vector<std::string> EventModelInterface::OutPut(){

}   

void EventModelInterface::StateTransitionExt(std::vector<std::string> inputItems, double e){

}

void EventModelInterface::StateTransitionInt(){

}


void EventModelInterface::StateTransitionCon(std::vector<std::string> inputItems){

}

double EventModelInterface::TimeAdvance(){
}


std::string EventModelInterface::GetStatus(){

}