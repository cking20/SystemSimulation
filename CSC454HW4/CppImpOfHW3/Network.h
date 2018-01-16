/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Network.h
 * Author: chrisrk192
 *
 * Created on October 18, 2017, 10:35 PM
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <vector>
#include "ModelInterface.h"
#include "Binding.h"

class Network: public ModelInterface{
public:
    int numInternalTicksToExternal;
    Network();
    Network(const Network& orig);
    virtual ~Network();
 
    std::string GetInitilizationSignature() override;
    void Initialize(std::vector<std::string>) override;
    std::vector<std::string> OutPut()override;  
    void StateTransition(std::vector<std::string>) override;
    std::string GetStatus() override; 
    std::vector<std::string> Tick(std::vector<std::string>) override;
    
    void Add(ModelInterface);
    void Bind_II(int);
    void Bind_OI(int,int);
    void Bind_OO(int);
    void Couplings(std::vector<std::string>);
    void FlushInputs();
private:
    std::vector<ModelInterface>* atomics;
    std::vector<std::vector<std::string>>* inputs;
    std::vector<std::vector<std::string>>* outputs;
    std::vector<Binding>* bindings;
    
    std::vector<std::string> ConcatinateString(
        std::vector<std::string>,std::vector<std::string>);
    
};

#endif /* NETWORK_H */

