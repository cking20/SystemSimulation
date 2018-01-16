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

#include <limits>
#include "EventModelInterface.h"


class Mem :public EventModelInterface<std::vector<std::string>,std::vector<std::string>>{
public:
    Mem();
    Mem(const Mem& orig);
    virtual ~Mem();
    
      
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
    int qHead;
    int qTail;
    bool tailEmpty;
    bool headEmpty;
};

#endif /* MEM_H */

