/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   XORModel.h
 * Author: chrisrk192
 *
 * Created on October 27, 2017, 10:21 PM
 */

#ifndef XORMODEL_H
#define XORMODEL_H


#include "EventModelInterface.h"


class XOR : public EventModelInterface<std::vector<std::string>,std::vector<std::string>>{
public:
    XOR();
    XOR(const XOR& orig);
    virtual ~XOR();
       
    //std::string GetInitilizationSignature() override;
    //void Initialize(std::vector<std::string>)override;
    std::vector<std::string> OutPut()override;  
    void StateTransitionExt(std::vector<std::string>, double)override;
    void StateTransitionCon(std::vector<std::string>)override;
    void StateTransitionInt();
    
    std::string GetStatus()override;
    //std::vector<std::string> Tick(std::vector<std::string>)override;
    double TimeAdvance() override;

private:
    int state;

};

#endif /* XORMODEL_H */

