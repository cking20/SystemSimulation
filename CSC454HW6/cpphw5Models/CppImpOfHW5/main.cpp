/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: chrisrk192
 *
 * Created on September 29, 2017, 2:18 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "VendingMachine.h"
#include "SimulationFramework.h"
#include "Network.h"
#include "XOR.h"
#include "Mem.h"
#include "VendingEventModel.h"
#include "DrillPressModel.h"

using namespace std;

bool keepRunning = true;
std::string delimiter = ", ";
/*
 * 
 */
vector<string> SplitString(string line, string delim){
    size_t pos = 0;
    vector<string> inputs;
    while ((pos = line.find(delim)) != std::string::npos) {
            inputs.push_back(line.substr(0, pos));
            line.erase(0, pos + delim.length());
        }
    inputs.push_back(line);
    return inputs;        
}
void LoadCmdsFromFile(SimulationFramework<int,int> *fp, string fileName){
    ifstream ifs;
    ifs.open(fileName);
    if(ifs.is_open()){
        while (!ifs.eof()){
            string line;
            ifs >> line;
            vector<string> inputs = SplitString(line, ", ");
            TwoDTime t(stod(inputs[0]), stoi(inputs[1]));
            inputs.erase(inputs.begin());
            inputs.erase(inputs.begin());
            //only acccpets one int per line
            fp->AddInput(t.getRealTime(), t.getDiscreteTime(), stoi(inputs[0]));
        }
    }
    ifs.close();
}

void PerformUserAction(vector<string> inp, SimulationFramework<int,int> * fp){
    if(inp.at(0).compare("-help") == 0){
        cout << "commands" << endl
                <<"\"-run\" runs the simulation" << endl
                <<"\"-pause\" pauses the simulation" << endl
                <<"\"-load <filename>\" loads cmds from file" << endl
                //<<"\"-delim <new delim>\" changes delim" << endl
                <<"\"-getStatus\" get the status of the model" << endl
                <<"\"-quit\" quits the simulation" << endl;
                
              
    }
    if(inp.at(0).compare("-run") == 0){
        fp->paused = false;
        vector<OutputTimePairs<int>*> out(fp->RunEventSimulation());
        string comb = "";
        OutputTimePairs<int>* temp;
        
        for (int i = 0; i < out.size(); i++) {
            temp = out.at(i);
            //String[] d = temp.getData();
            int d = temp->getData();
            //if (d != NULL) {
                comb+= "(Time: "+to_string(temp->getTime())+","+to_string(temp->getDiscreteTime())+")\n";
                comb+= "Output: "+to_string(d);
                comb += "\n";
            delete temp;    
            //}
        }
        cout << comb;
        
        /*
        if(fp->producedOutput){
            vector<vector<string>> outputs = fp->FlushOutputs();
            for(int i =0; i < outputs.size(); i++){
                vector<string> output = outputs.at(i);
                for(int j = 0; j < output.size(); j++){
                    if(j < output.size()-1)
                        cout << output.at(j) << delimiter;
                    else
                        cout << output.at(j);
                }
                cout << endl;
            }
        }  */
        
        
    }
    if(inp.at(0).compare("-pause") == 0){
        fp->paused = !fp->paused;
        
    }
    if(inp.at(0).compare("-load") == 0){
        LoadCmdsFromFile(fp, inp[1]);
        
    }
    if(inp.at(0).compare("-quit") == 0){
        fp->Stop();
        keepRunning = false;
    }
    
    
}
/*
void OutputRun(SimulationFramework<int,int> *fp){
    while(keepRunning){
        if(fp->producedOutput){
            vector<vector<string>> outputs = fp->FlushOutputs();
            for(int i =0; i < outputs.size(); i++){
                vector<string> output = outputs.at(i);
                for(int j = 0; j < output.size(); j++){
                    if(j < output.size()-1)
                        cout << output.at(j) << delimiter;
                    else
                        cout << output.at(j);
                }
                cout << endl;
            }
        }     
    }
    cout << "output thread done " << endl;
}
void RunSim(SimulationFramework<int,int> *fp){
    cout << "running"<<endl;
    fp->Run();
    cout << "run thread done " << endl;
}
 */
EventNetwork<std::vector<std::string>,std::vector<std::string>>* BuildXORNet(){
    EventNetwork<std::vector<std::string>,std::vector<std::string>>* theNetwork = 
            new EventNetwork<std::vector<std::string>,std::vector<std::string>>();
    //theNetwork->numInternalTicksToExternal = 3;
    XOR *x1 = new XOR();
    XOR *x2 = new XOR();
    Mem *m = new Mem();
    std::vector<std::string> init;
    init.push_back("0");
    init.push_back("0");
    
    
    theNetwork->Add(x1);//0
    theNetwork->Add(x2);//1
    theNetwork->Add(m);//2       
    theNetwork->Bind_II(x1);        
    theNetwork->Bind_OI(x1, x2);
    theNetwork->Bind_OI(x2, m);
    theNetwork->Bind_OI(m, x2);        
    theNetwork->Bind_OO(x2);
    return theNetwork;
    
}
EventNetwork<int, int>* CreateDrillPressEventNetwork(){
        EventNetwork<int,int>* theNetwork = new EventNetwork<int,int>();
        DrillPressModel* drill = new DrillPressModel(0, 2);
        DrillPressModel* press = new DrillPressModel(0, 1);
        theNetwork->Add(press);
        theNetwork->Add(drill);
        
        theNetwork->Bind_II(press);
        theNetwork->Bind_OI(press, drill);
        theNetwork->Bind_OO(drill);
        
        return theNetwork;
    
    }
int main(int argc, char** argv) {
    
    string inputLine;
    vector<string> inputs;    
    SimulationFramework<int, int> *f = new SimulationFramework<int, int>(true,true,true, true,5000);
    EventNetwork<int, int>* theNet = CreateDrillPressEventNetwork();
    
    
    
    cout << theNet->GetStatus();
    f->SetEventNetwork(theNet);
    //thread frameworkThread (RunSim,f);
    //thread printOutputThread (OutputRun,f);
    
    do{
        try{           
            getline (cin,inputLine);
            inputs = SplitString(inputLine,delimiter);
         
            if(inputs[0].at(0) == '-')
                PerformUserAction(inputs, f);
            else{               
                TwoDTime t(stod(inputs[0]), stoi(inputs[1]));
                inputs.erase(inputs.begin());
                inputs.erase(inputs.begin());
                
                f->AddInput(t.getRealTime(), t.getDiscreteTime(), stoi(inputs.front()));
                
            }                
        } catch(CantPerformAction c){
            cout << c.getMessage() << endl;
        }
    }while(keepRunning);
    f->Stop();
    //printOutputThread.join();
    //frameworkThread.join();
    delete f;
    return 0;
}