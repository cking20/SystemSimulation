/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DrillPressModel.cpp
 * Author: chrisrk192
 * 
 * Created on November 20, 2017, 9:15 PM
 */

#include "DrillPressModel.h"

DrillPressModel::DrillPressModel() {
}
DrillPressModel::DrillPressModel(int new_p, double new_t) {
    p = new_p;
    t = new_t;
    s = new_t;
}
DrillPressModel::DrillPressModel(const DrillPressModel& orig) {
}

DrillPressModel::~DrillPressModel() {
}

int DrillPressModel::OutPut() {
    return 1;
}    

void DrillPressModel::StateTransitionExt(int inputItems, double e) {
    if(p > 0){
            p += inputItems;
            s -= e;
        } else{
            p+= inputItems;
            s = t;
        }
}

void DrillPressModel::StateTransitionInt() {
    p--;
    s = t;
}

void DrillPressModel::StateTransitionCon(int inputItems) {
    p += inputItems - 1;
    s = t;
}

double DrillPressModel::TimeAdvance() {
    if(p > 0)
        return s;
    else 
        return std::numeric_limits<double>::infinity();
}

std::string DrillPressModel::GetStatus() {
    return "Op Time:"+std::to_string(t)+" numParts="+std::to_string(p)+" timeToFinish1stPart="+std::to_string(s);
}
