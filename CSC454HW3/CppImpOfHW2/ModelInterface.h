/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ModelInterface.h
 * Author: chrisrk192
 *
 * Created on September 29, 2017, 2:21 PM
 */
#include <vector>
#include <string>
#ifndef MODELINTERFACE_H
#define MODELINTERFACE_H

class ModelInterface {
public:
    ModelInterface();
    ModelInterface(const ModelInterface& orig);
    virtual ~ModelInterface();
    
    virtual std::string GetInitilizationSignature();
    virtual void Initialize(std::vector<std::string>);
    virtual std::string OutPut();  
    virtual void StateTransition(std::vector<std::string>);
    virtual std::string GetStatus();   
    
private:

};

struct CantPerformAction : public std::exception {
public:
    CantPerformAction(std::string msg) : m_message(msg) { }
    std::string getMessage(){
        return m_message;
    }
private:
    std::string m_message;
};
#endif /* MODELINTERFACE_H */

