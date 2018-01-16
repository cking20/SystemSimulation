/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TwoDTime.h
 * Author: chrisrk192
 *
 * Created on October 31, 2017, 6:41 PM
 */

#ifndef TWODTIME_H
#define TWODTIME_H

class TwoDTime {
public:
    TwoDTime();
    TwoDTime(double, int);
    TwoDTime(const TwoDTime& orig);
    virtual ~TwoDTime();
    double GetRealTime();
    int GetDiscreteTime();
private:
    double realTime;
    int discreteTime;
};

#endif /* TWODTIME_H */

