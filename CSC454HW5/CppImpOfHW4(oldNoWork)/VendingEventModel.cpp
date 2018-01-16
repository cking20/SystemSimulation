/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VendingEventModel.cpp
 * Author: chrisrk192
 * 
 * Created on October 30, 2017, 9:47 PM
 */
#include <string>
#include <vector>
#include "VendingEventModel.h"
#include "VendingMachine.h"

VendingEventModel::VendingEventModel() {
}

VendingEventModel::VendingEventModel(const VendingEventModel& orig) {
}

VendingEventModel::~VendingEventModel() {
    qNum = 0;
    nNum = 0; 
    dNum = 0;
    vNum = 0; 
    e = 0.0;
}

VendingEventModel::VendingEventModel(int nq, int nd, int nn, int nv){
        qNum = nq;
        dNum = nd;
        nNum = nn;
        vNum = nv;
        e = 0.0;
    }

std::vector<std::string> VendingEventModel::OutPut(){
    std::string output = "";
    for(int j = 0; j < vNum / 100; j++){
        if(j == (vNum / 100)-1){
            output+= EnumToString(coffee);
        } else{
            output+= EnumToString(coffee)+", ";
        }
    }                  
    std::vector<int> *l = CalcChange(vNum, qNum, dNum, nNum);
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


void VendingEventModel::StateTransitionExt(std::vector<std::string> inputItems, double time){
    e = time;
        ioSet s;        
        s = StringToEnum(inputItems[0]);        
        switch(s){
            case q:
                qNum++;
                vNum+=25;
                break;
            case n:
                nNum++;
                vNum+=5;
                break;
            case d:
                dNum++;
                vNum+=10;
                break; 
        }
        e = 0.0;

}

void VendingEventModel::StateTransitionInt(){
    e = TimeAdvance();    
    vNum = vNum % 100;//give out coffee
    std::vector<int> *l = CalcChange(vNum, qNum, dNum, nNum);
    //give out chnage
    for(int i = 0; i < l->size();i++){
       switch(l->at(i)){
           case q:
               qNum--;
               break;
           case d:
               dNum--;
               break;
           case n:
               nNum--;
               break;  
       } 
    }
    vNum = 0; 
    delete l;             
    e = 0.0;
}
void VendingEventModel::StateTransitionCon(std::vector<std::string> inputItems){
    StateTransitionInt();
    StateTransitionExt(inputItems,2.0);
}
double VendingEventModel::TimeAdvance(){
    if(vNum > 0)
            return 2.0;
        return std::numeric_limits<double>::infinity();
}

std::vector<std::string> VendingEventModel::GetStatus(){
    std::vector<std::string> s;
        s.push_back("q = " +qNum+"\n"
                + "d = " +dNum+"\n"
                + "n = " +nNum+"\n"
                + "v = " +vNum+"\n"
                + "e = " +e+"\n");
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
        default:
            return "badEnumToString";
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
