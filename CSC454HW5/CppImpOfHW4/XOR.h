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

#include "ModelInterface.h"


class XOR : public ModelInterface{
public:
    XOR();
    XOR(const XOR& orig);
    virtual ~XOR();
       
    std::string GetInitilizationSignature() override;
    void Initialize(std::vector<std::string>)override;
    std::vector<std::string> OutPut()override;  
    void StateTransition(std::vector<std::string>)override;
    std::string GetStatus()override;
    std::vector<std::string> Tick(std::vector<std::string>)override;
private:
    int state;

};

#endif /* XORMODEL_H */

