/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EventNetwork.h
 * Author: chrisrk192
 *
 * Created on November 16, 2017, 11:41 PM
 */

#ifndef EVENTNETWORK_H
#define EVENTNETWORK_H

#include <string>
#include <iostream>
#include <vector>
#include "EventModelInterface.h"
#include "Event.h"
#include "Binding.h"
template<class I, class O>
class EventNetwork{
public:
    std::vector<EventModelInterface<I,O>*> atomics;
    std::vector<Binding*> bindings;
    
    EventNetwork() {
        atomics;// = new std::vector<>();  
        
        bindings;// = new std::vector<>();
    }
    ~EventNetwork(){
        for(int i = 0; i < atomics.size(); i++){
            delete atomics.at(i);
        }
        for(int i = 0; i < bindings.size(); i++){
            delete bindings.at(i);
        }
    }
    
    void Add(EventModelInterface<I,O>* model){
        atomics.push_back(model);
    }
       //binds input to network as input to this network
    void Bind_II(EventModelInterface<I,O>* e){
        
        int sink = IndexOfAtomic(e);
        Binding* b = new Binding(-1, sink);
        bindings.push_back(b);
    }
    //binds output from srouce as input to sink
    void Bind_OI(EventModelInterface<I,O>* e,EventModelInterface<I,O>* k){
        int source = IndexOfAtomic(e);
        int sink = IndexOfAtomic(k);
        Binding* b = new Binding(source, sink);
        bindings.push_back(b);
    }
    //binds output from source to the output of this network
    void Bind_OO(EventModelInterface<I,O>* k){
        int source = IndexOfAtomic(k);
        Binding* b = new Binding(source, -1);
        bindings.push_back(b);
    }

    std::vector<EventModelInterface<I,O>*> getExternInputModels(){
        std::vector<EventModelInterface<I,O>*> a;// = new std::vector<>();
        for(int i = 0; i < bindings.size(); i++){
            if(bindings.at(i)->from == -1)
                a.push_back(atomics.at(bindings.at(i)->to));
        }
        return a;
    }
    
    std::vector<EventModelInterface<I,O>*> getExternOutputModels(){
        std::vector<EventModelInterface<I,O>*> a;// = new std::vector<>();
        for(int i = 0; i < bindings.size(); i++){
            if(bindings.at(i)->to == -1)
                a.push_back(atomics.at(bindings.at(i)->from));
        }
        return a;
    }
    bool isExternOutputModel(EventModelInterface<I,O>* e){
        //std::vector<EventModelInterface<I,O>> a = new std::vector<>();
        for(int i = 0; i < bindings.size(); i++){
            if(bindings.at(i)->to == -1)
                if(atomics.at(bindings.at(i)->from) == e)
                    return true;
                
        }
        return false;
    }
    std::vector<EventModelInterface<I,O>*> getModelsOutputBoundTo(EventModelInterface<I,O>* e){
        std::vector<EventModelInterface<I,O>*> a;// = new std::vector<>();
        int myIndex = IndexOfAtomic(e);
        for(int i = 0; i < bindings.size(); i++){
            if(bindings.at(i)->from == myIndex && bindings.at(i)->to != -1)
                a.push_back(atomics.at(bindings.at(i)->to));
        }
        return a;
    }
    
    
    
/*
    public void Couplings(String[] inputToNetwork){
        //then copy outputs to inputs based on mappings
        Binding temp;
        String[] cur;
        String[] trueOutput = new String[0];
        for (int i = 0; i < bindings.size();i++) {
            temp = bindings.at(bindings.begin()+i);
            if(temp->to == -1){
                if(temp->from == -1)
                    ;
                else 
                    trueOutput = ConcatinateString(trueOutput,outputs.get(temp->from));
            }
            else if(temp->from == -1){
                cur = inputs.get(temp->to);
                inputs.set(temp->to, ConcatinateString(cur,inputToNetwork));
            }
            else{
                //System.out.println("coupling "+temp->from +" to "+ temp->to);
                cur = inputs.get(temp->to);
                inputs.set(temp->to, ConcatinateString(cur,outputs.get(temp->from)));
            }
                
                
        }
        //return trueOutput;
    }
   
*/    
   
    std::string GetStatus() {
        std::string output = "Begin Status of Network\n";
        output += std::to_string(bindings.size()) + " Bindings\n";
        
        for(int j = 0; j < bindings.size(); j++){
            output += "     Binding of " + std::to_string(bindings.at(j)->from) + " to " + std::to_string(bindings.at(j)->to)+ '\n';
        }
        output += atomics.size() + " SubParts\n";
        for(int i = 0; i < atomics.size(); i++){
            output += "     " + (atomics.at(i)->GetStatus())+"\n";
        }
        output += "End Status of Network\n";
        return output;
    }
private:
    std::ptrdiff_t IndexOfAtomic(EventModelInterface<I,O>* e){
        int i;
        for(i = 0; i < atomics.size(); i++){
            if(atomics.at(i) == e)
                return i;
        }
        if(i >= atomics.size()) {
            std::cout << "Error: Attempting to access Atomic model not added to Network";
            
        }
        return i;
    }
};


#endif /* EVENTNETWORK_H */

