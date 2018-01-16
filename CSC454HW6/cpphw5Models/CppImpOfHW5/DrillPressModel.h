/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DrillPressModel.h
 * Author: chrisrk192
 *
 * Created on November 20, 2017, 9:15 PM
 */

#ifndef DRILLPRESSMODEL_H
#define DRILLPRESSMODEL_H
#include <vector>
#include <limits>
#include <string>
#include "EventModelInterface.h"


class DrillPressModel:public EventModelInterface<int,int> {
public:
    DrillPressModel();
    DrillPressModel(int, double);
    DrillPressModel(const DrillPressModel& orig);
    virtual ~DrillPressModel();
    
    virtual int OutPut() override;    
    virtual void StateTransitionExt(int, double) override;
    virtual void StateTransitionInt() override;
    virtual void StateTransitionCon(int) override;
    virtual double TimeAdvance() override;
    virtual std::string GetStatus() override;
private:
    int p;
    double s;
    double t;
};

#endif /* DRILLPRESSMODEL_H */

