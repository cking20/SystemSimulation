/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mem.cpp
 * Author: chrisrk192
 * 
 * Created on October 27, 2017, 10:24 PM
 */

#include "Mem.h"

Mem::Mem() {
}

Mem::Mem(const Mem& orig) {
}

Mem::~Mem() {
}

std::string Mem::GetInitilizationSignature(){
    return "initialState1, initialState2";
}
void Mem::Initialize(std::vector<std::string>args){
    if(args.size() >= 2){
            qHead = std::stoi(args.at(0));
            qTail = std::stoi(args.at(1));
        }
        else
            qHead = 0; qTail = 0;
}
std::vector<std::string> Mem::OutPut(){
    std::vector<std::string> o;
    o.push_back(std::to_string(qHead));
    return o;
}  
void Mem::StateTransition(std::vector<std::string>args){
    qHead = qTail;
    qTail = std::stoi(args.at(0));
}
std::string Mem::GetStatus(){
    return "Queue: " + std::to_string(qHead) + ", " + std::to_string(qTail);
}
std::vector<std::string> Mem::Tick(std::vector<std::string> inputs){
    std::vector<std::string> out = OutPut();
    StateTransition(inputs);
    return out;
}