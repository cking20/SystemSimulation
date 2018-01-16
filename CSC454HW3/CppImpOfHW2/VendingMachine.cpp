/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VendingMachine.cpp
 * Author: chrisrk192
 * 
 * Created on September 29, 2017, 2:22 PM
 */

#include "VendingMachine.h"
#include <string>
#include <vector>
#include <iostream>
VendingMachine::VendingMachine() {
    
    initSig = "\"number of quarters(0-100)\", \"number of dimes(0-100)\", \"number of nickels(0-100)\", \"initial value in machine(0-10000)\", \"cancel pushed(t/f)\"";

    numQuarters = 0; numNickels = 0; numDimes = 0; totalVal = 0;
    cancelPushed = false;
}

VendingMachine::VendingMachine(const VendingMachine& orig) {
}

VendingMachine::~VendingMachine() {
}


std::string VendingMachine::GetInitilizationSignature(){
    return initSig;
}

void VendingMachine::Initialize(std::vector<std::string> inputs){
    
    if(inputs.size() < 5)
        throw CantPerformAction(std::string("Not Enough Input to Init"));
    numQuarters = std::stoi(inputs.at(0));
    numDimes = std::stoi(inputs.at(1));
    numNickels = std::stoi(inputs.at(2));
    totalVal = std::stoi(inputs.at(3));
    if (inputs.at(4).at(0) == 't') {
        cancelPushed = true;
    } else if (inputs.at(4).at(0) == 'f'){
        cancelPushed = false;
    } else
        throw CantPerformAction(std::string("Bad input into Init"));
   
    
    
}

std::string VendingMachine::OutPut(){
    std::string output = "";
        if(cancelPushed){            
                std::vector<int> *l = CalcChange(totalVal, numQuarters, numDimes, numNickels);
                for(int i = 0; i < l->size();i++){
                    if(i == l->size()-1){
                        output+= EnumToString(l->at(i));
                    } else{
                        output+= EnumToString(l->at(i))+", ";
                    }
                }
                delete l;
        }else if(totalVal >= 100) {
            for(int j = 0; j < totalVal / 100; j++){
                if(j == (totalVal / 100)-1){
                    output+= EnumToString(coffee);
                } else{
                    output+= EnumToString(coffee)+", ";
                }
            }
                
        }
        if(output.compare("") ==0){
            output+=EnumToString(nothing);
        }
        return output;       
    
}  

void VendingMachine::StateTransition(std::vector<std::string> inputItems){
    if(cancelPushed){
        try{
            std::vector<int> *l = CalcChange(totalVal, numQuarters, numDimes, numNickels); 
            
            for(int i = 0; i < l->size();i++){
                switch(l->at(i)){
                    case quarter:
                        numQuarters--;
                        totalVal-=25;
                        break;
                    case nickel:
                        numNickels--;
                        totalVal-=5;
                        break;
                    case dime:
                        numDimes--;
                        totalVal-=10;
                        break;                        
                }
            }
            delete l;            
            if(totalVal > 0){
                throw CantPerformAction(std::string("Not enough Change."));//"Cant make all change.");
            }
        }catch(CantPerformAction e){
           cancelPushed = false;
           throw CantPerformAction(e.getMessage());
        }
        cancelPushed = false;
    }else if(totalVal >= 100) {
        int initVal = totalVal / 100;
        //for(int j = 0; j < initVal; j++){               
        //    totalVal -= 100;
        //}
        totalVal = totalVal % 100;
        }
        for(int n = 0; n < inputItems.size(); n++){
            ioSet s;
            //try{
                s = static_cast<ioSet>(StringToEnum(inputItems[n]));
            //} catch(IllegalArgumentException e){
            //    s = inputSet.wait;
            //}
            switch(s){
                case quarter:
                    numQuarters++;
                    totalVal+=25;
                    break;
                case nickel:
                    numNickels++;
                    totalVal+=5;
                    break;
                case dime:
                    numDimes++;
                    totalVal+=10;
                    break; 
                case cancel:
                    cancelPushed = true;
                    break;
                case wait:
                    break;
            }
        }
}

std::string VendingMachine::GetStatus(){
    std::string s =
        "number of quarters = " +std::to_string(numQuarters)+"\n"
                + "number of dimes    = " +std::to_string(numDimes)+"\n"
                + "number of nickels  = " +std::to_string(numNickels)+"\n"
                + "value in machine   = " +std::to_string(totalVal)+"\n"
                + "cancel pushed      = " +std::to_string(cancelPushed)+"\n";
    return s;
}  


/**
 * 
 * @param value
 * @param numQ
 * @param numD
 * @param numN
 * @return pointer to the vector NEEDS TO BE DELETED AFTER USE
 */
std::vector<int>* VendingMachine::CalcChange(int value, int numQ, int numD, int numN){
    std::vector<int> *l = new std::vector<int>;
        bool changeMadeFlag = false;
        while(value > 0){
            if(value >= 25 && numQ > 0){
                l->push_back(quarter);
                value -= 25;
                numQ--;
                changeMadeFlag = true;
            }
            if(value >= 10 && numD > 0){
                l->push_back(dime);
                value -= 10;
                numD--;
                changeMadeFlag = true;
            }
            if(value >= 5 && numN > 0){
                l->push_back(nickel);
                value -= 5;
                numN--;
                changeMadeFlag = true;
            }   
            if(numD <= 0 && numQ <= 0 && numN <= 0 || changeMadeFlag == false){
                value = 0;
                
            }
            changeMadeFlag = false;
                
        }
        return l;
}

int VendingMachine::StringToEnum(std::string in){
    if(in.compare("coffee") == 0)
        return coffee;
    if(in.compare("cancel") == 0)
        return cancel;
    if(in.compare("dime") == 0)
        return dime;
    if(in.compare("nickel") == 0)
        return nickel;
    if(in.compare("quarter") == 0)
        return quarter;
    if(in.compare("wait") == 0)
        return wait;
    if(in.compare("nothing") == 0)
        return nothing;
    return nothing;
    
}



std::string VendingMachine::EnumToString(int enm){
    switch(enm){
        case coffee: return "coffee";
        case cancel: return "cancel";
        case dime: return "dime";
        case nickel: return "nickel";
        case quarter: return "quarter";
        case wait: return "wait";
        default:
        case nothing: return "nothing";
    }
}
