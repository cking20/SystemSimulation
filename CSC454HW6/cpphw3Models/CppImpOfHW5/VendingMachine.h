/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VendingMachine.h
 * Author: chrisrk192
 *
 * Created on September 29, 2017, 2:22 PM
 */
#include <string>
#include <vector>
#include "ModelInterface.h"
#ifndef VENDINGMACHINE_H
#define VENDINGMACHINE_H

class VendingMachine: public ModelInterface {
public:
    std::string initSig;
    
    enum ioSet{nickel, dime, quarter, cancel, wait, coffee, nothing};
      
    VendingMachine();
    VendingMachine(const VendingMachine& orig);
    virtual ~VendingMachine();
    
    std::string GetInitilizationSignature() override;
    void Initialize(std::vector<std::string>) override;
    std::vector<std::string> OutPut()override;  
    void StateTransition(std::vector<std::string>) override;
    std::string GetStatus() override;   
    std::vector<std::string> Tick(std::vector<std::string>) override;
    
    
private:
    int numQuarters, numNickels, numDimes, totalVal;
    bool cancelPushed;
    std::vector<int>* CalcChange(int, int , int , int );
    int StringToEnum(std::string);
    std::string EnumToString(int);
    std::vector<std::string> SplitString(std::string,std::string);
    
};

#endif /* VENDINGMACHINE_H */

