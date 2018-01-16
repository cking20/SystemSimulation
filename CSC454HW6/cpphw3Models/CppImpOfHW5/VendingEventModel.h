/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VendingEventModel.h
 * Author: chrisrk192
 *
 * Created on November 7, 2017, 7:22 PM
 */

#ifndef VENDINGEVENTMODEL_H
#define VENDINGEVENTMODEL_H
#include <string>
#include <vector>
#include "EventModelInterface.h"


class VendingEventModel: public EventModelInterface<std::vector<std::string>,std::vector<std::string>>  {
public:
    enum ioSet{n, d, q, coffee};
    double e;
    
    
    VendingEventModel();
    VendingEventModel(const VendingEventModel& orig);
    virtual ~VendingEventModel();
    
    virtual std::vector<std::string> OutPut() override;    
    virtual void StateTransitionExt(std::vector<std::string>, double) override;
    virtual void StateTransitionInt() override;
    virtual void StateTransitionCon(std::vector<std::string>) override;
    virtual double TimeAdvance() override;
    virtual std::string GetStatus() override;
private:
    int numQuarters, numNickels, numDimes, totalVal;
    
    std::vector<int>* CalcChange(int, int , int , int );
    int StringToEnum(std::string);
    std::string EnumToString(int);
    std::vector<std::string> SplitString(std::string,std::string);

};

#endif /* VENDINGEVENTMODEL_H */

