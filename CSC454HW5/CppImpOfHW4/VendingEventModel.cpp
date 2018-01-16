/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VendingEventModel.cpp
 * Author: chrisrk192
 * 
 * Created on November 7, 2017, 7:22 PM
 */

#include "VendingEventModel.h"
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
VendingEventModel::VendingEventModel() {
    numQuarters = 0; numNickels = 0; numDimes = 0; totalVal = 0;
    e = 0.0;
}

VendingEventModel::VendingEventModel(const VendingEventModel& orig) {
}

VendingEventModel::~VendingEventModel() {
}

std::vector<std::string> VendingEventModel::OutPut(){
    std::string output = "";
        //if(cancelPushed){            
        if(totalVal >= 100) {
            for(int j = 0; j < totalVal / 100; j++){
                if(j == (totalVal / 100)-1){
                    output+= EnumToString(coffee)+", ";
                } else{
                    output+= EnumToString(coffee)+", ";
                }
            }
                
        }
        std::vector<int> *l = CalcChange(totalVal%100, numQuarters, numDimes, numNickels);
        for(int i = 0; i < l->size();i++){
            if(i == l->size()-1){
                output+= EnumToString(l->at(i));
            } else{
                output+= EnumToString(l->at(i))+", ";
            }
        }
        delete l;
        std::vector<std::string> so = SplitString(output,", ");
        return so; 
}   

void VendingEventModel::StateTransitionExt(std::vector<std::string> inputItems, double ee){
    e = ee;
    ioSet s;

    s = static_cast<ioSet>(StringToEnum(inputItems[n]));

    switch(s){
        case q:
            numQuarters++;
            totalVal+=25;
            break;
        case n:
            numNickels++;
            totalVal+=5;
            break;
        case d:
            numDimes++;
            totalVal+=10;
            break; 

    }
    e = 0.0;
}

void VendingEventModel::StateTransitionInt(){
    std::vector<int> *l = CalcChange(totalVal, numQuarters, numDimes, numNickels); 
            
            for(int i = 0; i < l->size();i++){
                switch(l->at(i)){
                    case q:
                        numQuarters--;
                        totalVal-=25;
                        break;
                    case n:
                        numNickels--;
                        totalVal-=5;
                        break;
                    case d:
                        numDimes--;
                        totalVal-=10;
                        break;                        
                }
            }
            delete l;            
            if(totalVal > 0){
                CantPerformEventAction(std::string("Not enough Change."));//"Cant make all change.");
            }
}

void VendingEventModel::StateTransitionCon(std::vector<std::string> inputItems){
    StateTransitionInt();
    StateTransitionExt(inputItems,2.0);
}

double VendingEventModel::TimeAdvance(){
    if(totalVal > 0)
        return 2.0;
    return INFINITY;
}


std::string VendingEventModel::GetStatus(){
    std::string s =
        "number of quarters = " +std::to_string(numQuarters)+"\n"
                + "number of dimes    = " +std::to_string(numDimes)+"\n"
                + "number of nickels  = " +std::to_string(numNickels)+"\n"
                + "value in machine   = " +std::to_string(totalVal)+"\n"
                ;
    return s;
}

std::vector<int>* VendingEventModel::CalcChange(int value, int numQ, int numD, int numN){
    std::vector<int> *l = new std::vector<int>;
        bool changeMadeFlag = false;
        while(value > 0){
            if(value >= 25 && numQ > 0){
                l->push_back(q);
                value -= 25;
                numQ--;
                changeMadeFlag = true;
            }
            if(value >= 10 && numD > 0){
                l->push_back(d);
                value -= 10;
                numD--;
                changeMadeFlag = true;
            }
            if(value >= 5 && numN > 0){
                l->push_back(n);
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

int VendingEventModel::StringToEnum(std::string in){
    if(in.compare("coffee") == 0)
        return coffee;
    
    if(in.compare("d") == 0)
        return d;
    if(in.compare("n") == 0)
        return n;
    if(in.compare("q") == 0)
        return q;
    return -1;
    
}

std::string VendingEventModel::EnumToString(int enm){
    switch(enm){
        case coffee: return "coffee";
        
        case d: return "d";
        case n: return "n";
        case q: return "q";
        
    }
}

std::vector<std::string> VendingEventModel::SplitString(std::string line, std::string delim){
    size_t pos = 0;
    std::vector<std::string> inputs;
    while ((pos = line.find(delim)) != std::string::npos) {
            inputs.push_back(line.substr(0, pos));
            line.erase(0, pos + delim.length());
        }
    inputs.push_back(line);
    return inputs;        
}
