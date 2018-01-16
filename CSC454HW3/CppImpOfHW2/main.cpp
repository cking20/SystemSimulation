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
#include "VendingMachine.h"

using namespace std;

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

int main(int argc, char** argv) {
    bool keepRunning = true;
    string inputLine;
    vector<string> inputs;
    std::string delimiter = ", ";
    VendingMachine* v = new VendingMachine();
    try{
        cout << v->GetInitilizationSignature() << endl;
        getline (cin,inputLine);
        inputs = SplitString(inputLine,delimiter);
        v->Initialize(inputs);
    } catch(CantPerformAction c){
        cout << c.getMessage();
        delete v;
        return 0;
    } catch(...){
        cout << "unknown Init error type";
        delete v;
        return 0;
    }
    do{
        try{
            cout << v->GetStatus();
            getline (cin,inputLine);
            inputs = SplitString(inputLine,delimiter);
            cout << "Output: "<<v->OutPut() << endl;
            v->StateTransition(inputs);
        } catch(CantPerformAction c){
            cout << c.getMessage() << endl;
        }
         
        if(inputLine.compare("quit") == 0)
            keepRunning = false;
    }while(keepRunning);
    delete v;
    return 0;
}


