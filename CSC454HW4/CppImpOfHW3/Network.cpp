/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Network.cpp
 * Author: chrisrk192
 * 
 * Created on October 18, 2017, 10:35 PM
 */

#include "Network.h"
#include "Binding.h"
#include "ModelInterface.h"
#include <vector>

Network::Network() {
    numInternalTicksToExternal = 1;
    atomics = new std::vector<ModelInterface>;  
    inputs = new std::vector<std::vector<std::string>>;  
    outputs = new std::vector<std::vector<std::string>>;  
    bindings = new std::vector<Binding>;  
}

Network::Network(const Network& orig) {
}

Network::~Network() {
    delete atomics;
    delete inputs;
    delete outputs;
    delete bindings;
}

std::string Network::GetInitilizationSignature(){
    return "";
}
void Network::Initialize(std::vector<std::string> args){
    std::string::size_type sz;   // alias of size_t
    numInternalTicksToExternal = std::stoi (args[0],&sz);
}

std::vector<std::string> Network::Tick(std::vector<std::string> args){
    std::vector<std::string> out;
    for(int i = 0; i < numInternalTicksToExternal; i++){
        out = OutPut();
        StateTransition(args);
    }
    return out;
}

std::vector<std::string> Network::OutPut(){
    for(int i = 0; i < atomics->size(); i++){
        std::vector<std::string> temp = atomics->at(i).OutPut();
        outputs->at(i) = temp;
        
    }
    // output from this network here not in Couplings
    std::vector<std::string> trueOutput = {};
    for (int i = 0; i < bindings->size();i++) {
        Binding temp = bindings->at(i);
        if(temp.to == -1){
            if(temp.from == -1)
                ;
            else 
                trueOutput = ConcatinateString(trueOutput,outputs->at(temp.from));
        }
    }
    return trueOutput;
}
void Network::StateTransition(std::vector<std::string> inputItems) {
    FlushInputs();
    Couplings(inputItems);
    for(int i = 0; i < atomics->size(); i++){     
        atomics->at(i).StateTransition(inputs->at(i));
        
    }
}
std::string Network::GetStatus() {
    std::string output = "Begin Status of Network\n";
        output += bindings->size() + " Bindings\n";
        output += numInternalTicksToExternal + " Internal Ticks to External\n";
        for(int j = 0; j < bindings->size(); j++){
            output += "     Binding of " + std::to_string(bindings->at(j).from) + " to " + std::to_string(bindings->at(j).to)+ "\n";
        }
        output += atomics->size() + " SubParts\n";
        for(int i = 0; i < atomics->size(); i++){
            output += "     " + atomics->at(i).GetStatus()+"\n";
        }
        output += "End Status of Network\n";
        
        return output;
}
void Network::Add(ModelInterface m){
    atomics->push_back(m);
}
void Network::Bind_II(int sink){
    bindings->push_back(Binding(-1,sink));
}
void Network::Bind_OI(int source,int sink){
    bindings->push_back(Binding(source,sink));
}
void Network::Bind_OO(int source){
    bindings->push_back(Binding(source,-1));
}
void Network::Couplings(std::vector<std::string> inputToNetwork){
    Binding temp;
    std::vector<std::string> cur;
    std::vector<std::string> trueOutput;// = new String[0];
    for (int i = 0; i < bindings->size();i++) {
        temp = bindings->at(i);
        if(temp.to == -1){
            //System.out.println("temp to == -1 skippin as should be done in output");
            //taken care of in output
            //trueOutput = ConcatinateString(trueOutput,outputs.get(temp.from));
        }
        else if(temp.from == -1){
            cur = inputs->at(temp.to);
            inputs->at(temp.to) = ConcatinateString(cur,inputToNetwork);
        }
        else{
            //System.out.println("coupling "+temp.from +" to "+ temp.to);
            cur = inputs->at(temp.to);
            inputs->at(temp.to) = ConcatinateString(cur,outputs->at(temp.from));
        }
    }
}
void Network::FlushInputs(){
    std::vector<std::string> nul;
    for (int i = 0; i < inputs->size(); i++) {
        inputs->at(i) = nul;
    }
}
std::vector<std::string> Network::ConcatinateString(std::vector<std::string> a,std::vector<std::string> b){
    std::vector<std::string> c;
    c.insert(c.end(),a.begin(),a.end());
    c.insert(c.end(),b.begin(),b.end());
    return c;
}

