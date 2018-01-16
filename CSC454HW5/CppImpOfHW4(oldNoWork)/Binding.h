/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Binding.h
 * Author: chrisrk192
 *
 * Created on October 18, 2017, 10:42 PM
 */

#ifndef BINDING_H
#define BINDING_H

class Binding {
public:
    Binding();
    Binding(int,int);
    Binding(const Binding& orig);
    virtual ~Binding();
    int from;
    int to;
private:

};

#endif /* BINDING_H */

