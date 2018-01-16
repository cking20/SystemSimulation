/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mem.h
 * Author: chrisrk192
 *
 * Created on October 27, 2017, 10:24 PM
 */

#ifndef MEM_H
#define MEM_H

#include "ModelInterface.h"


class Mem :public ModelInterface{
public:
    Mem();
    Mem(const Mem& orig);
    virtual ~Mem();
    
    std::string GetInitilizationSignature() override;
    void Initialize(std::vector<std::string>)override;
    std::vector<std::string> OutPut()override;  
    void StateTransition(std::vector<std::string>)override;
    std::string GetStatus()override;
    std::vector<std::string> Tick(std::vector<std::string>)override;
private:
    int qHead;
    int qTail;
};

#endif /* MEM_H */

