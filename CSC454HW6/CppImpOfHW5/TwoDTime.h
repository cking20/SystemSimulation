/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoDTime.h
 * Author: chrisrk192
 *
 * Created on November 16, 2017, 11:46 PM
 */

#ifndef TWODTIME_H
#define TWODTIME_H

class TwoDTime{
public:
     TwoDTime(double d, int i) {
        realTime = d;
        discreteTime = i;
    }
    TwoDTime(double d) {
        realTime = d;
        discreteTime = 0;
    }
    TwoDTime() {
        realTime = 0.0;
        discreteTime = 0;
    }
    TwoDTime(const TwoDTime& orig){}
    virtual ~TwoDTime(){}

    double getRealTime() {
        return realTime;
    }

    int getDiscreteTime() {
        return discreteTime;
    }
    
    TwoDTime* Advance(TwoDTime* interval){
        if (interval->realTime > 0) {
            TwoDTime *t = new TwoDTime(realTime+interval->realTime, 0);
            return t;
        } else{
            TwoDTime *t = new TwoDTime(realTime, discreteTime+interval->discreteTime);
            return t;
        }
    }  
 
    int compareTo(TwoDTime* that) {
        if(that->realTime == realTime){
            if(that->discreteTime == discreteTime)
                return 0;
            if(discreteTime < that->discreteTime)
                return -1;
            return 1;
        } else if(realTime < that->realTime)
            return -1;
        //else greater    
        return 1;       
    }
private:
    double realTime;
    int discreteTime;
};


#endif /* TWODTIME_H */

