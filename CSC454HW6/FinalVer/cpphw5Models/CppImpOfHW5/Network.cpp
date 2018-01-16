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
#include <iostream>

Network::Network() {
    numInternalTicksToExternal = 1;
    atomics;// = new std::vector<ModelInterface*>;  
    inputs;// = new std::vector<std::vector<std::string>>;  
    outputs;// = new std::vector<std::vector<std::string>>;  
    bindings;// = new std::vector<Binding>();  
}

Network::Network(const Network& orig) {
}

Network::~Network() {
    //delete atomics;
    //delete inputs;
    //delete outputs;
    //delete bindings;
    for(int i = 0; i < bindings.size(); i++)
        delete bindings.at(i);
    for(int i = 0; i < atomics.size(); i++)
        delete atomics.at(i);
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
    for(int i = 0; i < atomics.size(); i++){
        std::vector<std::string> temp(atomics.at(i)->OutPut());
        outputs.at(i).swap(temp);
        
    }
    // output from this network here not in Couplings
    std::vector<std::string> trueOutput = {};
    for (int i = 0; i < bindings.size();i++) {
        Binding *temp = bindings.at(i);
        //std::cout <<"binding from "<<temp->from<<" to "<<temp->to<<std::endl;
        if(temp->to == -1){
            if(temp->from == -1)
                ;
            else 
                trueOutput = ConcatinateString(trueOutput,outputs.at(temp->from));
        }
    }
    return trueOutput;
}
void Network::StateTransition(std::vector<std::string> inputItems) {
    FlushInputs();
    Couplings(inputItems);
    for(int i = 0; i < atomics.size(); i++){     
        atomics.at(i)->StateTransition(inputs.at(i));
        
    }
}
std::string Network::GetStatus() {
    std::string output = "Begin Status of Network\n";
        output += std::to_string(bindings.size()) + " Bindings\n";
        output += std::to_string(numInternalTicksToExternal) + " Internal Ticks to External\n";
        //std::cout << "binding@i=" << bindings.at(0)->from << " to " << bindings.at(0)->to << std::endl;
        for(int j = 0; j < bindings.size(); j++){
            output += "     Binding of " + std::to_string(bindings.at(j)->from) + " to " + std::to_string(bindings.at(j)->to)+ "\n";
        }
        output += atomics.size() + " SubParts\n";
        for(int i = 0; i < atomics.size(); i++){
            output += "//////////////\n" 
                    + atomics.at(i)->GetStatus()+"\n"
                    + "//////////////\n";
        }
        output += "End Status of Network\n";
        
        return output;
}
void Network::Add(ModelInterface*m){
    atomics.push_back(m);
    std::vector<std::string> i;
    inputs.push_back(i);
    std::vector<std::string> o;
    outputs.push_back(o);
}
void Network::Bind_II(int sink){
    Binding *b = new Binding;
    b->from = -1;
    b->to = sink;
    bindings.insert(bindings.begin(),b);
}
void Network::Bind_OI(int source,int sink){
    Binding *b = new Binding;
    b->from = source;
    b->to = sink;
    bindings.insert(bindings.begin(),b);
}
void Network::Bind_OO(int source){
    Binding *b = new Binding;
    b->from = source;
    b->to = -1;
    bindings.insert(bindings.begin(),b);
}
void Network::Couplings(std::vector<std::string> inputToNetwork){
    
    std::vector<std::string> cur;
    std::vector<std::string> trueOutput;// = new String[0];
    for (int i = 0; i < bindings.size();i++) {
        
        if(bindings.at(i)->to == -1){
            //System.out.println("bindings.at(i) to == -1 skippin as should be done in output");
            //taken care of in output
            //trueOutput = ConcatinateString(trueOutput,outputs.get(bindings.at(i).from));
        }
        else if(bindings.at(i)->from == -1){
            cur = inputs.at(bindings.at(i)->to);
            inputs.at(bindings.at(i)->to) = ConcatinateString(cur,inputToNetwork);
        }
        else{
            //System.out.println("coupling "+bindings.at(i).from +" to "+ bindings.at(i).to);
            cur = inputs.at(bindings.at(i)->to);
            inputs.at(bindings.at(i)->to) = ConcatinateString(cur,outputs.at(bindings.at(i)->from));
        }
    }
}
void Network::FlushInputs(){
    std::vector<std::string> nul;
    for (int i = 0; i < inputs.size(); i++) {
        inputs.at(i) = nul;
    }
}
std::vector<std::string> Network::ConcatinateString(std::vector<std::string> a,std::vector<std::string> b){
    std::vector<std::string> c;
    c.insert(c.end(),a.begin(),a.end());
    c.insert(c.end(),b.begin(),b.end());
    return c;
}

