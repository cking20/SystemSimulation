/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OutputTimePairs.h
 * Author: chrisrk192
 *
 * Created on November 16, 2017, 11:40 PM
 */

#ifndef OUTPUTTIMEPAIRS_H
#define OUTPUTTIMEPAIRS_H
#include "TwoDTime.h"
template<class Data>
class OutputTimePairs{
public:
    OutputTimePairs(){
        data;
        time;
        discrete_time = 0;
    }
    OutputTimePairs(const OutputTimePairs& orig){}
    virtual ~OutputTimePairs(){}
    
    OutputTimePairs(double t,Data d) {
        data = d;
        time = t;
    }
    OutputTimePairs(double t,int dis,Data d) {
        data = d;
        time = t;
        discrete_time = dis;
    }

    Data getData() {
        return data;
    }
    
    double getTime() {
        return time;
    }
    int getDiscreteTime(){
        return discrete_time;
    }
private:
    Data data;
    double time;
    int discrete_time;
};


#endif /* OUTPUTTIMEPAIRS_H */

