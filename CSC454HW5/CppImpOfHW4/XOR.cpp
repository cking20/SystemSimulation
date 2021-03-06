/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   XORModel.cpp
 * Author: chrisrk192
 * 
 * Created on October 27, 2017, 10:21 PM
 */

#include "XOR.h"

XOR::XOR() {
}

XOR::XOR(const XOR& orig) {
}

XOR::~XOR() {
}

std::string XOR::GetInitilizationSignature(){
    return "initial state";
}
void XOR::Initialize(std::vector<std::string> inputs ){
    state = std::stoi(inputs.at(0));
}
std::vector<std::string> XOR::OutPut(){
    std::vector<std::string> o;
    o.push_back(std::to_string(state));
    return o;
}  
void XOR::StateTransition(std::vector<std::string> inputItems){
    int one, two;
        if(inputItems.size() < 2)
            throw new CantPerformAction("not enough inputs");
            one = stoi(inputItems.at(0));
            two = stoi(inputItems.at(1));
        state =  one ^ two;
}
std::string XOR::GetStatus(){
    return "state = "+std::to_string(state);
}
std::vector<std::string> XOR::Tick(std::vector<std::string> inputs){
    std::vector<std::string> out = OutPut();
    StateTransition(inputs);
    return out;
}