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
#include <limits>
#include "ModelInterface.h"
#include "EventModelInterface.h"
#include "OutputTimePairs.h"
#include "EventScheduler.h"
#include "EventNetwork.h"
#include "TwoDTime.h"
template<class I, class O>
class SimulationFramework {
public:
    static std::string DELIM;
    bool producedOutput;
    int tickTime;
    bool debugMode;
    bool paused;
    bool tickOnEmptyInput;
    bool inEventMode;
    SimulationFramework();
    SimulationFramework(const SimulationFramework& orig);
    virtual ~SimulationFramework();
    SimulationFramework(bool,bool ,bool , bool ,long);
    void SetModel(ModelInterface*);
    void SetEventModel(EventModelInterface<I,O>*);
    void SetEventNetwork(EventNetwork<I,O>*);
    std::string GetStatus();
    void AddInput(I);
    void AddInput(double, int, I);
    //std::vector<std::vector<std::string>> FlushOutputs();
    void Stop();
    void Run();
    std::vector<OutputTimePairs<O>*> RunEventSimulation();
    
private:
    double currentTime;
    bool consoleMode;
    ModelInterface* theModel;
    EventModelInterface<I,O>* theEventModel;
    EventNetwork<I,O>* theEventNetwork;
    std::vector<I> theCmds; 
    std::vector<TwoDTime*> theCmdsTime; 
    std::mutex cmdMutex;
    std::vector<O> theOutputs;
    std::vector<TwoDTime*> theOutputsTime; 
    std::mutex outputMutex;
    //private int numModels;
    std::atomic< bool > keepSimulating;
    
    
    //std::vector<std::string> RemoveFirst( std::vector<std::string>);
    std::vector<std::string> Tick(ModelInterface, std::vector<std::string>);

};

#endif /* SIMULATIONFRAMEWORK_H */

template<class I, class O>
SimulationFramework<I,O>::SimulationFramework() {
    producedOutput = false;
    tickTime = 5;
    consoleMode = false;
    debugMode = false;
    paused = true;
    tickOnEmptyInput = false;
    keepSimulating = true;
    inEventMode = true;
    currentTime = 0.0;
    theCmds;
    theOutputs;
    theModel = NULL;
    theEventModel = NULL;
    theEventNetwork = NULL;
}
template<class I, class O>
SimulationFramework<I,O>::SimulationFramework(const SimulationFramework& orig) {
}
template<class I, class O>
SimulationFramework<I,O>::~SimulationFramework() {
    if(theModel != NULL){
        delete theModel;
    }
    if(theEventModel != NULL){
        delete theEventModel;
    }
    if(theEventNetwork != NULL){
        delete theEventNetwork;
    }
    for(int i = 0; i < theCmdsTime.size(); i++){
            delete theCmdsTime.at(i);
    }
    
}
template<class I, class O>
SimulationFramework<I,O>::SimulationFramework(bool isInEventMode, bool inDebugMode,bool tickOnEmpty, bool startPaused,long deltaTime){
        //instance = m;
        inEventMode = isInEventMode;
        keepSimulating = true;
        producedOutput = false;
        tickTime = deltaTime; 
        debugMode = inDebugMode;
        tickOnEmptyInput = tickOnEmpty;
        paused = startPaused;
        //theModels = new std::vector<>();
        theCmds;
        theOutputs;
        currentTime = 0.0;
        theModel = NULL;
        theEventModel = NULL;
        theEventNetwork = NULL;
    }
template<class I, class O>
void SimulationFramework<I,O>::SetModel(ModelInterface*m){
    theModel = m;
}
template<class I, class O>
void SimulationFramework<I,O>::SetEventModel(EventModelInterface<I,O> *m){
    theEventModel = m;
}
template<class I, class O>
void SimulationFramework<I,O>::SetEventNetwork(EventNetwork<I,O> *m){
    theEventNetwork = m;
}
template<class I, class O>
std::string SimulationFramework<I,O>::GetStatus(){
    return theModel->GetStatus();
}
template<class I, class O>
void SimulationFramework<I,O>::AddInput(I s){
    cmdMutex.lock();
    theCmds.push_back(s);
    cmdMutex.unlock();
}
template<class I, class O>
void SimulationFramework<I,O>::AddInput(double real, int disc, I s){
    cmdMutex.lock();
    theCmds.push_back(s);
    TwoDTime* t = new TwoDTime(real,disc);
    theCmdsTime.push_back(t);
    cmdMutex.unlock();
}
/*
template<class I, class O>
O SimulationFramework<I,O>::FlushOutputs(){
    outputMutex.lock();
    std::vector<O> ret(theOutputs);
    theOutputs.clear();
    producedOutput = false;
    outputMutex.unlock();
    return ret;
}
/*
template<class I, class O>
I SimulationFramework<I,O>::RemoveFirst( I input){
    std::vector<std::string> ret (input);
    ret.erase(ret.begin());
    return ret;
}*/
template<class I, class O>
void SimulationFramework<I,O>::Stop(){
    keepSimulating = false;
}

//assume input already added, model added
template<class I, class O>
std::vector<OutputTimePairs<O>*> SimulationFramework<I,O>::RunEventSimulation(){
    std::vector<OutputTimePairs<O>*> theoutput;// = new std::vector<>();
    EventScheduler<I,O> schedule;// = new EventScheduler<>();
    TwoDTime *current2Dtime = new TwoDTime(0.0,0);// = new TwoDTime(0, 0);
    
    //queue event for each input
    for (int i = 0; i < theCmds.size(); i++) {         
        //send input to each thing hooked up to I of network
        std::vector<EventModelInterface<I,O>*> ins = theEventNetwork->getExternInputModels();
        for (int j = 0; j < ins.size(); j++) {
            Event<I,O>* e = new Event<I,O>();
            
            e->input = theCmds.at(i);
            e->model = ins.at(j);
            //std::cout <<"theCmdsTime.at(i)=" <<  theCmdsTime.at(i)->getDiscreteTime() << std::endl;
            e->myTime = theCmdsTime.at(i);
            //System.out.println("queued input @ "+e->myTime->getRealTime());
            e->typeOfEvent = Event<I,O>::external;
            e->timeQueued = current2Dtime;
            schedule.Add(e);
        }

    }
    //for each model queue internal event
    //std::cout << "num atomics: " << theEventNetwork->atomics.size() << std::endl;
    
    for(int i = 0; i < theEventNetwork->atomics.size(); i++){
        Event<I,O>* e = new Event<I, O>();
        e->model = theEventNetwork->atomics.at(i);
        e->typeOfEvent = Event<I,O>::internal;
        TwoDTime *tempTime = new TwoDTime(e->model->TimeAdvance()+current2Dtime->getRealTime(), 0);
        //std::cout << "tempTime = " << tempTime->getRealTime() << ", " << tempTime->getDiscreteTime()<< std::endl;
        e->myTime = tempTime;
        e->timeQueued = current2Dtime;
        schedule.Add(e);
    }
    theCmds.clear();
    theCmdsTime.clear();
    theOutputs.clear();
    theOutputsTime.clear();
    //schedule.Print();
    //double e;
    //int testBreaker = 0;
    while(!schedule.isEmpty() && std::numeric_limits<double>::infinity() != schedule.Peek()->myTime->getRealTime()){
        //testBreaker++;
        //if(testBreaker > 10){
        //    std::cout << "broke the loop";
        //    return theoutput;
        //}
        //schedule.Print();
        //pop event
        O anOutput;// = NULL;
        Event<I,O>* even = schedule.Pop();
        std::cout << "\n-------------------------------Currently working with " << even->Print() << std::endl;
        std::cout << "- - - - - Schedule DEBUG - - - - -" << std::endl;
        schedule.Print();
        std::cout << "- - - - - END Schd DEBUG - - - - -" << std::endl;
        //switch type
        try{
            //e = even->myTime->getRealTime() - current2Dtime->getRealTime();
        
            //ext
            if(even->typeOfEvent == Event<I,O>::external){
                std::cout << "Current Event Debug" << std::endl;
                std::cout << "event type        : "<<even->typeOfEvent << std::endl;
                //std::cout << "event input       : "<< even->input << std::endl;
                std::cout << "event time        : "<< even->myTime->getRealTime() << ", " << even->myTime->getDiscreteTime() << std::endl;
                std::cout << "event model       : "<< even->model << std::endl;
                std::cout << "event model status: "<< even->model->GetStatus() << std::endl;
                
                
                current2Dtime = even->myTime;
                //System.out.println("ext evnt @ current Time: "+current2Dtime->getRealTime()+" e = "+(even->myTime->getRealTime()-even->timeQueued->getRealTime()));
                //remove internals for that model
                //get the time the old int event was queued then sub and thats e
                Event<I,O>* oldInternalEvent  = schedule.DeleteOldInternals(even->model);
                double ee = 0.0;
                if(oldInternalEvent != NULL){
                    ee = current2Dtime->getRealTime() - oldInternalEvent->timeQueued->getRealTime();
                }
                //run model ext
                even->model->StateTransitionExt(even->input,ee);
                std::cout << "EXT Debug: " << even->model << " " << even->model->GetStatus() << std::endl;
                //queue new internal event
                Event<I,O>* newEven = new Event<I,O>();
                newEven->model = even->model;
                TwoDTime* tempTime = new TwoDTime(even->model->TimeAdvance(), 0);
                newEven->myTime = current2Dtime->Advance(tempTime);
                delete tempTime;
                //System.out.println("queue newEven.time = "+newEven->myTime->getRealTime());
                newEven->typeOfEvent = Event<I,O>::internal;
                newEven->timeQueued = current2Dtime;
                schedule.Add(newEven);
        }
            //int
            if(even->typeOfEvent == Event<I,O>::internal){
                std::cout << "Current Event Debug" << std::endl;
                std::cout << "event type        : "<<even->typeOfEvent << std::endl;
                //std::cout << "event input       : "<< even->input << std::endl;
                std::cout << "event time        : "<< even->myTime->getRealTime() << ", " << even->myTime->getDiscreteTime() << std::endl;
                std::cout << "event model       : "<< even->model << std::endl;
                std::cout << "event model status: "<< even->model->GetStatus() << std::endl;
                
                current2Dtime = even->myTime;
                //System.out.println("int evnt @ current Time: "+current2Dtime->getRealTime());
                //remove internals for that model(dont think i need to)
                //schedule.DeleteOldInternals(even->model);
                //produce output
                                
                anOutput = even->model->OutPut();
                //send this output to the appropriate other models
                if(theEventNetwork->isExternOutputModel(even->model)){
                    //std::cout << "ADDING TRUE OUTPUT OF " << anOutput << std::endl;
                    theOutputs.push_back(anOutput);
                    theOutputsTime.push_back(current2Dtime);
                    OutputTimePairs<O>* tempOuts = new OutputTimePairs<O>(current2Dtime->getRealTime(),current2Dtime->getDiscreteTime(),anOutput);
                    //std::cout << "queueing ouput of: "<< tempOuts->getData() << std::endl;
                    theoutput.push_back(tempOuts);                        
                }// else {//not an external output, add ext events for others bound to
                    std::vector<EventModelInterface<I,O>*> boundTo = theEventNetwork->getModelsOutputBoundTo(even->model);
                    for (int i = 0; i < boundTo.size(); i++) {
                        Event<I,O>* newE = new Event<I,O>();
                        newE->model = boundTo.at(i);
                        TwoDTime* now = new TwoDTime(0, 0);
                        newE->myTime = current2Dtime->Advance(now);
                        delete now;
                        //System.out.println("queue newEven.time = "+newE->myTime->getRealTime());
                        newE->typeOfEvent = Event<I,O>::external;
                        newE->input = (I)anOutput;
                        newE->timeQueued = current2Dtime;
                        schedule.Add(newE);
                    }
                //}
                //run int
                even->model->StateTransitionInt();
                std::cout << "INT Debug: " << even->model << " " << even->model->GetStatus() << std::endl;
                //queue new int
                TwoDTime* tempTime2 = new TwoDTime(even->model->TimeAdvance(), 0);
                even->myTime = current2Dtime->Advance(tempTime2);
                even->timeQueued = current2Dtime;
                schedule.Add(even);
            }
            //con
            if(even->typeOfEvent == Event<I,O>::confluent){
                std::cout << "Current Event Debug" << std::endl;
                std::cout << "event type        : "<<even->typeOfEvent << std::endl;
                //std::cout << "event input       : "<< even->input << std::endl;
                std::cout << "event time        : "<< even->myTime->getRealTime() << ", " << even->myTime->getDiscreteTime() << std::endl;
                std::cout << "event model       : "<< even->model << std::endl;
                std::cout << "event model status: "<< even->model->GetStatus() << std::endl;
                
                
                current2Dtime = even->myTime;
                //System.out.println("con evnt @ current Time: "+current2Dtime->getRealTime());
                //remove internals for that model(dont think i need to) 
                //schedule.DeleteOldInternals(even->model);
                //produce output
                anOutput = even->model->OutPut();
                //send this output to the appropriate other models
                //external output
                if(theEventNetwork->isExternOutputModel(even->model)){ 
                    theOutputs.push_back(anOutput);
                    theOutputsTime.push_back(current2Dtime);
                    
                    OutputTimePairs<O>* tempOut = new OutputTimePairs<O>(current2Dtime->getRealTime(),current2Dtime->getDiscreteTime(),anOutput);
                    //std::cout << "queueing ouput of: "<< tempOut->getData() << std::endl;
                    theoutput.push_back(tempOut);
                }// else {//not external output
                    std::vector<EventModelInterface<I,O>*> boundTo2;//
                    boundTo2 = theEventNetwork->getModelsOutputBoundTo(even->model);
                    for (int i = 0; i < boundTo2.size(); i++) {
                        Event<I,O>* newE = new Event<I,O>();
                        newE->model = boundTo2.at(i);
                        TwoDTime *now2 = new TwoDTime(0, 0);
                        newE->myTime = current2Dtime->Advance(now2);
                        delete now2;
                        //System.out.println("queue newEven.time = "+newE->myTime->getRealTime());
                        newE->typeOfEvent = Event<I,O>::external;
                        newE->input = (I)anOutput;
                        newE->timeQueued = current2Dtime;
                        schedule.Add(newE);
                    }
                //}
                //run int
                even->model->StateTransitionCon(even->input);
                std::cout << "CON Debug: " << even->model << " " <<even->model->GetStatus() << std::endl;
                //queue new int
                even->typeOfEvent = Event<I,O>::internal;
                TwoDTime *tempTime3 = new TwoDTime(even->model->TimeAdvance(), 0);
                even->myTime = current2Dtime->Advance(tempTime3);
                delete tempTime3;
                even->timeQueued = current2Dtime;
                schedule.Add(even);
            }
                
            
        
        }catch(CantPerformAction e){
            std::cout << e.getMessage();
        }

       //std::cout << "Done with this event\n";
    }
    std::cout << "Done Simulating" << std::endl;
    std::cout << "Num Output should be :" << theoutput.size() << std::endl;
    return theoutput;
}
    
/*
template<class I, class O>
void SimulationFramework<I,O>::Run(){
    try{
        if(inEventMode){
            while(keepSimulating){
                if (theCmds.size() > 0 && !paused) {
                    I cmd;
                    cmdMutex.lock();
                        cmd = theCmds.at(0);
                        theCmds.erase(theCmds.begin());              
                    cmdMutex.unlock();
                    
                    double nextCMDtime = std::stod(cmd.at(0));
                    O out; 
                    double timeto = currentTime + theEventModel->TimeAdvance();
                    if(nextCMDtime < timeto){//external
                        double e = nextCMDtime - currentTime;
                        currentTime = nextCMDtime;                            
                        theEventModel->StateTransitionExt(RemoveFirst(cmd), e);

                    }else if(nextCMDtime > timeto){//internal

                        theCmds.insert(theCmds.begin(),cmd);//   .push_back(0, cmd);

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
                    O out;
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
                I cmd;
                cmdMutex.lock();
                    cmd = theCmds.at(0);
                    theCmds.erase(theCmds.begin());              
                cmdMutex.unlock();
                O out;
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
template<class I, class O>
std::vector<std::string> Tick(ModelInterface m, I input){
    O ret;
    try{
        ret = m.Tick(input);
    } catch(CantPerformAction c){
        //std::cout << c.getMessage() << std::endl;
    }
    return ret;
}
*/
