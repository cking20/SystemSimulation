/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoDTime.cpp
 * Author: chrisrk192
 * 
 * Created on October 31, 2017, 6:41 PM
 */

#include "TwoDTime.h"

TwoDTime::TwoDTime() {
}

TwoDTime::TwoDTime(const TwoDTime& orig) {
}

TwoDTime::~TwoDTime() {
}
TwoDTime::TwoDTime(double real, int disc) {
    realTime = real;
    discreteTime = disc;
}
double TwoDTime::GetRealTime(){
    return realTime;
}
int TwoDTime::GetDiscreteTime(){
    return discreteTime;
}
