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
void LoadCmdsFromFile(SimulationFramework *fp, string fileName){
    ifstream ifs;
    ifs.open(fileName);
    if(ifs.is_open()){
        while (!ifs.eof()){
            string line;
            ifs >> line;
            fp->AddInput(SplitString(line, ", "));
        }
    }
    ifs.close();
}

void PerformUserAction(vector<string> inp, SimulationFramework* fp){
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
void OutputRun(SimulationFramework* fp){
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
void RunSim(SimulationFramework* fp){
    cout << "running"<<endl;
    fp->Run();
    cout << "run thread done " << endl;
}
Network* BuildXORNet(){
    Network* theNetwork = new Network();
    theNetwork->numInternalTicksToExternal = 3;
    XOR *x1 = new XOR();
    XOR *x2 = new XOR();
    Mem *m = new Mem();
    std::vector<std::string> init;
    init.push_back("0");
    init.push_back("0");
    
    x1->Initialize(init);
    x2->Initialize(init);
    m->Initialize(init);
    theNetwork->Add(x1);//0
    theNetwork->Add(x2);//1
    theNetwork->Add(m);//2       
    theNetwork->Bind_II(0);        
    theNetwork->Bind_OI(0, 1);
    theNetwork->Bind_OI(1, 2);
    theNetwork->Bind_OI(2, 1);        
    theNetwork->Bind_OO(1);
    return theNetwork;
    
}
int main(int argc, char** argv) {
    
    string inputLine;
    vector<string> inputs;    
    SimulationFramework *f = new SimulationFramework(true,false, true,5000);
    Binding b(23,45);
    cout << b.from << " " <<b.to;
    Network *n = BuildXORNet();
    //VendingMachine v;
    //ModelInterface* m1 = &v;
    //n.numInternalTicksToExternal = 1;
    //n.Add(m1);
    //n.Bind_II(0);
    //n.Bind_OO(0);
    ModelInterface *m2 = n;
    cout << m2->GetStatus();
    f->SetModel(m2);
    thread frameworkThread (RunSim,f);
    thread printOutputThread (OutputRun,f);
    
    do{
        try{           
            getline (cin,inputLine);
            inputs = SplitString(inputLine,delimiter);
            //cout << "Output: "<<v->OutPut() << endl;
            ///v->StateTransition(inputs);
            if(inputs[0].at(0) == '-')
                PerformUserAction(inputs, f);
            else{
                f->AddInput(inputs);
            }                
        } catch(CantPerformAction c){
            cout << c.getMessage() << endl;
        }
    }while(keepRunning);
    f->Stop();
    printOutputThread.join();
    frameworkThread.join();
    delete f;
    return 0;
}