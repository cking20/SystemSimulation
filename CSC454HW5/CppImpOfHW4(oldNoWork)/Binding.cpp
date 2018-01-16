/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Binding.cpp
 * Author: chrisrk192
 * 
 * Created on October 18, 2017, 10:42 PM
 */

#include "Binding.h"

Binding::Binding(){
}

Binding::Binding(int t, int f) {
    to = t;
    from = f;
}

Binding::Binding(const Binding& orig) {
}

Binding::~Binding() {
}

