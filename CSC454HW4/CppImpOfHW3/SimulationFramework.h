/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulationFramework.h
 * Author: chrisrk192
 *
 * Created on October 7, 2017, 1:30 PM
 */

#ifndef SIMULATIONFRAMEWORK_H
#define SIMULATIONFRAMEWORK_H

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "ModelInterface.h"
class SimulationFramework {
public:
    static std::string DELIM;
    bool producedOutput;
    int tickTime;
    
    SimulationFramework();
    SimulationFramework(const SimulationFramework& orig);
    virtual ~SimulationFramework();
    SimulationFramework(bool ,bool , bool ,long);
    void SetModel(ModelInterface);
    std::string GetStatus();
    void AddInput(std::vector<std::string>);
    std::vector<std::vector<std::string>> FlushOutputs();
    void Stop();
    void Run();
    
private:
    bool consoleMode;
    bool debugMode;
    bool paused;
    bool tickOnEmptyInput;
    ModelInterface theModel;
    std::vector<std::vector<std::string>> theCmds; 
    std::mutex cmdMutex;
    std::vector<std::vector<std::string>> theOutputs;
    std::mutex outputMutex;
    //private int numModels;
    std::atomic< bool > keepSimulating;
    
    std::vector<std::string> Tick(ModelInterface, std::vector<std::string>);
};

#endif /* SIMULATIONFRAMEWORK_H */

