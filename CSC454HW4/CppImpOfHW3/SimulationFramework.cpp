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
 */

#include "SimulationFramework.h"
#include "ModelInterface.h"

#include <thread>
#include <chrono>

SimulationFramework::SimulationFramework() {
    producedOutput = false;
    tickTime = 5;
    consoleMode = false;
    debugMode = false;
    paused = false;
    tickOnEmptyInput = true;
    keepSimulating = true;
}

SimulationFramework::SimulationFramework(const SimulationFramework& orig) {
}

SimulationFramework::~SimulationFramework() {
}

SimulationFramework::SimulationFramework(bool inDebugMode,bool tickOnEmpty, bool startPaused,long deltaTime){
        //instance = m;      
        tickTime = deltaTime; 
        debugMode = inDebugMode;
        tickOnEmptyInput = tickOnEmpty;
        paused = startPaused;
        //theModels = new ArrayList<>();
        theCmds;
        theOutputs;
    }
void SimulationFramework::SetModel(ModelInterface m){
    theModel = m;
}

std::string SimulationFramework::GetStatus(){
    return theModel.GetStatus();
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

void SimulationFramework::Stop(){
    keepSimulating = false;
}

void SimulationFramework::Run(){
    while (keepSimulating){//!Thread.interrupted())
        //System.out.println("simulating thread");
        if (theCmds.size() > 0 && !paused) {
            std::vector<std::string> cmd;
            cmdMutex.lock();
                cmd = theCmds.at(0);
                theCmds.erase(theCmds.begin());
                
            cmdMutex.unlock();
            std::vector<std::string> out(theModel.Tick(cmd));//Tick(theModel, cmd));
            outputMutex.lock();
                theOutputs.push_back(out);
                producedOutput = true;
            outputMutex.unlock();
            if(debugMode){
                std::vector<std::string> debug;
                debug.push_back(theModel.GetStatus());
                    theOutputs.push_back(debug);//System.out.println(theModel.GetStatus());
            }
        }else if(tickOnEmptyInput && !paused){//dont tick if there isnt any input
            std::vector<std::string> temp;
            std::vector<std::string> out(theModel.Tick(temp));
            outputMutex.lock();
                theOutputs.push_back(out);
                producedOutput = true;
            outputMutex.unlock();
            if(debugMode){
                std::vector<std::string> debug;
                debug.push_back(theModel.GetStatus());
                theOutputs.push_back(debug);//System.out.println(theModel.GetStatus());
            }
        }   
        
        std::this_thread::sleep_for(std::chrono::seconds(tickTime));
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

