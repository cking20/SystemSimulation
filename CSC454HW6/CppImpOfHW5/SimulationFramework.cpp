/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationFramework.cpp
 * Author: chrisrk192
 * 
 * Created on October 7, 2017, 1:30 PM
 

//#include "SimulationFramework.h"
#include "ModelInterface.h"
#include "EventModelInterface.h"

#include <iostream>
#include <thread>
#include <chrono>

SimulationFramework::SimulationFramework() {
    producedOutput = false;
    tickTime = 5;
    consoleMode = false;
    debugMode = false;
    paused = true;
    tickOnEmptyInput = false;
    keepSimulating = true;
    inEventMode = true;
    currentTime = 0.0;
}

SimulationFramework::SimulationFramework(const SimulationFramework& orig) {
}

SimulationFramework::~SimulationFramework() {
}

SimulationFramework::SimulationFramework(bool isInEventMode, bool inDebugMode,bool tickOnEmpty, bool startPaused,long deltaTime){
        //instance = m;
        inEventMode = isInEventMode;
        keepSimulating = true;
        producedOutput = false;
        tickTime = deltaTime; 
        debugMode = inDebugMode;
        tickOnEmptyInput = tickOnEmpty;
        paused = startPaused;
        //theModels = new ArrayList<>();
        theCmds;
        theOutputs;
        currentTime = 0.0;
    }
void SimulationFramework::SetModel(ModelInterface*m){
    theModel = m;
}
void SimulationFramework::SetEventModel(EventModelInterface *m){
    theEventModel = m;
}

std::string SimulationFramework::GetStatus(){
    return theModel->GetStatus();
}

void SimulationFramework::AddInput(std::vector<std::string> s){
    cmdMutex.lock();
    theCmds.push_back(s);
    cmdMutex.unlock();
}

std::vector<std::vector<std::string>> SimulationFramework::FlushOutputs(){
    outputMutex.lock();
    std::vector<std::vector<std::string>> ret(theOutputs);
    theOutputs.clear();
    producedOutput = false;
    outputMutex.unlock();
    return ret;
}

std::vector<std::string> SimulationFramework::RemoveFirst( std::vector<std::string> input){
    std::vector<std::string> ret (input);
    ret.erase(ret.begin());
    return ret;
}

void SimulationFramework::Stop(){
    keepSimulating = false;
}

void SimulationFramework::Run(){
    try{
        if(inEventMode){
            while(keepSimulating){
                if (theCmds.size() > 0 && !paused) {
                    std::vector<std::string> cmd;
                    cmdMutex.lock();
                        cmd = theCmds.at(0);
                        theCmds.erase(theCmds.begin());              
                    cmdMutex.unlock();
                    
                    double nextCMDtime = std::stod(cmd.at(0));
                    std::vector<std::string> out;
                    double timeto = currentTime + theEventModel->TimeAdvance();
                    if(nextCMDtime < timeto){//external
                        double e = nextCMDtime - currentTime;
                        currentTime = nextCMDtime;                            
                        theEventModel->StateTransitionExt(RemoveFirst(cmd), e);

                    }else if(nextCMDtime > timeto){//internal

                        theCmds.insert(theCmds.begin(),cmd);//   .add(0, cmd);

                        currentTime = timeto;
                        out = theEventModel->OutPut();
                        theEventModel->StateTransitionInt();

                    } else {//confluent
                        currentTime = timeto;
                        out = theEventModel->OutPut();
                        theEventModel->StateTransitionCon(RemoveFirst(cmd));

                    }  
                        outputMutex.lock();
                        std::vector<std::string> timeStamp;
                        std::string ts = "Time "+std::to_string(currentTime)+": "; 
                        timeStamp.push_back(ts);
                        theOutputs.push_back(timeStamp);
                        theOutputs.push_back(out);
                        producedOutput = true;           
                        if(debugMode){
                            std::vector<std::string> debug;
                            debug.push_back(theEventModel->GetStatus());
                                theOutputs.push_back(debug);//System.out.println(theModel->GetStatus());
                        }
                        outputMutex.unlock();
                    
                }else if(theCmds.size()==0 && !paused){//final internal trans
                    std::vector<std::string> out;
                    keepSimulating = false;
                    currentTime = currentTime + theEventModel->TimeAdvance();
                    out = theEventModel->OutPut();
                    theEventModel->StateTransitionInt();
                    
                    outputMutex.lock();
                    std::vector<std::string> timeStamp;
                    std::string ts = "Time "+std::to_string(currentTime)+": "; 
                    timeStamp.push_back(ts);
                    theOutputs.push_back(timeStamp);
                    theOutputs.push_back(out);
                    producedOutput = true;           
                    if(debugMode){
                        std::vector<std::string> debug;
                        debug.push_back(theEventModel->GetStatus());
                            theOutputs.push_back(debug);//System.out.println(theModel->GetStatus());
                    }
                    outputMutex.unlock();
                }
            }
        }else{
            while (keepSimulating){//!Thread.interrupted())
            //System.out.println("simulating thread");
        
            if (theCmds.size() > 0 && !paused) {
                std::vector<std::string> cmd;
                cmdMutex.lock();
                    cmd = theCmds.at(0);
                    theCmds.erase(theCmds.begin());              
                cmdMutex.unlock();
                std::vector<std::string> out;
                out = theModel->Tick(cmd);          
                outputMutex.lock();
                    theOutputs.push_back(out);
                    producedOutput = true;           
                    if(debugMode){
                        std::vector<std::string> debug;
                        debug.push_back(theModel->GetStatus());
                            theOutputs.push_back(debug);//System.out.println(theModel->GetStatus());
                    }
                outputMutex.unlock();
            }else if(tickOnEmptyInput && !paused){//dont tick if there isnt any input
                std::vector<std::string> temp;
                std::vector<std::string> out(theModel->Tick(temp));
                outputMutex.lock();
                    theOutputs.push_back(out);
                    producedOutput = true;
                    if(debugMode){
                        std::vector<std::string> debug;
                        debug.push_back(theModel->GetStatus());
                        theOutputs.push_back(debug);//System.out.println(theModel->GetStatus());
                    }
                outputMutex.unlock();
            }
        }
           
        
        //std::this_thread::sleep_for(std::chrono::seconds(tickTime));
    }
    } catch(CantPerformAction c){
        std::cout << c.getMessage() << std::endl;
        keepSimulating = false;
    }
}

std::vector<std::string> Tick(ModelInterface m, std::vector<std::string> input){
    std::vector<std::string> ret;
    try{
        ret = m.Tick(input);
    } catch(CantPerformAction c){
        //std::cout << c.getMessage() << std::endl;
    }
    return ret;
}

*/