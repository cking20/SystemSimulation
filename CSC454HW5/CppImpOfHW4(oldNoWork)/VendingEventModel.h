/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VendingEventModel.h
 * Author: chrisrk192
 *
 * Created on October 30, 2017, 9:47 PM
 */

#ifndef VENDINGEVENTMODEL_H
#define VENDINGEVENTMODEL_H
#include <string>
#include <vector>
#include "EventModelInterface.h"

//<std::vector<std::string>,std::vector<std::string>>
class VendingEventModel : public EventModelInterface{
public:
    enum ioSet{n, d, q, coffee} ;
    double e;
    
    VendingEventModel();
    VendingEventModel(const VendingEventModel& orig);
    virtual ~VendingEventModel();
    
    VendingEventModel(int , int , int , int );
    std::vector<std::string> OutPut() override;    
    void StateTransitionExt(std::vector<std::string>, double) override;
    void StateTransitionInt() override;
    void StateTransitionCon(std::vector<std::string>) override;
    double TimeAdvance() override;
    std::vector<std::string> GetStatus() override;
   
private:
    int qNum,nNum,dNum,vNum;
    std::vector<int>* CalcChange(int, int , int , int );
    int StringToEnum(std::string);
    std::string EnumToString(int);
    std::vector<std::string> SplitString(std::string,std::string);
};

#endif /* VENDINGEVENTMODEL_H */

